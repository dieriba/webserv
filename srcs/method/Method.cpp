# include "../../includes/method/Method.hpp"
# include "../../includes/server/HttpServer.hpp"
# include "../../includes/method/Head.hpp"
# include "../../includes/method/Get.hpp"
# include "../../includes/method/Post.hpp"
# include "../../includes/method/Put.hpp"
# include "../../includes/method/Delete.hpp"
# include "../../includes/method/Options.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

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

std::string Method::getAllowedMethod(const HttpServer& instance, const std::map<std::string, short int>& _httpMethods)
{
    std::string allowed_method;
    std::map<std::string, short int>::const_iterator end = --_httpMethods.end();
    
    for (std::map<std::string, short int>::const_iterator it = _httpMethods.begin(); it != _httpMethods.end(); it++)
    {
        if (instance.checkBits(it -> second))
        {
            if (it != end)
                allowed_method += it -> first + ", ";
            else
                allowed_method += it -> first;
        }
    }

    return allowed_method;
}

int Method::handleFileRessource(ClientSocketStream& event, HttpResponse& res)
{
    try
    {
        char buffer[REQUEST_SIZE];

        std::ifstream& file = res.getFile();
        
        file.read(buffer, REQUEST_SIZE);

        if (UtilityMethod::sendBuffer(event.getFd(), buffer, file.gcount()) == IO::IO_ERROR) return (IO::IO_ERROR);

        if (file.fail() || file.eof()) res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    }
    catch(const std::exception& e)
    {
        res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    }
    
    return IO::IO_SUCCESS;
}

int Method::sendRedirect(const ClientSocketStream& client, HttpResponse& res, const char *status_line)
{
    if (UtilityMethod::sendBuffer(client.getFd(), status_line, UtilityMethod::myStrlen(status_line)) == IO::IO_ERROR)
            return IO::IO_ERROR;
    
    const std::string& redirect_url = client.getServer() -> getInstance() -> getRedirect();

    if (UtilityMethod::sendBuffer(client.getFd(), redirect_url.c_str(), redirect_url.size()) || UtilityMethod::sendBuffer(client.getFd(), CRLF CRLF, UtilityMethod::myStrlen(CRLF CRLF)))
        return IO::IO_ERROR;
            
    res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    return IO::IO_SUCCESS;
}

void Method::setCookieHeader(IO& object)
{
    static int counter = 0;

    std::time_t currentTime = std::time(NULL);
    std::string uniqueCookie = UtilityMethod::numberToString(currentTime) + "_" + UtilityMethod::numberToString(counter++);
    
    uniqueCookie = WELCOME_COOKIES + uniqueCookie + "; PATH=/;";
    appendToResponse(SET_COOKIE, uniqueCookie);
    object.setOptions(IO::IO_COOKIE, SET);
}

void Method::makeStatusLine(IO& object, const int& status)
{
    std::string version(HTTP_VERSION);
    std::ostringstream ss;
    
    ss << status;
    std::string code(ss.str());
    
    _response = version + " " + code + " " + HttpServer::getHttpResponse(status) -> second + CRLF SERVER_NAME + UtilityMethod::getDateAndTime() + CRLF;

    if (object.checkBits(IO::IO_COOKIE) == 0) setCookieHeader(object);
}

void Method::addEndHeaderCRLF(void)
{
    _response += CRLF;
}

void Method::appendToResponse(const std::string& key, const std::string& value)
{
    _response += key + ": " + value + CRLF;
}

void Method::addCustomHeader(const HttpServer& instance)
{
    if (instance.checkBits(HttpServer::HTTP_SERVER_CUSTOM_HEADER) == 0)
        return ;

    const std::map<std::string, std::string>& map = instance.getHeadersMap();
    std::map<std::string, std::string>::const_iterator it = map.begin();
    for (; it != map.end(); it++)
        appendToResponse(it -> first, it -> second);
} 

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
Method* Method::createHead() {return new Head; }
Method* Method::createGet() {return new Get; }
Method* Method::createPost() {return new Post; }
Method* Method::createPut() {return new Put; }
Method* Method::createOptions() {return new Options; }
Method* Method::createDelete() {return new Delete; }
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
std::vector<Method::Factory> Method::_tab;