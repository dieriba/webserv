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

        case TOO_LARGE_CONTENT : res = SERVER_ERROR_TOO_LARGE_CONTENT;
            break;

        case INTERNAL_SERVER_ERROR : res = SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR;
            break;

        case METHOD_NOT_SUPPORTED : res = SERVER_ERROR_PAGE_METHOD_NOT_SUPPORTED;
            break;

        case VERSION_NOT_SUPPORTED : res = SERVER_ERROR_PAGE_VERSION_NOT_SUPPORTED;
            break;
            
        default: res = SERVER_ERROR_PAGE_BAD_REQUEST;
            break;
    }
    
    return res;
}

int Error::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    (void)req;
    std::string error_page = getErrorPage(event.getErrStatus());
    if (sendBuffer(event.getFd(), error_page.c_str(), error_page.size()))
        return IO::IO_ERROR;
    res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
