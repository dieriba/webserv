# include "../../includes/method/Delete.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

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
int Delete::sendResponse(ClientSocketStream& event, HttpRequest& req, HttpResponse& res)
{
    if (res.checkBits(HttpResponse::HTTP_RESPONSE_REDIRECT_SET)) return res.sendRedirect(event, res, FOUND_REDIRECT_IND_METHOD);

    const std::string& full_path(req.getHeaders().find(FULLPATH) -> second);

    res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);

    if (std::remove(full_path.c_str()) != 0)
    {
        if (res.sendResponse(SERVER_ERROR_PAGE_FORBIDDEN) == IO::IO_ERROR)
            return IO::IO_ERROR;
    }
    else if (res.sendResponse(SERVER_SUCCESS_DELETE_RESPONSE) == IO::IO_ERROR)
            return IO::IO_ERROR;
    
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Delete::clone(void) const {return new Delete; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
