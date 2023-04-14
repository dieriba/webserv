#ifndef __STRING_UTILS__
#define __STRING_UTILS__
#include "CommonLib.hpp"

class StringUtils
{
    public:
        StringUtils();
        StringUtils(const StringUtils& rhs);
        ~StringUtils();
        
        std::string rtrim(const std::string &line, const char *toTrim);
        std::string ltrim(const std::string &line, const char *toTrim);
        std::string trimBothEnd(std::string& line, const char *toTrim);
    private:
        StringUtils& operator=(const StringUtils& rhs);        
};

#endif