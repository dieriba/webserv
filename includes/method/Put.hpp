# ifndef __PUT_HPP__
# define __PUT_HPP__

# include "Method.hpp"
# include "../utils/FileWriter.hpp"

class ClientSocketStream;
class HttpRequest;

class Put: public Method, public FileWriter
{
    public:
        Put();
        Put(const Put&);
        Put& operator=(const Put&);
        virtual ~Put();

        /*MEMBER FUNCTION*/
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);

        /*VIRTUAL MEMBER FUNCTION*/
        virtual Method* clone(void) const;

};

# endif