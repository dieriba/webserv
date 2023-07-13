# ifndef _UTILITY_METHOD_HPP__
# define _UTILITY_METHOD_HPP__
# include "CommonLib.hpp"

class IO;
class HttpResponse;
class Method;

class UtilityMethod
{
    public:
        UtilityMethod();
        UtilityMethod(const UtilityMethod&);
        ~UtilityMethod();
        
        /*STRING MANIPULATION*/
        static int basicCgiSetup(IO&, HttpResponse&);
        static void deleteEventFromEpollInstance(const int&, const int&);
        static int sendBuffer(int, const char *, int);
        static std::string getDateAndTime(void);
        static std::string rtrim(const std::string& ,const char *);
        static std::string ltrim(const std::string& ,const char *);
        static std::string trimBothEnd(std::string& ,const char *);
        static std::vector<std::string> stringSpliter(const std::string& , const std::string&);
        static size_t  count(const std::string& , char);
        static int switchEvents(const int&, uint32_t, struct epoll_event&, IO&);
        static std::string numberToString(const size_t&);
        static const std::string& getMimeType(std::string&, const std::string&, const std::string&, const bool&);
        static std::string getFileExtension(const std::string&, const bool&);
        static std::string decimalToHex(const int&);
        static size_t hexToDecimal(const std::string&);
        static int is_a_directory(const char *);
        static size_t myStrlen(const char *);
        static std::string remove_dup(std::string&);
    
    private:
        UtilityMethod& operator=(const UtilityMethod&);        
};

#endif