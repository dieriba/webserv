# include "../../includes/method/Options.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Options::Options(){};
Options::Options(const Options& rhs):Method(rhs){};
Options& Options::operator=(const Options& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Options::~Options(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int Options::sendResponse(ClientSocketStream& client, HttpRequest& /* req */, HttpResponse& res)
{
    res.makeStatusLine(client, NO_CONTENT);
    
    HttpServer* instance = client.getServer() -> getInstance();
    
    res.setHeader("Allow", client.getAllowedMethod(*(instance), HttpServer::_httpMethods) + CRLF);
    
    if (res.sendResponse() == IO::IO_ERROR) return IO::IO_ERROR;

    res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);

    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Options::clone(void) const {return new Options; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
