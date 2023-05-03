# include "../../includes/method/Post.hpp"
# include "../../includes/http/HttpRequest.hpp"
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
void Post::sendResponse(IO& event, const HttpRequest& req)
{
    (void)event;
    (void)req;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
