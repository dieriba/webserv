# include "../../includes/http/RequestChecker.hpp"
# include "../../includes/http/HttpRequest.hpp"
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

int RequestChecker::serverOrLocation(const std::vector<Location>& locations, Location& location, const HttpRequest& req)
{
    std::string path = req.getHeaders().find("PATH") -> second;

    for (size_t i = 0; i < locations.size(); i++)
    {
        if (path.size() == 1 && locations[i].getSubPath() == path)
        {
            location = locations[i];
            return i;
        }
        else if (path.find(locations[i].getSubPath()) != std::string::npos)
        {
            location = locations[i];
            return i;
        }
    }
    return 0;
}

int RequestChecker::checkAll(Server& server, const HttpRequest& req)
{
    int res = checkHeader(req);
    Location location;
    
    if (res) return res;

    int loc = serverOrLocation(server.getLocations(), location, req);

    if (loc) server.setOptions(LOCATION_BLOCK, SET);

    //std::cout << "I should look to serve data from: " << (loc == 1 ? "Location Block" : "Server Block") << std::endl;

    for (size_t i = 0; tab[i] != 0; i++)
    {
        res = tab[i](loc, server, location, req);
    
        if (res) return res;
    }
    return res;
}

int RequestChecker::checkHeader(const HttpRequest& req)
{
    (void)req;
    return 0;
}

int RequestChecker::checkValidPath(const int& _loc, const Server& server, const Location& location, const HttpRequest& req)
{
    std::string root;

    if (location.getRootDir().size() == 0 && server.getRootDir().size() == 0)
        return NOT_FOUND;

    if (_loc)
    {
        root = location.getRootDir();
        if (*(root.rbegin()) == '/')
            root.erase(root.end() - 1);
        root += req.getHeaders().find("PATH") -> second;
        if (access(root.c_str(), F_OK) != 0)
            return NOT_FOUND;    
    }
    else
    {
        root = server.getRootDir();
        if (*(root.rbegin()) == '/')
            root.erase(root.end() - 1);
        root += req.getHeaders().find("PATH") -> second;
        if (access(root.c_str(), F_OK) != 0)
            return NOT_FOUND;
    }
    return 0;
}

int RequestChecker::checkAllowedMethod(const int& _loc, const Server& server, const Location& location, const HttpRequest& req)
{
    if (_loc && location.checkBits(req.getMethod()) == 0)
    {
        return METHOD_NOT_ALLOWED;
    }
    else if (_loc == 0 && server.checkBits(req.getMethod()) == 0)
    {
        return METHOD_NOT_ALLOWED;
    }
    return 0;
}

int RequestChecker::checkBodySize(const int& _loc, const Server& server, const Location& location, const HttpRequest& req)
{
    if ((_loc && location.getBodySize() != std::string::npos) && req.getBodySize() > location.getBodySize())
        return TOO_LARGE_CONTENT;
    else if ((_loc == 0 && server.getBodySize() != std::string::npos) && req.getBodySize() > server.getBodySize())
        return TOO_LARGE_CONTENT;
    return 0;
}
/*----------------------------------------STATIC FUNCTION----------------------------------------*/

RequestChecker::Checker RequestChecker::tab[10] = {0};