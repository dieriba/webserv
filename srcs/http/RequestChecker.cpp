# include "../../includes/http/RequestChecker.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/TcpServer.hpp"
# include "../../includes/Server.hpp"
# include "../../includes/Location.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
RequestChecker::RequestChecker(){};
RequestChecker::~RequestChecker(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------STATIC FUNCTION----------------------------------------*/

const TcpServer *RequestChecker::serverOrLocation(const Server& server, const HttpRequest& req)
{
    std::string path = req.getHeaders().find("PATH") -> second;
    const std::vector<Location>& locations = server.getLocations();

    for (size_t i = 0; i < locations.size(); i++)
    {
        if ((path.size() == 1 && locations[i].getIndexPath() == path) || path.find(locations[i].getIndexPath()) != std::string::npos)
            return &locations[i];
    }

    return &server;
}

int RequestChecker::checkAll(Server& server, const HttpRequest& req)
{
    int res = checkHeader(req);
    
    if (res) return res;

    const TcpServer *instance = serverOrLocation(server, req);
    
    server.setInstance((TcpServer *)instance);
    //if (loc) server.setOptions(LOCATION_BLOCK, SET);

    //std::cout << "I should look to serve data from: " << (loc == 1 ? "Location Block" : "Server Block") << std::endl;

    for (size_t i = 0; tab[i] != 0; i++)
    {
        res = tab[i](instance, req);
    
        if (res) return res;
    }
    return res;
}

int RequestChecker::checkHeader(const HttpRequest& req)
{
    (void)req;
    return 0;
}

int RequestChecker::checkValidPath(const TcpServer *instance, const HttpRequest& req)
{
    std::string root;
    if (instance -> getRootDir().size() == 0) return NOT_FOUND;

    root = instance -> getRootDir();
    root += req.getHeaders().find("PATH") -> second;
    const char *root_c = root.c_str();
    
    if (req.getMethod() != POST)
    {
        if (access(root_c, F_OK) != 0)
            return NOT_FOUND;
        
        if (req.getMethod() == GET && (access(root_c, R_OK) != 0))
            return FORBIDEN;
    }
    
    if (req.getMethod() == POST && (access(root_c, W_OK) != 0))
        return FORBIDEN;

    return 0;
}

int RequestChecker::checkAllowedMethod(const TcpServer *instance, const HttpRequest& req)
{
    if (instance -> checkBits(req.getMethod()) == 0) return METHOD_NOT_ALLOWED;
    
    return 0;
}

int RequestChecker::checkBodySize(const TcpServer *instance, const HttpRequest& req)
{
    if ((instance -> getBodySize() != std::string::npos) && (req.getBodySize() > instance -> getBodySize()))
        return TOO_LARGE_CONTENT;
    
    return 0;
}
/*----------------------------------------STATIC FUNCTION----------------------------------------*/

RequestChecker::Checker RequestChecker::tab[10] = {0};