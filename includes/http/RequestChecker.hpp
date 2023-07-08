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

        /*STATIC FUNCTION*/
        static const HttpServer* serverOrLocation(const Server&, const HttpRequest&);
        static int checkAllowedMethod(const HttpServer&, HttpRequest&);
        static int checkBodySize(const HttpServer&, HttpRequest&);
        static int checkAll(ClientSocketStream&, HttpRequest&);
        static int checkGetHeadMethod(const HttpServer&, HttpRequest&);
        static int checkPostPutMethod(const HttpServer&, HttpRequest&);
        static int checkOptionsMethod(const HttpServer&, HttpRequest&);
        static int checkDeleteMethod(const HttpServer&, HttpRequest&);
        static int checkHeader(const HttpServer&, HttpRequest&);
        static std::vector<Checker> tab;
    
    private:
        RequestChecker(const RequestChecker&);
};
# endif