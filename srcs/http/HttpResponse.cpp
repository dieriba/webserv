# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/TcpServer.hpp"


/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpResponse::HttpResponse():HttpMessage(),_method(NULL){};
HttpResponse::HttpResponse(const HttpResponse& rhs):HttpMessage(rhs),_method(rhs._method){};
HttpResponse& HttpResponse::operator=(const HttpResponse& rhs)
{
    if (this == &rhs) return *this;
    s_buffer = rhs.s_buffer;
    _body = rhs._body;
    _headers = rhs._headers;
    _method = rhs._method;
    return *this;
};
HttpResponse::~HttpResponse()
{
    delete _method;
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
Method *HttpResponse::getHttpMethod(void) const {return _method;};
Error& HttpResponse::getErrorMethod(void) {return _error;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void HttpResponse::setMethodObj(Method *method)
{
    _method = method;
}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void HttpResponse::serveResponse(IO& event, const HttpRequest& req)
{
    _method -> sendResponse(event, req);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
