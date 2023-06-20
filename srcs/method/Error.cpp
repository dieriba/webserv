# include "../../includes/method/Error.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/IO.hpp"

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

        case INTERNAL_SERVER_ERROR : res = SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR;
            break;

        case METHOD_NOT_SUPPORTED : res = SERVER_ERROR_PAGE_METHOD_NOT_SUPPORTED;
            break;

        case VERSION_NOT_SUPPORTED : res = SERVER_ERROR_PAGE_VERSION_NOT_SUPPORTED;
            break;
            
        case UNSUPPORTED_MEDIA_TYPE : res = SERVER_ERROR_PAGE_UNSUPPORTED_MEDIA_TYPE;
            break;

        default: res = SERVER_ERROR_PAGE_BAD_REQUEST;
            break;
    }
    
    return res;
}

int Error::firstStep(IO& event, HttpResponse& res, const int& err)
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

    res.setOptions(HttpResponse::STARTED, SET);
    res.setOptions(HttpResponse::FILE, SET);

    return IO::IO_SUCCESS;
}

int Error::sendResponse(IO& event, HttpRequest& /* req */, HttpResponse& res)
{    
    HttpServer& instance = *(event.getServer() -> getInstance());
    int err = 0;

    if (instance.checkBits(HttpServer::ERROR_PAGE_SET))
    {
        if (!res.checkBits(HttpResponse::STARTED)) err = firstStep(event, res, event.getErrStatus());

        if (err == IO::IO_SUCCESS && res.checkBits(HttpResponse::FILE)) return handleFileRessource(event, res);
    }
    
    std::string error_page = getErrorPage(event.getErrStatus());
    if (UtilityMethod::sendBuffer(event.getFd(), error_page.c_str(), error_page.size()) == IO::IO_ERROR) return IO::IO_ERROR;
    res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Error::clone(void) const {return new Error; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
