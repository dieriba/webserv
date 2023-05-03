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

        void sendResponse(const IO&, const HttpRequest&);
};

# endif