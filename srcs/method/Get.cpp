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
int Get::handleFileRessource(IO& event, const HttpRequest& req, std::string& ressource)
{
    (void)event;
    (void)req;
    std::ifstream file;
    file.open(ressource.c_str(), std::ios::binary);
    if (!file) return (event.getReponse().getErrorMethod().sendResponse(event, req), 1);
    ressource.clear();
    return 0;
}

int Get::handleDirectoryRessource(IO& event, DIR *directory)
{
    (void)event;
    (void)directory;
    return (0);
}

void Get::sendResponse(IO& event, const HttpRequest& req)
{
    TcpServer& instance = *(event.getServer() -> getInstance());
    std::string ressource(instance.getRootDir() + req.getHeaders().find(PATH) -> second);
    DIR *directory = opendir(ressource.c_str());
    makeStatusLine(OK);
    if (directory == NULL && (errno == ENOENT || errno == ENOTDIR))
        handleFileRessource(event, req, ressource);
    else
        handleDirectoryRessource(event, directory);
    std::cout << _response;
    exit(1);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
