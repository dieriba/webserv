# include "../../includes/method/Method.hpp"
# include "../../includes/TcpServer.hpp"
# include "../../includes/method/Get.hpp"
# include "../../includes/method/Post.hpp"
# include "../../includes/method/Delete.hpp"
# include "../../includes/method/Error.hpp"
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
void Method::makeStatusLine(int status, const std::string& method)
{
    std::string version(HTTP_VERSION);
    
    if (status == 0) status = OK;
    
    _response = version + " " + method + " " + TcpServer::getHttpResponse(status) -> second + CRLF;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
Method* Method::createGet() {return new Get; }
Method* Method::createPost() {return new Post; }
Method* Method::createDelete() {return new Delete; }
Method* Method::createError() {return new Error; }
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
Method::Factory Method::_tab[4] = {0};