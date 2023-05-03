# include "../../includes/method/Get.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Get::Get(){};
Get::Get(const Get& rhs):Method(rhs){};
Get& Get::operator=(const Get& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Get::~Get(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void Get::sendResponse(const IO& event, const HttpRequest& req)
{
    TcpServer& instance = *(event.getServer() -> getInstance());
    std::string ressource(instance.getRootDir() + req.getHeaders().find(PATH) -> second);
    std::cout << ressource << std::endl;
    exit(1);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
