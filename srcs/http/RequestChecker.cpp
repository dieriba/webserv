# include "../../includes/http/RequestChecker.hpp"
# include "../../includes/server/HttpServer.hpp"
# include "../../includes/server/Server.hpp"
# include "../../includes/server/Location.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------STATIC FUNCTION----------------------------------------*/

const HttpServer *RequestChecker::serverOrLocation(const Server& server, const HttpRequest& req)
{
    std::string path = req.getHeaders().find(PATH) -> second;
    const std::vector<Location>& locations = server.getLocations();

    for (size_t i = 0; i < locations.size(); i++)
    {
        if ((path.size() == 1 && locations[i].getIndexPath() == path) || path.find(locations[i].getIndexPath()) == 0)
            return &locations[i];
    }

    return &server;
}

int RequestChecker::checkAll(ClientSocketStream& client, HttpRequest& req)
{
    Server& server = *(client.getServer());
    
    const HttpServer *instance = server.getInstance();

    int _res = 0;
    
    for (size_t i = 0; tab[i] != 0; i++)
    {
        _res = tab[i](*(instance), req);

        if (_res) return _res;
    }

    return _res;
}

int RequestChecker::checkDeleteMethod(const HttpServer& instance, HttpRequest& req)
{
    if (req.getMethod() != HttpServer::HTTP_SERVER_DELETE) return IO::IO_SUCCESS;

    const std::string& full_path(req.getHeaders()[FULLPATH]);
    const std::map<std::string, std::string>& _map = req.getHeaders();
    
    std::string dir_path = instance.getRootDir() + req.getHeaders()[PATH];

    size_t i = full_path.rfind('/');

    const char *root_c = full_path.c_str();
    char *alias_root = (char *)root_c;
    char stop = root_c[i + 1];

    if ((_map.find(CONTENT_LEN) != _map.end()) || (_map.find(TRANSFERT_ENCODING) != _map.end())) return BAD_REQUEST;

    alias_root[i + 1] = 0;

    if (req.checkBits(HttpRequest::HTTP_REQUEST_DIRECTORY) || access(alias_root, W_OK | X_OK) != 0) return FORBIDEN;

    alias_root[i + 1] = stop;

    if (access(root_c, F_OK) != 0) return NOT_FOUND;
    
    return IO::IO_SUCCESS;
}

int RequestChecker::checkPostPutMethod(const HttpServer& instance, HttpRequest& req)
{
    if (req.getMethod() != HttpServer::HTTP_SERVER_POST && req.getMethod() != HttpServer::HTTP_SERVER_PUT) return IO::IO_SUCCESS;

    if (instance.checkBits(HttpServer::HTTP_SERVER_FILE_UPLOAD_) == false) return METHOD_NOT_ALLOWED;

    std::map<std::string, std::string>& _map = req.getHeaders();

    std::map<std::string, std::string>::iterator it = _map.find(CONTENT_TYP);
    std::map<std::string, std::string>::iterator it_length = _map.find(CONTENT_LEN);
    std::map<std::string, std::string>::iterator it_transfer = _map.find(TRANSFERT_ENCODING);

    if ((((it_length == _map.end()) && (it_transfer == _map.end())) || (it_length != _map.end() && it_transfer != _map.end()))
        || it == _map.end()) 
        return BAD_REQUEST;
        
    if (req.checkBits(HttpRequest::HTTP_REQUEST_CGI_)) return IO::IO_SUCCESS;

    const std::string& full_path(req.getHeaders()[FULLPATH]);

    size_t i = full_path.rfind('/');

    const char *root_c = full_path.c_str();
    char *alias_root = (char *)root_c;
    char stop = root_c[i + 1];

    if (req.getHeaders().find(PATH) -> second != instance.getIndexPath()) alias_root[i + 1] = 0;
 
    if (UtilityMethod::is_a_directory(alias_root) == 0)
    {
        if (errno == EACCES) return FORBIDEN;

        return NOT_FOUND;
    }
        
    alias_root[i + 1] = stop;

    if (access(alias_root, W_OK) && errno == EACCES) return FORBIDEN;
    
    size_t len = UtilityMethod::myStrlen(MULTIPART_FORM_DATA"; boundary=");

    if (it -> second.compare(0, len, MULTIPART_FORM_DATA"; boundary=") == 0)
    {
        if (it_transfer != _map.end()) return BAD_REQUEST;

        int count = 0;
                
        for (size_t i = len; it -> second[i]; i++)
        {
            if (it -> second[i] == '-')
                count++;
            else
                break ;
        }

        if (count <= 2) return BAD_REQUEST; 

        req.setBoundary(DOUBLE_HIPHEN + it -> second.erase(0, len));
        req.setEndBoundary(DOUBLE_HIPHEN + it -> second + DOUBLE_HIPHEN);
        req.setCrlfBoundary(CRLF + req.getBoundary());
        req.setCrlfEndBoundary(CRLF + req.getEndBoundary());
        _map[BOUNDARY] = req.getBoundary();
        _map[END_BOUNDARY] = req.getEndBoundary();
        _map[CRLF_BOUNDARY] = req.getCrlfBoundary();
        _map[CRLF_END_BOUNDARY] = req.getCrlfEndBoundary();
        req.setOptions(HttpRequest::HTTP_REQUEST_MULTIPART_DATA, SET);       
    }
    else
    {
        std::string& path(_map.find(PATH) -> second);
        std::string _pathMimeType = UtilityMethod::getMimeType(path, "", "", false);

        if (path.size() > 2 && *(path.rbegin()) == '/') path.erase(path.size() - 1);

        if (path != instance.getIndexPath() && _pathMimeType != it -> second) return BAD_REQUEST;

        req.setOptions(HttpRequest::HTTP_REQUEST_NO_ENCODING, SET);
    }
    
    return IO::IO_SUCCESS;
}

int RequestChecker::checkGetHeadMethod(const HttpServer& instance, HttpRequest& req)
{
    if (req.getMethod() != HttpServer::HTTP_SERVER_GET && req.getMethod() != HttpServer::HTTP_SERVER_HEAD) return IO::IO_SUCCESS;

    std::map<std::string, std::string>& _map = req.getHeaders();

    if ((_map.find(CONTENT_LEN) != _map.end()) || (_map.find(TRANSFERT_ENCODING) != _map.end())) return BAD_REQUEST;

    if (req.checkBits(HttpRequest::HTTP_REQUEST_CGI_)) return IO::IO_SUCCESS;

    std::string full_path(req.getHeaders()[FULLPATH]);

    std::string dir_path = instance.getRootDir() + req.getHeaders()[PATH];

    const char *root_c = full_path.c_str();

    if (access(root_c, F_OK) != 0)
    {
        if (errno == EACCES) return FORBIDEN;

        return NOT_FOUND;
    };

    if ((access(root_c, R_OK) != 0)) return FORBIDEN;

    return IO::IO_SUCCESS;
}

int RequestChecker::checkHeader(const HttpServer& instance, HttpRequest& req)
{
    std::map<std::string, std::string> _map = req.getHeaders();
    
    if (instance.getRootDir().size() == 0) return NOT_FOUND;

    if (_map[VERSION] != HTTP_VERSION) return VERSION_NOT_SUPPORTED;
    
    std::string dir_path = instance.getRootDir() + req.getHeaders()[PATH];

    if (UtilityMethod::is_a_directory(dir_path.c_str())) req.setOptions(HttpRequest::HTTP_REQUEST_DIRECTORY, SET);

    if (req.getMethod() != HttpServer::HTTP_SERVER_DELETE && req.checkBits(HttpRequest::HTTP_REQUEST_CGI_))
    {
        std::string full_path(req.getHeaders()[FULLPATH]);
        size_t i = full_path.find('?');

        if (i != std::string::npos) full_path = full_path.substr(0, i);
        const std::map<std::string, std::string>& cgi_map = instance.getCgiMap();
        const std::map<std::string, std::string>::const_iterator& it = cgi_map.find(UtilityMethod::getFileExtension(full_path, 1));

        std::cout << "Full path value: " << full_path << std::endl;

        if (access(it -> second.c_str() , F_OK) != 0 || access(full_path.c_str(), F_OK) != 0) return NOT_FOUND;

        if (access(it -> second.c_str() , X_OK) != 0 || access(full_path.c_str(), R_OK) != 0) return FORBIDEN;

        req.getHeaders()[CGI_EXECUTABLE] = it -> second;
        req.getHeaders()[CGI_ARGS] = full_path;
        req.setOptions(HttpRequest::HTTP_REQUEST_CGI_, SET);
    }

    return IO::IO_SUCCESS;
}

int RequestChecker::checkAllowedMethod(const HttpServer& instance, HttpRequest& req)
{
    if (instance.checkBits(req.getMethod()) == 0) return METHOD_NOT_ALLOWED;
    
    return IO::IO_SUCCESS;
}

int RequestChecker::checkBodySize(const HttpServer& instance, HttpRequest& req)
{
    if ((instance.getBodySize() != std::string::npos) && (req.getBodySize() > instance.getBodySize()))                                                                            
        return TOO_LARGE_CONTENT;

    return IO::IO_SUCCESS;
}
/*----------------------------------------STATIC FUNCTION----------------------------------------*/

RequestChecker::Checker RequestChecker::tab[10] = {0};