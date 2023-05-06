# include "../../includes/method/Method.hpp"
# include "../../includes/TcpServer.hpp"
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

int Method::sendBuffer(int client_socket, const char *buffer, int bytes)
{
    while (bytes > 0)
    {
        int bytesSent = send(client_socket, buffer, bytes, 0);
        if (bytesSent <= 0) return IO::IO_ERROR;

        bytes -= bytesSent;

        buffer += bytesSent;
    }
    return IO::IO_SUCCESS;
}

void Method::makeStatusLine(const int& status)
{
    std::string version(HTTP_VERSION);
    std::ostringstream ss;

    ss << status;
    std::string code(ss.str());
    
    _response = version + " " + code + " " + TcpServer::getHttpResponse(status) -> second + CRLF;
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