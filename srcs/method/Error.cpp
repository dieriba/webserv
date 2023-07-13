# include "../../includes/method/Error.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/server/HttpServer.hpp"
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
    switch (err)
    {
        case FORBIDEN : return SERVER_ERROR_PAGE_FORBIDDEN;
            
        
        case NOT_FOUND : return SERVER_ERROR_PAGE_NOT_FOUND;
            

        case METHOD_NOT_ALLOWED : return SERVER_ERROR_PAGE_METHOD_NOT_ALLOWED;
            

        case REQUEST_TIMEOUT : return SERVER_ERROR_PAGE_REQUEST_TIMEOUT;
            

        case TOO_LARGE_CONTENT : return SERVER_ERROR_TOO_LARGE_CONTENT;
            

        case UNSUPPORTED_MEDIA_TYPE : return SERVER_ERROR_PAGE_UNSUPPORTED_MEDIA_TYPE;
            

        case INTERNAL_SERVER_ERROR : return SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR;
            

        case METHOD_NOT_SUPPORTED : return SERVER_ERROR_PAGE_METHOD_NOT_SUPPORTED;
            

        case GATEWAY_TIMEOUT : return SERVER_ERROR_PAGE_GATEWAY_TIMEOUT;
            

        case VERSION_NOT_SUPPORTED : return SERVER_ERROR_PAGE_VERSION_NOT_SUPPORTED;
            

        default: return SERVER_ERROR_PAGE_BAD_REQUEST;    
    }
}

void Error::addSpecificHeader(ClientSocketStream& client, const int& err)
{
    switch (err)
    {
        case METHOD_NOT_ALLOWED: appendToResponse("Allow", getAllowedMethod(*(client.getServer() -> getInstance()), HttpServer::_httpMethods));
            break;
        
        default:
            break;
    }
}

int Error::firstStep(ClientSocketStream& client, HttpResponse& res, const int& err)
{
    std::map<short int, std::string>& _map = client.getServer() -> getInstance() -> getErrorMaps();
    std::map<short int, std::string>::iterator it = _map.find(client.getErrStatus());

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

    makeStatusLine(client, err);
    addSpecificHeader(client, err);
    std::cout << "second: " << it -> second << " Res: " << UtilityMethod::getMimeType(it -> second, "", "", false) << std::endl;
    appendToResponse(CONTENT_TYP, UtilityMethod::getMimeType(it -> second, "", "", false));
    appendToResponse(CONTENT_LEN, UtilityMethod::numberToString(res.getBodySize()));
    _response += CRLF;

    if (UtilityMethod::sendBuffer(client.getFd(), _response.c_str(), _response.size()) == IO::IO_ERROR) return (IO::IO_ERROR);
    
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

    err = IO::IO_SUCCESS;
    
    if (req.getMethod() == HttpServer::HTTP_SERVER_HEAD)
    {
        error_page.insert(error_page.find(CRLF) + 2, ("Allow: " + getAllowedMethod(*(client.getServer() -> getInstance()), HttpServer::_httpMethods) + CRLF));
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
