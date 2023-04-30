# ifndef __REQUEST_CHECKER_HPP__
# define __REQUEST_CHECKER_HPP__

# include "../CommonLib.hpp"

class HttpRequest;
class Server;
class Location;


class RequestChecker
{
    public:
        typedef int (*Checker)(const int& loc, const Server&, const HttpRequest&);
        RequestChecker();
        ~RequestChecker();

        /*STATIC FUNCTION*/
        static int checkValidPath(const int& loc, const Server& server, const HttpRequest& req);
        static int checkAllowedMethod(const int& loc, const Server& server, const HttpRequest& req);
        static int checkBodySize(const int& loc, const Server& server, const HttpRequest& req);
        static int checkAll(const Server& server, const HttpRequest& req);
        static int checkHeader(const HttpRequest& req);
        static Checker tab[10];
    
    private:
        RequestChecker& operator=(const RequestChecker& rhs);
        RequestChecker(const RequestChecker& rhs);
        static int serverOrLocation(const Server& server, const std::vector<Location>& location, const HttpRequest& req);
};
# endif