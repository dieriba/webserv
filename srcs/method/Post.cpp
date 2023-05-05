# include "../../includes/method/Post.hpp"
# include "../../includes/http/HttpRequest.hpp"
<<<<<<< HEAD
# include "../../includes/http/HttpResponse.hpp"
=======
>>>>>>> origin/main
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
<<<<<<< HEAD
void Post::sendResponse(IO& event, const HttpRequest& req, HttpResponse& res)
{
    (void)event;
    (void)req;
    (void)res;
=======
void Post::sendResponse(IO& event, const HttpRequest& req)
{
    (void)event;
    (void)req;
>>>>>>> origin/main
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
