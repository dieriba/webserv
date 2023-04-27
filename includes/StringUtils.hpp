#ifndef __STRING_UTILS__
#define __STRING_UTILS__
#include "CommonLib.hpp"

class StringUtils
{
    public:
        StringUtils();
        StringUtils(const StringUtils& rhs);
        ~StringUtils();
        
        static std::string rtrim(const std::string &line, const char *toTrim);
        static std::string ltrim(const std::string &line, const char *toTrim);
        static std::string trimBothEnd(std::string& line, const char *toTrim);
        static std::vector<std::string> stringSpliter(const std::string& line, const std::string& delim);
        static size_t  count(const std::string& line, char to_count);
    private:
        StringUtils& operator=(const StringUtils& rhs);        
};

#endif