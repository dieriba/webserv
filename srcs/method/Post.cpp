# include "../../includes/method/Post.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Post::Post(){};
Post::Post(const Post& rhs):Method(rhs){};
Post& Post::operator=(const Post& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Post::~Post(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int Post::sendResponse(IO& event, const HttpRequest& req, HttpResponse& res)
{
    (void)req;
    makeStatusLine(CREATED);
    appendToResponse(CONTENT_LEN, UtilityMethod::numberToString(0));
    appendToResponse(CONTENT_TYP, "text/plain");
    _response += CRLF;
    if (sendBuffer(event.getFd(), _response.c_str(), _response.size()))
    {
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
        return IO::IO_ERROR;
    }
    res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
