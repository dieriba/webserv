# ifndef __DELETE_HPP__
# define __DELETE_HPP__

# include "Method.hpp"

class IO;
class HttpRequest;
class ClientSocketStream;

class Delete: public Method
{
    public:
        Delete();
        Delete(const Delete&);
        Delete& operator=(const Delete&);
        ~Delete();

        /*MEMBER FUNCTION*/
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);

        /*VIRTUAL MEMBER FUNCTION*/
        virtual Method* clone(void) const;
};


# endif