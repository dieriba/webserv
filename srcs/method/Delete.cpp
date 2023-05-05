# include "../../includes/method/Delete.hpp"
# include "../../includes/http/HttpRequest.hpp"
<<<<<<< HEAD
# include "../../includes/http/HttpResponse.hpp"
=======
>>>>>>> origin/main
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Delete::Delete(){};
Delete::Delete(const Delete& rhs):Method(rhs){};
Delete& Delete::operator=(const Delete& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Delete::~Delete(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
<<<<<<< HEAD
void Delete::sendResponse(IO& event, const HttpRequest& req, HttpResponse& res)
{
    (void)event;
    (void)req;
    (void)res;
=======
void Delete::sendResponse(IO& event, const HttpRequest& req)
{
(   void)event;
    (void)req;
>>>>>>> origin/main
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
