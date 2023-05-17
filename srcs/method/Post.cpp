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
int Post::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    if (event.getEvents() & EPOLLIN)
    {
        (void)req;
        if (res.checkBits(HttpResponse::MULTIPART_DATA))
        {
            std::cout << req.getHeaders().find(BOUNDARY) -> second << std::endl;
            std::cout << req.getHeaders().find(END_BOUNDARY) -> second << std::endl;
        }
        exit(1);
    }
    else
    {
        if (sendBuffer(event.getFd(), SERVER_SUCCESS_POST_RESPONSE, strlen(SERVER_SUCCESS_POST_RESPONSE)))
        {
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
            return IO::IO_ERROR;
        }
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
