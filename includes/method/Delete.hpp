# ifndef __DELETE_HPP__
# define __DELETE_HPP__

# include "Method.hpp"

class IO;
class HttpRequest;

class Delete: public Method
{
    public:
        Delete();
        Delete(const Delete& rhs);
        Delete& operator=(const Delete& rhs);
        ~Delete();

        /*MEMBER FUNCTION*/
        void sendResponse(IO&, const HttpRequest&, HttpResponse&);
};


# endif