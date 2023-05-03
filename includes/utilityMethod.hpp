#ifndef _UTILITY_METHOD_HPP__
#define _UTILITY_METHOD_HPP__
#include "CommonLib.hpp"

class utilityMethod
{
    public:
        utilityMethod();
        utilityMethod(const utilityMethod& rhs);
        ~utilityMethod();
        
        /*STRING MANIPULATION*/
        static std::string rtrim(const std::string &line, const char *toTrim);
        static std::string ltrim(const std::string &line, const char *toTrim);
        static std::string trimBothEnd(std::string& line, const char *toTrim);
        static std::vector<std::string> stringSpliter(const std::string& line, const std::string& delim);
        static size_t  count(const std::string& line, char to_count);

    
    private:
        utilityMethod& operator=(const utilityMethod& rhs);        
};

#endif