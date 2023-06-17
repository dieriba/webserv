# include "../../includes/method/Method.hpp"
# include "../../includes/server/HttpServer.hpp"
# include "../../includes/method/Get.hpp"
# include "../../includes/method/Post.hpp"
# include "../../includes/method/Delete.hpp"
# include "../../includes/method/Error.hpp"
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Method::Method(){};
Method::Method(const Method& rhs){(void)rhs;};
Method& Method::operator=(const Method& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Method::~Method(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
std::string& Method::getResponse(void) {return _response;}
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

int Method::handleFileRessource(IO& event, HttpResponse& res)
{
    try
    {
        char buffer[REQUEST_SIZE];

        std::ifstream& file = res.getFile();
        
        file.read(buffer, REQUEST_SIZE);

        if (UtilityMethod::sendBuffer(event.getFd(), buffer, file.gcount()) == IO::IO_ERROR) return (IO::IO_ERROR);

        if (file.fail() || file.eof()) res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    catch(const std::exception& e)
    {
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}

int Method::sendRedirect(const IO& event, HttpResponse& res, const char *status_line)
{
    if (UtilityMethod::sendBuffer(event.getFd(), status_line, UtilityMethod::myStrlen(status_line)) == IO::IO_ERROR)
            return IO::IO_ERROR;
    
    const std::string& redirect_url = event.getServer() -> getInstance() -> getRedirect();

    if (UtilityMethod::sendBuffer(event.getFd(), redirect_url.c_str(), redirect_url.size()) || UtilityMethod::sendBuffer(event.getFd(), CRLF CRLF, UtilityMethod::myStrlen(CRLF CRLF)))
        return IO::IO_ERROR;
            
    res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    return IO::IO_SUCCESS;
}

void Method::setCookieHeader(IO& object)
{
    static int counter = 0;

    std::time_t currentTime = std::time(NULL);
    std::string uniqueCookie = UtilityMethod::numberToString(currentTime) + "_" + UtilityMethod::numberToString(counter++);
    
    uniqueCookie = WELCOME_COOKIES + uniqueCookie + "; PATH=/;";
    appendToResponse(SET_COOKIE, uniqueCookie);
    object.setOptions(IO::COOKIE, SET);
}

void Method::makeStatusLine(IO& object, const int& status)
{
    std::string version(HTTP_VERSION);
    std::ostringstream ss;

    ss << status;
    std::string code(ss.str());
    
    _response = version + " " + code + " " + HttpServer::getHttpResponse(status) -> second + CRLF;

    if (object.checkBits(IO::COOKIE) == 0) setCookieHeader(object);
}

void Method::addEndHeaderCRLF(void)
{
    _response += CRLF;
}

void Method::appendToResponse(const std::string& key, const std::string& value)
{
    _response += key + ": " + value + CRLF;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
Method* Method::createGet() {return new Get; }
Method* Method::createPost() {return new Post; }
Method* Method::createDelete() {return new Delete; }
Method* Method::createError() {return new Error; }
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
Method::Factory Method::_tab[4] = {0};