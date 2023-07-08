# include "../../includes/method/Put.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/IO/CgiStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Put::Put():FileWriter(){};
Put::Put(const Put& rhs):Method(rhs),FileWriter(rhs){};
Put& Put::operator=(const Put& rhs)
{
    if (this == &rhs) return *this;
    _request_body_size = rhs._request_body_size;
    return *this;
};
Put::~Put(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

int Put::sendResponse(ClientSocketStream& client, HttpRequest& req, HttpResponse& res)
{
    if (res.checkBits(HttpResponse::HTTP_RESPONSE_REDIRECT_SET)) return sendRedirect(client, res, FOUND_REDIRECT_POST);

    if (client.getEvents() & EPOLLIN)
    {
        if (req.checkBits(HttpRequest::HTTP_REQUEST_MULTIPART_DATA))
            return handleMultipartData(client, req);
        else
        {
            if (req.checkBits(HttpRequest::HTTP_REQUEST_CONTENT_LENGTH))
                return writeToFile(req);
            return fillChunkBody(req);
        }
    }
    else
    {
        if (UtilityMethod::sendBuffer(client.getFd(), SERVER_SUCCESS_POST_RESPONSE, UtilityMethod::myStrlen(SERVER_SUCCESS_POST_RESPONSE)) == IO::IO_ERROR)
        {
            res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
            return IO::IO_ERROR;
        }

        res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Put::clone(void) const {return new Put; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
