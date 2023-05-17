# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/server/TcpServer.hpp"


/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpResponse::HttpResponse():HttpMessage(),BitsManipulation(),_method(NULL),_directory(NULL){};
HttpResponse::HttpResponse(const HttpResponse& rhs):HttpMessage(rhs),BitsManipulation(rhs),_method(rhs._method){};
HttpResponse& HttpResponse::operator=(const HttpResponse& rhs)
{
    if (this == &rhs) return *this;
    s_buffer = rhs.s_buffer;
    _body = rhs._body;
    _headers = rhs._headers;
    _method = rhs._method;
    _directory = rhs._directory;
    _options = rhs._options;
    return *this;
};
HttpResponse::~HttpResponse()
{
    delete _method;
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
Method *HttpResponse::getHttpMethod(void) const {return _method;};
Method *HttpResponse::getHttpMethod(void) {return _method;};
Error& HttpResponse::getErrorMethod(void) {return _error;};
const std::string& HttpResponse::getPath(void) const {return _path_req;};
const DIR *HttpResponse::getDirectory(void) const {return _directory;}
std::ifstream& HttpResponse::getFile(void) {return _file;}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void HttpResponse::setMethodObj(Method *method)
{
    _method = method;
}

void HttpResponse::setPath(const std::string& path)
{
    _path_req = path;
}

void HttpResponse::setDirectory(DIR *directory)
{
    _directory = directory;
}

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void HttpResponse::clear(void)
{
    delete _method;
    _method = NULL;
    resetOptions();
}

int HttpResponse::serveResponse(IO& event, HttpRequest& req)
{
    return _method -> sendResponse(event, req, *this);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
