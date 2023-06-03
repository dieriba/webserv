# ifndef __ERROR_HPP__
# define __ERROR_HPP__

# include "Method.hpp"

class IO;
class HttpRequest;

class Error: public Method
{
    public:
        Error();
        Error(const Error& rhs);
        Error& operator=(const Error& rhs);
        ~Error();

        /*MEMBER FUNCTION*/
        int firstStep(IO& event, HttpResponse& res, const int& err);
        int sendResponse(IO&, HttpRequest&, HttpResponse&);
    private:
        std::string getErrorPage(const short int& err) const;
};

# endif