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
    if (res.checkBits(HttpResponse::REDIRECT_SET)) return sendRedirect(event, res, FOUND_REDIRECT_IND_METHOD);

    const std::string& full_path(req.getHeaders().find(FULLPATH) -> second);

    res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);

    if (std::remove(full_path.c_str()) != 0)
    {
        if (sendBuffer(event.getFd(), SERVER_ERROR_PAGE_FORBIDDEN, UtilityMethod::myStrlen(SERVER_ERROR_PAGE_FORBIDDEN)))
            return IO::IO_ERROR;
    }
    else if (sendBuffer(event.getFd(), SERVER_SUCCESS_DELETE_RESPONSE, UtilityMethod::myStrlen(SERVER_SUCCESS_DELETE_RESPONSE)))
            return IO::IO_ERROR;
    
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
