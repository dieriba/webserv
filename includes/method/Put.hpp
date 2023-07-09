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

        /*VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int open_file(ClientSocketStream&);
        virtual int open_file(ClientSocketStream&, std::string& filepath);
        
        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        virtual Method* clone(void) const;
};

# endif