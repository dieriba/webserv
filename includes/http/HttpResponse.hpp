# ifndef __HTTP_RESPONSE_HPP__
# define __HTTP_RESPONSE_HPP__

# include "HttpMessage.hpp"
# include "../method/Method.hpp"

class HttpRequest;
class IO;

class HttpResponse: public HttpMessage
{
    public:
        HttpResponse();
        HttpResponse(const HttpResponse& rhs);
        HttpResponse& operator=(const HttpResponse& rhs);
        ~HttpResponse();

        /*MEMBER FUNCTION*/
        void Get_(const IO&, const HttpRequest&);
        void Post_(const IO&, const HttpRequest&);
        void Delete_(const IO&, const HttpRequest&);
        void serveResponse(const IO&, const HttpRequest&);
        void makeStatusLine(const std::string& version, const std::string& method);

        /*SETTER*/
        void setMethodObj(Method *method);
    
    private:
        Method  *_method;
};

# endif