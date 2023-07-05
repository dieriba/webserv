# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/server/HttpServer.hpp"


/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpResponse::HttpResponse():HttpMessage(),BitsManipulation(),_method(NULL),_directory(NULL)
{
    _pipes[0] = HttpResponse::READ_END;
    _pipes[1] = HttpResponse::WRITE_END;
};

HttpResponse::HttpResponse(const HttpResponse& rhs):HttpMessage(rhs),BitsManipulation(rhs),_method(rhs._method)
{
    _pipes[0] = rhs._pipes[0];
    _pipes[1] = rhs._pipes[1];
};

HttpResponse& HttpResponse::operator=(const HttpResponse& rhs)
{
    if (this == &rhs) return *this;
    _pipes[0] = rhs._pipes[0];
    _pipes[1] = rhs._pipes[1];
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
const int& HttpResponse::getReadEnd(void) const {return _pipes[0];};
const int& HttpResponse::getWriteEnd(void) const {return _pipes[1];};
int *HttpResponse::getPipes(void) {return _pipes;}
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

void HttpResponse::clearReadEnd(void)
{
    if (_pipes[0] == HttpResponse::READ_END) return ;

    close(_pipes[0]);
    _pipes[0] = HttpResponse::READ_END;
}

void HttpResponse::clearWriteEnd(void)
{
    if (_pipes[1] == HttpResponse::WRITE_END) return ;

    close(_pipes[1]);
    _pipes[1] = HttpResponse::WRITE_END;
}

void HttpResponse::clearBothEnd(void)
{
    clearReadEnd();
    clearWriteEnd();
}

void HttpResponse::clear(void)
{
    delete _method;
    _method = NULL;
    _file.clear();
    
    if (_file.is_open()) _file.close();
    
    resetOptions();
}

int HttpResponse::serveResponse(ClientSocketStream& client, HttpRequest& req)
{
    return _method -> sendResponse(client, req, *this);
}

int HttpResponse::switchMethod(IO& event, const short int& method, const short int& status)
{
    delete _method;
    event.setErrorStatus(status);
    _method = Method::_tab[method]();
    return (1);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
