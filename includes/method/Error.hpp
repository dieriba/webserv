# ifndef __ERROR_HPP__
# define __ERROR_HPP__

# include "Method.hpp"

class ClientSocketStream;
class HttpServer;
class HttpRequest;

class Error: public Method
{
    public:
        Error();
        Error(const Error&);
        Error& operator=(const Error&);
        ~Error();

        /*MEMBER FUNCTION*/
        void addSpecificHeader(ClientSocketStream&, const int&);
        int firstStep(ClientSocketStream&, HttpResponse&, const int&);

        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        virtual Method* clone(void) const;
        
    private:
        std::string getErrorPage(const short int&) const;
};

# endif