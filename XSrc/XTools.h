#ifndef __XCONFIG_H__
#define __XCONFIG_H__

#include <string>

class XTools
{
public:
    static std::string GBKToUTF8(const std::string& strGBK);

    static std::string UTF8ToGBK(const std::string& strUTF8);
};

#endif // ifndef
