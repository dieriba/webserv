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
int Get::handleFileRessource(const IO& event, const HttpRequest& req, std::string& ressource)
{
    int status = event.getErrStatus();
    std::ifstream file;
    std::ostringstream ss;
    
    if (status == 0)
    {
        file.open(ressource.c_str(), std::ios::binary);

        status = !file ? FORBIDEN : OK;

        ressource.clear();
    }

    ss << status;
    std::string code(ss.str());
    ressource = req.getHeaders().find(VERSION) -> second + " "\
                + code + " " + TcpServer::getHttpResponse(status) -> second + CRLF;
    if ((event.getErrStatus()) || (event.getErrStatus() == 0 && !file)) return 1;
     
    return 0;
}

int Get::handleDirectoryRessource(const IO& event, DIR *directory)
{
    (void)event;
    (void)directory;
    return 0;
}

void Get::sendResponse(const IO& event, const HttpRequest& req)
{
    TcpServer& instance = *(event.getServer() -> getInstance());
    std::string ressource(instance.getRootDir() + req.getHeaders().find(PATH) -> second);
    DIR *directory = opendir(ressource.c_str());
    if (directory == NULL && (errno == ENOENT || errno == ENOTDIR))
    {
        handleFileRessource(event, req, ressource);
    }
    else
        handleDirectoryRessource(event, directory);
    std::cout << ressource;
    exit(1);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
