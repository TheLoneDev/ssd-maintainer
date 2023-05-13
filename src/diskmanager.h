#ifndef _H_DISK_MANAGER
#define _H_DISK_MANAGER
#include "diskobject.h"
#include <filesystem>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>

class DiskManager
{
public:
    void ScanBlockDevices(const char* path = "/sys/block");
    inline bool HasDevices() const { return static_cast<bool>(GetNumDevices()); };
    inline std::size_t GetNumDevices() const { return m_DiskObjects.size(); };
    inline auto& GetDisk(std::size_t index) const { return m_DiskObjects.at(index); };
    inline const auto& GetDisks() const { return m_DiskObjects; };
private:
    std::size_t GetDiskSize(std::string devName);
private:
    std::vector<DiskObject> m_DiskObjects;
};

#endif