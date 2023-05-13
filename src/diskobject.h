#ifndef _H_DISK_OBJECT
#define _H_DISK_OBJECT
#include <string>

class DiskObject
{
public:
    DiskObject(std::string name, std::size_t size ,bool isSSD, bool isVirtual) : m_Name(name), m_Size(size) ,m_bSSD(isSSD),m_bVirtual(isVirtual) {};
public:
    inline bool IsSSD() const { return m_bSSD; };
    inline bool IsVirtual() const { return m_bVirtual; };
    inline const std::string& GetName() const { return m_Name; };
    inline std::size_t GetSize() const { return m_Size; };
private:
    bool m_bVirtual;
    bool m_bSSD;
    std::size_t m_Size;
    std::string m_Name;
};

#endif