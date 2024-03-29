# include "../../includes/method/Head.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Head::Head(){};
Head::Head(const Head& rhs):Method(rhs){};
Head& Head::operator=(const Head& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Head::~Head(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int Head::sendResponse(ClientSocketStream& client, HttpRequest& req, HttpResponse& res)
{
    HttpServer& instance = *(client.getServer() -> getInstance());
    std::string full_path(req.getHeaders().find(FULLPATH) -> second);
    
    bool directory = req.checkBits(HttpRequest::HTTP_REQUEST_DIRECTORY);
    bool cgi_get = req.checkBits(HttpRequest::HTTP_REQUEST_CGI_);

    if (cgi_get == false && ((directory == true && instance.checkBits(HttpServer::HTTP_SERVER_AUTO_INDEX_) > 0) || directory == 0))
    {
        std::ifstream& file = res.getFile();

        file.open(full_path.c_str(), std::ifstream::in | std::ifstream::binary);

        if (!file) return FORBIDEN;
        
        file.seekg(0, std::ios::end);
        size_t len = file.tellg();
        file.seekg(0, std::ios::beg);


        std::string ressource(full_path);
        
        res.makeStatusLine(OK)
           .setHeader(CONTENT_TYP, UtilityMethod::getMimeType(ressource, instance.getFullIndexPath(), instance.getIndex(), true))
           .setHeader(CONTENT_LEN, UtilityMethod::numberToString(len))
           .addEndHeaderCRLF();
    }
    else if (directory == true || cgi_get == true)
    {
        
        if (directory)
        {
            if (access(PATH_TO_DIRECTORY_LISTING_SCRIPT, F_OK) != 0) return NOT_FOUND;

            if ((access(PATH_TO_DIRECTORY_LISTING_SCRIPT, X_OK | R_OK) != 0)) return FORBIDEN;
        }
        else
        {
            const char *executable = req.getHeaders().find(CGI_EXECUTABLE) -> second.c_str();

            if (access(executable, F_OK) != 0) return NOT_FOUND;

            if ((access(executable, X_OK) != 0)) return FORBIDEN;
        }

        res.makeStatusLine(OK)
           .setHeader(CONTENT_TYP, "application/octet-stream")
           .setHeader(CONTENT_LEN, UtilityMethod::numberToString(0))
           .addEndHeaderCRLF();
    }
    
    if (res.sendResponse() == IO::IO_ERROR) return (IO::IO_ERROR);

    res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    
    return IO::IO_ERROR;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Head::clone(void) const {return new Head; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
