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
    if (!res.checkBits(HttpResponse::REDIRECT_SET))
    {

    }
    else
    {
        if (sendBuffer(event.getFd(), FOUND_REDIRECT_IND_METHOD, UtilityMethod::myStrlen(FOUND_REDIRECT_IND_METHOD)))
            return IO::IO_ERROR;
        const std::string& link = event.getServer() -> getInstance() -> getRedirect();
        
        if (sendBuffer(event.getFd(), link.c_str(), link.size()) || sendBuffer(event.getFd(), CRLF CRLF, UtilityMethod::myStrlen(CRLF CRLF)))
            return IO::IO_ERROR;
            
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    (void)req;
    (void)res;
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
