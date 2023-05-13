#include "diskmanager.h"

void DiskManager::ScanBlockDevices(const char* path)
{
    if(!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        return;

    m_DiskObjects.clear();

    for(const auto& dirEntry : std::filesystem::directory_iterator(path))
    {
        if(dirEntry.is_symlink())
        {
            bool virt = false, ssd = false;
            auto target = std::filesystem::read_symlink(dirEntry.path());
            if(!target.string().compare(0, strlen("../devices/virtual"), "../devices/virtual"))
                virt = true;

            std::filesystem::path rotFilePath = std::filesystem::path(path) / target / "queue" / "rotational";
            std::ifstream rotFile(rotFilePath, std::ios::in);

            if(rotFile.is_open())
            {
                char c = 0;
                rotFile.get(c);
                if(c == '0')
                    ssd = true;
                
                rotFile.close();
            }
            
            m_DiskObjects.push_back(DiskObject(dirEntry.path().filename().string(), GetDiskSize(dirEntry.path().filename().string()) ,ssd, virt));
        }
    }
}

std::size_t DiskManager::GetDiskSize(std::string devName)
{
    // Haven't found a C++ way to do it,
    // So using Linux/POSIX C-API unfortunately
    int diskFD = open(std::string("/dev/").append(devName).c_str(), O_RDONLY);
    if(diskFD == -1)
        return 0;

    uint64_t diskSize = 0;
    if(ioctl(diskFD, BLKGETSIZE64, &diskSize) == -1)
    {
        close(diskFD);
        return 0;
    }
    
    return diskSize;
}