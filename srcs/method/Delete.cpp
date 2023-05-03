# include "../../includes/method/Delete.hpp"
# include "../../includes/http/HttpRequest.hpp"
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
void Delete::sendResponse(const IO& event, const HttpRequest& req)
{
(   void)event;
    (void)req;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/