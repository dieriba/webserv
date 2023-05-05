# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/TcpServer.hpp"


/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
<<<<<<< HEAD
HttpResponse::HttpResponse():HttpMessage(),BitsManipulation(),_method(NULL),_directory(NULL){};
HttpResponse::HttpResponse(const HttpResponse& rhs):HttpMessage(rhs),BitsManipulation(rhs),_method(rhs._method){};
=======
HttpResponse::HttpResponse():HttpMessage(),_method(NULL){};
HttpResponse::HttpResponse(const HttpResponse& rhs):HttpMessage(rhs),_method(rhs._method){};
>>>>>>> origin/main
HttpResponse& HttpResponse::operator=(const HttpResponse& rhs)
{
    if (this == &rhs) return *this;
    s_buffer = rhs.s_buffer;
    _body = rhs._body;
    _headers = rhs._headers;
    _method = rhs._method;
<<<<<<< HEAD
    _directory = rhs._directory;
=======
>>>>>>> origin/main
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
<<<<<<< HEAD
const std::string& HttpResponse::getPath(void) const {return _path_req;};
const DIR *HttpResponse::getDirectory(void) const {return _directory;}
std::ifstream& HttpResponse::getFile(void) {return _file;}
=======
>>>>>>> origin/main
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void HttpResponse::setMethodObj(Method *method)
{
    _method = method;
}
<<<<<<< HEAD

void HttpResponse::setPath(const std::string& path)
{
    _path_req = path;
}

void HttpResponse::setDirectory(DIR *directory)
{
    _directory = directory;
}

=======
>>>>>>> origin/main
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void HttpResponse::serveResponse(IO& event, const HttpRequest& req)
{
<<<<<<< HEAD
    _method -> sendResponse(event, req, *this);
=======
    _method -> sendResponse(event, req);
>>>>>>> origin/main
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
