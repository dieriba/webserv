# include "../../includes/method/Error.hpp"
# include "../../includes/http/HttpRequest.hpp"
<<<<<<< HEAD
# include "../../includes/http/HttpResponse.hpp"
=======
>>>>>>> origin/main
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
<<<<<<< HEAD
void Error::sendResponse(IO& event, const HttpRequest& req, HttpResponse& res)
{
    (void)event;
    (void)req;
    (void)res;
    makeStatusLine(event.getErrStatus());
    appendToResponse(CONTENT_LEN, utilityMethod::numberToString(0));
    appendToResponse(CONTENT_TYP, "text/plain");
    _response += CRLF;
    std::cout << _response;
    send(event.getFd(), _response.data(), _response.size(), 0);
=======
void Error::sendResponse(IO& event, const HttpRequest& req)
{
    (void)event;
    (void)req;
    makeStatusLine(event.getErrStatus());
    std::cout << _response;
    exit(1);
>>>>>>> origin/main
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
