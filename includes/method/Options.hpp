# ifndef __OPTIONS_HPP__
# define __OPTIONS_HPP__

# include "Method.hpp"

class IO;
class HttpRequest;
class ClientSocketStream;

class Options: public Method
{
    public:
        Options();
        Options(const Options&);
        Options& operator=(const Options&);
        ~Options();

        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        virtual Method* clone(void) const;
};


# endif