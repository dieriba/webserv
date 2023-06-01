# ifndef __REQUEST_CHECKER_HPP__
# define __REQUEST_CHECKER_HPP__

# include "../utils/CommonLib.hpp"

class HttpServer;
class Server;
class IO;
class HttpRequest;
class HttpResponse;

class RequestChecker
{
    public:
        typedef int (*Checker)(const HttpServer *instance, HttpRequest&);
        RequestChecker();
        ~RequestChecker();

        /*STATIC FUNCTION*/
        static const HttpServer* serverOrLocation(const Server& server, const HttpRequest& req);
        static int checkValidPath(const HttpServer *instance, HttpRequest& req);
        static int checkAllowedMethod(const HttpServer *instance, HttpRequest& req);
        static int checkBodySize(const HttpServer *instance, HttpRequest& req);
        static int checkAll(IO& object, HttpRequest& req);
        static int checkGetMethod(const HttpServer& instance, HttpRequest& req);
        static int checkPostMethod(const HttpServer& instance, HttpRequest& req);
        static int checkDeleteMethod(const HttpServer& instance, HttpRequest& req);
        static int checkHeader(const HttpServer& instance, HttpRequest& req);
        static Checker tab[10];
    
    private:
        RequestChecker& operator=(const RequestChecker& rhs);
        RequestChecker(const RequestChecker& rhs);
};
# endif