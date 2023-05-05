# ifndef _UTILITY_METHOD_HPP__
# define _UTILITY_METHOD_HPP__
# include "CommonLib.hpp"

class IO;

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
        static void switchEvents(const int& _ws, uint32_t mode, struct epoll_event& event, const IO& ev);
        static std::string numberToString(const int& number);
        static const std::string& getMimeType(std::string& path, const std::string& index_path, const std::string& index);
        static std::string decimalToHex(const int& number);
    
    private:
        utilityMethod& operator=(const utilityMethod& rhs);        
};

#endif