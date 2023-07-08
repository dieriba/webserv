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

        /*MEMBER FUNCTION*/
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);

        /*VIRTUAL MEMBER FUNCTION*/
        virtual Method* clone(void) const;
};


# endif