# ifndef __HTTP_RESPONSE_HPP__
# define __HTTP_RESPONSE_HPP__

# include "HttpMessage.hpp"
# include "../method/Method.hpp"
# include "../method/Error.hpp"

class HttpRequest;
class IO;

class HttpResponse: public HttpMessage
{
    public:
        HttpResponse();
        HttpResponse(const HttpResponse& rhs);
        HttpResponse& operator=(const HttpResponse& rhs);
        ~HttpResponse();

        /*GETTER*/
        Error& getErrorMethod(void);
        Method *getHttpMethod(void) const;

        /*SETTER*/
        void setMethodObj(Method *method);
    
        /*MEMBER FUNCTION*/
        void serveResponse(IO&, const HttpRequest&);

    private:
        Method  *_method;
        Error   _error;
};

# endif