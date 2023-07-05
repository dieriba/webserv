# ifndef __REQUEST_CHECKER_HPP__
# define __REQUEST_CHECKER_HPP__

# include "../utils/CommonLib.hpp"

class HttpServer;
class Server;
class ClientSocketStream;
class HttpRequest;
class HttpResponse;

class RequestChecker
{
    public:
        typedef int (*Checker)(const HttpServer&, HttpRequest&);
        RequestChecker();
        ~RequestChecker();

        /*STATIC FUNCTION*/
        static const HttpServer* serverOrLocation(const Server&, const HttpRequest&);
        static int checkAllowedMethod(const HttpServer&, HttpRequest&);
        static int checkBodySize(const HttpServer&, HttpRequest&);
        static int checkAll(ClientSocketStream&, HttpRequest&);
        static int checkGetMethod(const HttpServer&, HttpRequest&);
        static int checkPostMethod(const HttpServer&, HttpRequest&);
        static int checkDeleteMethod(const HttpServer&, HttpRequest&);
        static int checkHeader(const HttpServer&, HttpRequest&);
        static Checker tab[10];
    
    private:
        RequestChecker& operator=(const RequestChecker&);
        RequestChecker(const RequestChecker&);
};
# endif