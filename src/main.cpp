#include "utils.h"
#include "diskmanager.h"
#include <iostream>

#define DEF_BLOCKSIZE 4096

void PrintWelcome();

int main()
{
    
    PrintWelcome();

    if(!IsRoot())
    {
        puts("Not running as root, make sure user has permissions");
    }
    
    DiskManager diskMgr;
    diskMgr.ScanBlockDevices();

    if(!diskMgr.HasDevices())
    {
        std::cerr << "Could not detect any block devices." << std::endl; // Probably permission issue, 
                                                                         // since Linux must have at least 1 block device, right? ᕦ(⩾﹏⩽)ᕥ
        return -1;
    }

    std::cout << "Choose which disk to fully read:\n";
    std::size_t numDisks = diskMgr.GetNumDevices(); // Just to make sure GetNumDevices() is not called everytime
    for(std::size_t i = 0; i < numDisks; i++)
    {
        auto& disk = diskMgr.GetDisk(i);
        std::cout << "(" << i+1 << ") " << disk.GetName();
        if(disk.IsSSD())
            std::cout << " (SSD)";

        if(disk.IsVirtual())
            std::cout << " (Virtual)";

        std::cout << '\n';
    }

    std::string userInput;
    std::size_t inNum = 0;
    //bool inputValid = false;
    while(1) // while(inputValid)
    {
        std::getline(std::cin, userInput);
        if(!userInput.size())
        {
            std::cout << "Empty input was entered\n";
            continue;
        }
        
        if(!IsStrNumber(userInput))
        {
            std::cout << "Input is not only a number\n";
            continue;
        }
        
        inNum = std::stoull(userInput);

        // if (num < 0) unsigned cannot be lower than 0 anyway lol

        if(!inNum)
        {
            std::cout << "Index starts from 1" << std::endl;
            continue;
        }

        if (inNum > numDisks)
        {
            std::cout << "Number is bigger than disks available\n";
            continue;
        }

        std::cout << "Selected number " << inNum << '\n';
        //inputValid = true;
        break;
    }

    if(!inNum)
    {
        std::cerr << "Error, selected index is somehow 0" << std::endl;
        return -2;
    }

    auto& disk = diskMgr.GetDisk(inNum - 1);

    if(disk.IsVirtual())
    {
        std::cout << "This disk is a virtual disk, are you sure you want to continue? (y/n):";
        std::getline(std::cin, userInput);
        if(userInput.empty() || std::tolower(userInput[0]) != 'y')
            return -3;
    }

    if(!disk.IsSSD())
    {
        std::cout << "This disk is doesn't seem to be an SSD, are you sure you want to continue? (y/n):";
        std::getline(std::cin, userInput);
        if(userInput.empty() || std::tolower(userInput[0]) != 'y')
            return -4;
    }

    auto diskDevPath = std::filesystem::path("/dev") / disk.GetName();
    if(!std::filesystem::exists(diskDevPath))
    {
        std::cerr << "Could not access dev file (" << diskDevPath << ')' << std::endl;
        return -5;
    }

    uint64_t diskSize = disk.GetSize();
    uint64_t numBlocks = diskSize / DEF_BLOCKSIZE;

    std::cout << "Opening " << diskDevPath << "...\n";
    std::ifstream diskDev(diskDevPath, std::ios::in);

    if(!diskDev.is_open())
    {
        std::cerr << "Could not open " << diskDevPath << std::endl;
        return -6;
    }


    char buffer[DEF_BLOCKSIZE];

    std::cout << "Reading " << diskDevPath << "\n\n";
    for(uint64_t i = 0; i < numBlocks; i++)
    {
        std::cout << "\033[1A" << "\r\x1B[K";
        std::cout << disk.GetName() << ':' <<(i * 4096) << '/' << diskSize;
        if(!i)
            std::cout << "(0%)\n";
        else
            std::cout << '(' << (static_cast<double>(i) / static_cast<double>(numBlocks) * 100) << "%)\n";

        diskDev.read(buffer, DEF_BLOCKSIZE);
    }

    std::cout << "Finished!" << std::endl;

    return 0;
}

void PrintWelcome()
{
    std::cout << "Welcome to ssd-maintainer.\n"
                 "ssd-maintainer helps with SSDs that don't have data degradation mitigations\n"
                 "Don't use often, since multiple(miliions+) readings like this WILL shorten the SSD lifespan\n";
}