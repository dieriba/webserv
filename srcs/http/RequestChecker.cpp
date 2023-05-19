# include "../../includes/http/RequestChecker.hpp"
# include "../../includes/server/TcpServer.hpp"
# include "../../includes/server/Server.hpp"
# include "../../includes/server/Location.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"

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

int RequestChecker::checkAll(IO& object, HttpRequest& req, HttpResponse& res)
{
    Server& server = *(object.getServer());
    
    int _res = checkHeader(req, res);
    
    if (_res) return _res;

    const TcpServer *instance = server.getInstance();
    
    //if (loc) server.setOptions(LOCATION_BLOCK, SET);

    //std::cout << "I should look to serve data from: " << (loc == 1 ? "Location Block" : "Server Block") << std::endl;

    for (size_t i = 0; tab[i] != 0; i++)
    {
        _res = tab[i](instance, req);
    
        if (_res) return _res;
    }

    

    return _res;
}

int RequestChecker::checkHeader(HttpRequest& req, HttpResponse& res)
{
    std::map<std::string, std::string>& _map = req.getHeaders();
    
    if (req.getMethod() == TcpServer::GET)
    {

    }
    else if (req.getMethod() == TcpServer::POST)
    {
        std::map<std::string, std::string>::iterator it = _map.find(CONTENT_TYP);

        if (((_map.find(CONTENT_LEN) == _map.end()) && (_map.find(TRANSFERT_ENCODING) == _map.end())) || it == _map.end())
            return BAD_REQUEST;

        size_t len = UtilityMethod::myStrlen(MULTIPART_FORM_DATA"; boundary=");
        
        if (it -> second.compare(0, len, MULTIPART_FORM_DATA"; boundary=") == 0)
        {
            int count = 0;
            
            for (size_t i = len; it -> second[i]; i++)
            {
                if (it -> second[i] == '-')
                    count++;
                else
                    break ;
            }

            if (count <= 2) return BAD_REQUEST; 
            
            _map[BOUNDARY] = it -> second.erase(0, len);
            _map[END_BOUNDARY] = it -> second + "--";

            res.setOptions(HttpResponse::MULTIPART_DATA, SET);       
        }
        else
        {
            std::string& path(_map.find(PATH) -> second);
            std::string _pathMimeType = UtilityMethod::getMimeType(path, "", "", false);

            if (path != SLASH && _pathMimeType != it -> second) return BAD_REQUEST;

            res.setOptions(HttpResponse::NO_ENCODING, SET);       
        }
    }

    return IO::IO_SUCCESS;
}

int RequestChecker::checkValidPath(const TcpServer *instance, const HttpRequest& req)
{
    std::string root;
 
    if (instance -> getRootDir().size() == 0) return NOT_FOUND;

    root = instance -> getRootDir();
    root += req.getHeaders().find("PATH") -> second;
    
    const char *root_c = root.c_str();
    
    if (req.getMethod() == TcpServer::POST)
        std::cout << "Root: " << root_c << std::endl;
    
    if (req.getMethod() != TcpServer::POST)
    {
        
        if (access(root_c, F_OK) != 0)
            return NOT_FOUND;
        
        if (req.getMethod() == TcpServer::GET && (access(root_c, R_OK) != 0))
            return FORBIDEN;

    }
    
    if (req.getMethod() == TcpServer::POST && (access(root_c, F_OK) == 0 && access(root_c, W_OK) != 0))
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

    std::cout << instance->getRootDir() << std::endl;

    if ((instance -> getBodySize() != std::string::npos) && (req.getBodySize() > instance -> getBodySize()))                                                                            
        return TOO_LARGE_CONTENT;

    return IO::IO_SUCCESS;
}
/*----------------------------------------STATIC FUNCTION----------------------------------------*/

RequestChecker::Checker RequestChecker::tab[10] = {0};