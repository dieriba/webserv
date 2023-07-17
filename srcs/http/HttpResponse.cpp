# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/server/HttpServer.hpp"


/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpResponse::HttpResponse():HttpMessage(),BitsManipulation(),_method(NULL),_directory(NULL)
{
    _pipes[0] = HttpResponse::HTTP_RESPONSE_READ_END;
    _pipes[1] = HttpResponse::HTTP_RESPONSE_WRITE_END;
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
std::string& HttpResponse::getResponse(void) {return _response;}
const std::string& HttpResponse::getResponse(void) const {return _response;}
const int& HttpResponse::getReadEnd(void) const {return _pipes[0];};
const short int& HttpResponse::getStatus(void) const { return _status; }
const int& HttpResponse::getWriteEnd(void) const {return _pipes[1];};
int *HttpResponse::getPipes(void) {return _pipes;}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
HttpResponse& HttpResponse::setStatus(const short int& status = BAD_REQUEST)
{
    _status = status;

    return *this;
}


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

void HttpResponse::setFd(const int& fd)
{
    _fd = fd;
}

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void HttpResponse::clearReadEnd(void)
{
    if (_pipes[0] == HttpResponse::HTTP_RESPONSE_READ_END) return ;

    close(_pipes[0]);
    _pipes[0] = HttpResponse::HTTP_RESPONSE_READ_END;
}

void HttpResponse::clearWriteEnd(void)
{
    if (_pipes[1] == HttpResponse::HTTP_RESPONSE_WRITE_END) return ;

    close(_pipes[1]);
    _pipes[1] = HttpResponse::HTTP_RESPONSE_WRITE_END;
}

HttpResponse& HttpResponse::appendToResponse(const char* buffer, const size_t& size)
{
    _response.append(buffer, size);

    return *this;
}

HttpResponse& HttpResponse::appendToResponse(const std::string& to_append)
{
    _response.append(to_append);

    return *this;
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
    _response.clear();

    if (_file.is_open()) _file.close();
    
    resetOptions();
}

int HttpResponse::serveResponse(ClientSocketStream& client, HttpRequest& req)
{
    return _method -> sendResponse(client, req, *this);
}

int HttpResponse::setErrorObjectResponse(const short int& status)
{
    delete _method;
    _status = status;
    _method = new Error;
    return IO::IO_ERROR;
}

int HttpResponse::sendRedirect(const ClientSocketStream& client, HttpResponse& res, const char *status_line)
{
    if (UtilityMethod::sendBuffer(client.getFd(), status_line, UtilityMethod::myStrlen(status_line)) == IO::IO_ERROR)
            return IO::IO_ERROR;
    
    const std::string& redirect_url = client.getServer() -> getInstance() -> getRedirect();

    if (UtilityMethod::sendBuffer(client.getFd(), redirect_url.c_str(), redirect_url.size()) || UtilityMethod::sendBuffer(client.getFd(), CRLF CRLF, UtilityMethod::myStrlen(CRLF CRLF)))
        return IO::IO_ERROR;
            
    res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    return IO::IO_SUCCESS;
}

int HttpResponse::sendResponse(void)
{
    short int res = IO::IO_SUCCESS;

    if (UtilityMethod::sendBuffer(_fd, _response.c_str(), _response.size()) == IO::IO_ERROR)
        res = IO::IO_ERROR;
    
    _response.clear();
    
    return res;
}

int HttpResponse::sendResponse(const char *buffer)
{
    short int res = IO::IO_SUCCESS;

    if (UtilityMethod::sendBuffer(_fd, buffer, std::strlen(buffer)) == IO::IO_ERROR)
        res = IO::IO_ERROR;
    
    _response.clear();
    
    return res;
}

int HttpResponse::sendResponse(const char *buffer, const size_t& size)
{
    short int res = IO::IO_SUCCESS;

    if (UtilityMethod::sendBuffer(_fd, buffer, size) == IO::IO_ERROR)
        res = IO::IO_ERROR;
    
    _response.clear();
    
    return res;
}

HttpResponse& HttpResponse::addCustomHeader(ClientSocketStream& client, const short int& err)
{
    HttpResponse& res = client.getResponse();

    switch (err)
    {
        case METHOD_NOT_ALLOWED: res.setHeader("Allow", client.getAllowedMethod(*(client.getServer() -> getInstance()), HttpServer::_httpMethods));
            break;
        
        default:
            break;
    }

    return *this;
}

HttpResponse& HttpResponse::setCookieHeader(IO& object)
{
    static int counter = 0;

    std::time_t currentTime = std::time(NULL);
    std::string uniqueCookie = UtilityMethod::numberToString(currentTime) + "_" + UtilityMethod::numberToString(counter++);
    
    uniqueCookie = WELCOME_COOKIES + uniqueCookie + "; PATH=/;";
    setHeader(SET_COOKIE, uniqueCookie);
    object.setOptions(IO::IO_COOKIE, SET);

    return *this;
}

HttpResponse& HttpResponse::makeStatusLine(IO& object, const int& status)
{
    std::string version(HTTP_VERSION);
    std::ostringstream ss;
    
    ss << status;
    std::string code(ss.str());
    
    _response = version + " " + code + " " + HttpServer::getHttpResponse(status) -> second + CRLF SERVER_NAME + UtilityMethod::getDateAndTime() + CRLF;

    if (object.checkBits(IO::IO_COOKIE) == 0) setCookieHeader(object);

    return *this;
}

HttpResponse& HttpResponse::addEndHeaderCRLF(void)
{
    _response += CRLF;

    return *this;
}

HttpResponse& HttpResponse::setHeader(const std::string& key, const std::string& value)
{
    _response += key + ": " + value + CRLF;

    return *this;
}

HttpResponse& HttpResponse::addCustomHeader(const HttpServer& instance)
{
    if (instance.checkBits(HttpServer::HTTP_SERVER_CUSTOM_HEADER) == 0)
        return *this;

    const std::map<std::string, std::string>& map = instance.getHeadersMap();
    std::map<std::string, std::string>::const_iterator it = map.begin();
    for (; it != map.end(); it++)
        setHeader(it -> first, it -> second);

    return *this;
} 
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
