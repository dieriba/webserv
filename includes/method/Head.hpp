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

        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        virtual Method* clone(void) const;
};


# endif