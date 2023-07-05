# ifndef __ERROR_HPP__
# define __ERROR_HPP__

# include "Method.hpp"

class ClientSocketStream;
class HttpRequest;

class Error: public Method
{
    public:
        Error();
        Error(const Error&);
        Error& operator=(const Error&);
        ~Error();

        /*MEMBER FUNCTION*/
        int firstStep(ClientSocketStream&, HttpResponse&, const int&);
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);

        /*VIRTUAL MEMBER FUNCTION*/
        virtual Method* clone(void) const;
    private:
        std::string getErrorPage(const short int&) const;
};

# endif