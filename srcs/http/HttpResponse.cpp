# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/TcpServer.hpp"


/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpResponse::HttpResponse():HttpMessage(){};
HttpResponse::HttpResponse(const HttpResponse& rhs):HttpMessage(rhs){};
HttpResponse& HttpResponse::operator=(const HttpResponse& rhs)
{
    if (this == &rhs) return *this;
    s_buffer = rhs.s_buffer;
    return *this;
};
HttpResponse::~HttpResponse(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void HttpResponse::setMethodObj(Method *method)
{
    _method = method;
}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void HttpResponse::makeStatusLine(const std::string& version, const std::string& method)
{
    s_buffer = version + " " + method + " " + TcpServer::getHttpResponse(OK) -> second + CRLF;
}


void HttpResponse::serveResponse(const IO& event, const HttpRequest& req)
{
    _method -> sendResponse(event, req);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
