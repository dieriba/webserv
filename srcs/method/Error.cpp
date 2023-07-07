# include "../../includes/method/Error.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Error::Error(){};
Error::Error(const Error& rhs):Method(rhs){};
Error& Error::operator=(const Error& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Error::~Error(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

std::string Error::getErrorPage(const short int& err) const
{
    std::string res;

    switch (err)
    {
        case FORBIDEN : res = SERVER_ERROR_PAGE_FORBIDDEN;
            break;
        
        case NOT_FOUND : res = SERVER_ERROR_PAGE_NOT_FOUND;
            break;

        case METHOD_NOT_ALLOWED : res = SERVER_ERROR_PAGE_METHOD_NOT_ALLOWED;
            break;

        case REQUEST_TIMEOUT : res = SERVER_ERROR_PAGE_REQUEST_TIMEOUT;
            break;

        case TOO_LARGE_CONTENT : res = SERVER_ERROR_TOO_LARGE_CONTENT;
            break;

        case UNSUPPORTED_MEDIA_TYPE : res = SERVER_ERROR_PAGE_UNSUPPORTED_MEDIA_TYPE;
            break;

        case INTERNAL_SERVER_ERROR : res = SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR;
            break;

        case METHOD_NOT_SUPPORTED : res = SERVER_ERROR_PAGE_METHOD_NOT_SUPPORTED;
            break;

        case GATEWAY_TIMEOUT : res = SERVER_ERROR_PAGE_GATEWAY_TIMEOUT;
            break;

        case VERSION_NOT_SUPPORTED : res = SERVER_ERROR_PAGE_VERSION_NOT_SUPPORTED;
            break;

        default: res = SERVER_ERROR_PAGE_BAD_REQUEST;
            break;
    }
    
    return res;
}

int Error::firstStep(ClientSocketStream& event, HttpResponse& res, const int& err)
{
    std::map<short int, std::string>& _map = event.getServer() -> getInstance() -> getErrorMaps();
    std::map<short int, std::string>::iterator it = _map.find(event.getErrStatus());

    if (it == _map.end()) it = _map.find(DEFAULT_ERROR_PAGE_VALUE);
    
    if (it == _map.end()) return IO::IO_ERROR;

    if (access(it -> second.c_str(), F_OK) != 0) return IO::IO_ERROR;

    if (access(it -> second.c_str(), R_OK) != 0) return IO::IO_ERROR;

    std::ifstream& file = res.getFile();

    file.open(it -> second.c_str(), std::ifstream::in | std::ifstream::binary);

    if (!file) return IO::IO_ERROR;
        
    file.seekg(0, std::ios::end);
    res.setBodySize(file.tellg());
    file.seekg(0, std::ios::beg);

    if (file.fail()) return IO::IO_ERROR;

    makeStatusLine(event, err);
    appendToResponse(CONTENT_TYP, UtilityMethod::getMimeType(it -> second, "", "", false));
    appendToResponse(CONTENT_LEN, UtilityMethod::numberToString(res.getBodySize()));
    _response += CRLF;

    if (UtilityMethod::sendBuffer(event.getFd(), _response.c_str(), _response.size()) == IO::IO_ERROR) return (IO::IO_ERROR);
    
    _response.clear();
    res.setOptions(HttpResponse::HTTP_RESPONSE_STARTED, SET);
    res.setOptions(HttpResponse::HTTP_RESPONSE_FILE, SET);

    return IO::IO_SUCCESS;
}

int Error::sendResponse(ClientSocketStream& client, HttpRequest& req, HttpResponse& res)
{    
    HttpServer& instance = *(client.getServer() -> getInstance());
    int err = IO::IO_SUCCESS;

    if (instance.checkBits(HttpServer::HTTP_SERVER_ERROR_PAGE_SET))
    {
        if (!res.checkBits(HttpResponse::HTTP_RESPONSE_STARTED))
            err = firstStep(client, res, client.getErrStatus());

        if (err == IO::IO_SUCCESS)
        {
            if (req.getMethod() == HttpServer::HTTP_SERVER_HEAD)
            {
                res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
                return IO::IO_ERROR;
            }

            if (res.checkBits(HttpResponse::HTTP_RESPONSE_FILE))
                return handleFileRessource(client, res);
        }

    }
    
    std::string error_page = getErrorPage(client.getErrStatus());
    size_t len = error_page.size();

   // err = IO::IO_SUCCESS;
    
    if (req.getMethod() == HttpServer::HTTP_SERVER_HEAD)
    {
        len = error_page.find(CRLF CRLF) + 4;
        error_page[len] = 0;
        err = IO::IO_ERROR;
    }

    if (UtilityMethod::sendBuffer(client.getFd(), error_page.c_str(), len) == IO::IO_ERROR)
        return (IO::IO_ERROR);
        
    res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);

    return err;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Error::clone(void) const {return new Error; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
