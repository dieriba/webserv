# ifndef __REQUEST_CHECKER_HPP__
# define __REQUEST_CHECKER_HPP__

# include "../utils/CommonLib.hpp"

class HttpRequest;
class TcpServer;
class Server;

class RequestChecker
{
    public:
        typedef int (*Checker)(const TcpServer *instance, const HttpRequest&);
        RequestChecker();
        ~RequestChecker();

        /*STATIC FUNCTION*/
        static const TcpServer* serverOrLocation(const Server& server, const HttpRequest& req);
        static int checkValidPath(const TcpServer *instance, const HttpRequest& req);
        static int checkAllowedMethod(const TcpServer *instance, const HttpRequest& req);
        static int checkBodySize(const TcpServer *instance, const HttpRequest& req);
        static int checkAll(Server& server, const HttpRequest& req);
        static int checkHeader(const HttpRequest& req);
        static Checker tab[10];
    
    private:
        RequestChecker& operator=(const RequestChecker& rhs);
        RequestChecker(const RequestChecker& rhs);
};
# endif