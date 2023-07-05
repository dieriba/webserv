# ifndef __GET_HPP__
# define __GET_HPP__

# include "Method.hpp"

class ClientSocketStream;
class HttpRequest;
class HttpServer;

class Get: public Method
{
    public:
        Get();
        Get(const Get&);
        Get& operator=(const Get&);
        virtual ~Get();

        /*MEMBER FUNCTION*/
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        int getCgiHandler(ClientSocketStream&, const HttpRequest&, HttpResponse&);
        int firstStep(ClientSocketStream&, const HttpRequest&, HttpResponse&);
        int handleChunkedRequest(HttpResponse&, std::string&);
        int directoryCgi(ClientSocketStream&, const HttpRequest&, HttpResponse&);

        /*VIRTUAL FUNCTION*/
        virtual Method* clone(void) const;
};


# endif