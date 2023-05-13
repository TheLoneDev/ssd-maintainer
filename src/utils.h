#ifndef _H_UTILS
#define _H_UTILS
#include <unistd.h>
#include <string>

inline bool IsRoot()
{
    return !getuid();
}

inline bool IsStrNumber(const std::string& str)
{
    for(char c : str)
    {
        if(c && (c < '0' || c > '9'))
            return false;
    }
    return true;
}

#endif