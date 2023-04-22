# ifndef __HTTP_RESPONSE_HPP__
# define __HTTP_RESPONSE_HPP__

# include "HttpMessage.hpp"

class HttpResponse: public HttpMessage
{
    public:
        HttpResponse();
        HttpResponse(const HttpResponse& rhs);
        HttpResponse& operator=(const HttpResponse& rhs);
        ~HttpResponse();

};

# endif