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
int Error::sendResponse(IO& event, const HttpRequest& req, HttpResponse& res)
{
    (void)req;
    makeStatusLine(event.getErrStatus());
    appendToResponse(CONTENT_LEN, UtilityMethod::numberToString(0));
    appendToResponse(CONTENT_TYP, "text/plain");
    _response += CRLF;
    std::cout << _response;
    if (sendBuffer(event.getFd(), _response.c_str(), _response.size()))
        return IO::IO_ERROR;
    res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
