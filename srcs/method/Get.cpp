# include "../../includes/method/Get.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/utilityMethod.hpp"
# include "../../includes/TcpServer.hpp"

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
int Get::handleFileRessource(TcpServer& instance, IO& event, const HttpRequest& req, std::string& ressource)
{
    std::ifstream file;
    std::stringstream buffer;

    if (req.getHeaders().find(PATH) -> second == instance.getIndexPath())
        ressource = instance.getRootDir() + "/" + instance.getIndex();
    
    file.open(ressource.c_str(), std::ifstream::in | std::ifstream::binary);
    
    if (!file) return (event.getReponse().getErrorMethod().sendResponse(event, req), 1);

    buffer << file.rdbuf();
    
    appendToResponse(CONTENT_TYP, utilityMethod::getMimeType(ressource, instance.getRootDir() + "/" + instance.getIndexPath(), instance.getIndex()));
    appendToResponse(CONTENT_LEN, utilityMethod::numberToString(buffer.str().size()));
    _response += CRLF;
    _response += buffer.str();

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
    std::string path(req.getHeaders().find(PATH) -> second);
    std::string ressource(instance.getRootDir() + req.getHeaders().find(PATH) -> second);
    DIR *directory = NULL;

    if (path != "/") directory = opendir(ressource.c_str());
    
    makeStatusLine(OK);
    
    if ((path == instance.getIndexPath() || (directory == NULL && (errno == ENOENT || errno == ENOTDIR)))
        && handleFileRessource(instance, event, req, ressource))
        return ;
    else if (directory)
        handleDirectoryRessource(event, directory);

    send(event.getFd(), _response.data(), _response.size(), 0);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
