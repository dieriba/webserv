# ifndef __HEAD_HPP__
# define __HEAD_HPP__

# include "Method.hpp"

class ClientSocketStream;
class HttpRequest;
class HttpServer;

class Head: public Method
{
    public:
        Head();
        Head(const Head&);
        Head& operator=(const Head&);
        virtual ~Head();

        /*MEMBER FUNCTION*/
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);

        /*VIRTUAL FUNCTION*/
        virtual Method* clone(void) const;
};


# endif