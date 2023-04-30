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

int RequestChecker::serverOrLocation(const Server& server, const std::vector<Location>& location, const HttpRequest& req)
{
    (void) server;
    std::string path = req.getHeaders().find("PATH") -> second;
    
    for (size_t i = 0; i < location.size(); i++)
    {
        std::cout << location[i].getSubPath() << std::endl;
        std::cout << location[i].getRootDir() << std::endl;
    }

    return 0;
}

int RequestChecker::checkAll(const Server& server, const HttpRequest& req)
{
    int res = checkHeader(req);
    
    if (res) return res;

    int loc = serverOrLocation(server, server.getLocations(), req);

    for (size_t i = 0; tab[i] != 0; i++)
    {
        res = tab[i](loc, server, req);
    
        if (res) return res;
    }
    return res;
}

int RequestChecker::checkHeader(const HttpRequest& req)
{
    (void)req;
    return 0;
}

int RequestChecker::checkValidPath(const int& _loc, const Server& server, const HttpRequest& req)
{
    (void)_loc;
    (void)server;
    (void)req;
    return 0;
}

int RequestChecker::checkAllowedMethod(const int& _loc, const Server& server, const HttpRequest& req)
{
    (void)_loc;
    if (server.checkBits(req.getMethod()) == 0)
        return METHOD_NOT_ALLOWED;
    return 0;
}

int RequestChecker::checkBodySize(const int& _loc, const Server& server, const HttpRequest& req)
{
    (void)_loc;
    (void)server;
    (void)req;
    return 0;
}
/*----------------------------------------STATIC FUNCTION----------------------------------------*/

RequestChecker::Checker RequestChecker::tab[10] = {0};