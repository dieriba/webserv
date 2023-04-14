#ifndef __STRING_UTILS__
#define __STRING_UTILS__
#include "CommonLib.hpp"
#include <sstream>
class StringUtils
{
    public:
        StringUtils();
        StringUtils(const StringUtils& rhs);
        ~StringUtils();
        
        std::string rtrim(const std::string &line, const char *toTrim);
        std::string ltrim(const std::string &line, const char *toTrim);
        std::string trimBothEnd(std::string& line, const char *toTrim);
        std::vector<std::string> stringSpliter(const std::string& line, const std::string& delim);
        size_t  count(const std::string& line, char to_count);
    private:
        StringUtils& operator=(const StringUtils& rhs);        
};

#endif