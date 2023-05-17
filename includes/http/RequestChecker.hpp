# ifndef __REQUEST_CHECKER_HPP__
# define __REQUEST_CHECKER_HPP__

# include "../utils/CommonLib.hpp"

class TcpServer;
class Server;
class IO;
class HttpRequest;
class HttpResponse;

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
        static int checkAll(IO& object, HttpRequest& req, HttpResponse& res);
        static int checkHeader(HttpRequest& req, HttpResponse& res);
        static Checker tab[10];
    
    private:
        RequestChecker& operator=(const RequestChecker& rhs);
        RequestChecker(const RequestChecker& rhs);
};
# endif