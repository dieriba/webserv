# ifndef _UTILITY_METHOD_HPP__
# define _UTILITY_METHOD_HPP__
# include "CommonLib.hpp"

class IO;

class UtilityMethod
{
    public:
        UtilityMethod();
        UtilityMethod(const UtilityMethod& rhs);
        ~UtilityMethod();
        
        /*STRING MANIPULATION*/
        static std::string rtrim(const std::string &line, const char *toTrim);
        static std::string ltrim(const std::string &line, const char *toTrim);
        static std::string trimBothEnd(std::string& line, const char *toTrim);
        static std::vector<std::string> stringSpliter(const std::string& line, const std::string& delim);
        static size_t  count(const std::string& line, char to_count);
        static int switchEvents(const int& _ws, uint32_t mode, struct epoll_event& event, IO& ev);
        static std::string numberToString(const size_t& number);
        static const std::string& getMimeType(std::string& path, const std::string& index_path, const std::string& index, const bool& to_check);
        static std::string getFileExtension(const std::string& content_type, const short int& opt);
        static std::string decimalToHex(const int& number);
        static size_t hexToDecimal(const std::string& hex);
        static char *mystrstr(const char *big, const char *needle);
        static int is_a_directory(const char *path);
        static size_t myStrlen(const char *buffer);
        static std::string remove_dup(std::string& ressource);
    
    private:
        UtilityMethod& operator=(const UtilityMethod& rhs);        
};

#endif