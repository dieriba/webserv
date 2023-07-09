# ifndef __GET_HPP__
# define __GET_HPP__

# include "Method.hpp"

class HttpServer;

class Get: public Method
{
    public:
        Get();
        Get(const Get&);
        Get& operator=(const Get&);
        virtual ~Get();

        /*MEMBER FUNCTION*/
        int getCgiHandler(ClientSocketStream&, const HttpRequest&, HttpResponse&);
        int firstStep(ClientSocketStream&, const HttpRequest&, HttpResponse&);
        int directoryCgi(ClientSocketStream&, const HttpRequest&, HttpResponse&);

        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        virtual Method* clone(void) const;
};


# endif