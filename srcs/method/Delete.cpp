# include "../../includes/method/Delete.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
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
int Delete::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    (void)event;
    (void)req;
    (void)res;

    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
