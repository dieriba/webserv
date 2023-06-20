# include "./includes/utils/CommonLib.hpp"
# include "./includes/server/HttpServer.hpp"
# include "./includes/server/Server.hpp"
# include "./includes/utils/ExceptionThrower.hpp"
# include "./includes/server/Location.hpp"
# include "./includes/http/RequestChecker.hpp"
# include "./includes/method/Method.hpp"

void    print_map_error_page(const HttpServer& instance)
{
    std::map<short int, std::string>::const_iterator it = instance.getErrorMaps().begin();
    std::map<short int, std::string>::const_iterator end = instance.getErrorMaps().end();

    for (; it != end; it++)
    {
        std::cout << "Error: " << it -> first << ", error_page_location: " << it -> second << std::endl;
    }
    
}

void    print_location_config(const Location& location)
{
    std::map<std::string, std::string> map(location.getCgiMap());
    std::map<std::string, std::string>::iterator m_it = map.begin();
    std::map<std::string, std::string>::iterator m_end = map.end();
    std::cout << std::endl << std::endl ;
    std::cout << "----------Location----------";
    std::cout << std::endl << std::endl
        << "METHOD GET: " << (location.checkBits(HttpServer::GET) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD POST: " << (location.checkBits(HttpServer::POST) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD DELETE: " << (location.checkBits(HttpServer::DELETE) > 0 ? "Enabled" : "Disabled") << std::endl << std::endl
        << "Client Max Body Size: " << location.getBodySize() << std::endl
        << "Index HTML: " << (location.getIndex().size() > 0 ? location.getIndex() : "No root index") << std::endl
        << "Sub Path: " << location.getIndexPath() << std::endl
        << "Root Directory: " << (location.getRootDir().size() > 0 ? location.getRootDir() : "No root directory") << std::endl
        << "Redirect: "  << (location.getRedirect().size() > 0 ? location.getRedirect() : "No redirect") << std::endl
        << "Upload Folder: " << ((location.getUploadsFilesFolder().size()) ? location.getUploadsFilesFolder() : "No uploads folder set") << std::endl
        << "AUTO INDEX: " << ((location.checkBits(HttpServer::AUTO_INDEX_)) > 0 ? "Auto Index ON" : "Auto Index OFF") << std::endl
        << "FILE_UPLOAD: " << ((location.checkBits(HttpServer::FILE_UPLOAD_)) > 0? "FILE_UPLOAD ON" : "FILE_UPLOAD OFF") << std::endl
        << "Server: " << location.getServer() << std::endl;
        std::cout << std::endl;
        for (; m_it != m_end; m_it++)
            std::cout << "cgi: " << m_it -> first << " " << m_it -> second << std::endl;
        const HttpServer& instance = location;
    print_map_error_page(instance);
}

void    print_server_config(const Server& server)
{
    std::vector<std::string> serv = server.getServerNames();
    std::vector<std::string>::iterator serv_it_ = serv.begin();
    std::vector<std::string>::iterator serv_it_end = serv.end();
    std::vector<Location> loca(server.getLocations());
    std::vector<Location>::iterator loca_it_ = loca.begin();
    std::vector<Location>::iterator loca_it_end = loca.end();
    std::map<std::string, std::string> map(server.getCgiMap());
    std::map<std::string, std::string>::iterator m_it = map.begin();
    std::map<std::string, std::string>::iterator m_end = map.end();
    std::cout
        << "METHOD GET: " << (server.checkBits(HttpServer::GET) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD POST: " << (server.checkBits(HttpServer::POST) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD DELETE: " << (server.checkBits(HttpServer::DELETE) > 0 ? "Enabled" : "Disabled") << std::endl << std::endl
        << "IP: " << (server.getIp().size() > 0 ? server.getIp() : "NO IP SET") << std::endl
        << "Redirect: " << (server.getRedirect().size() > 0 ? server.getRedirect(): "No redirection") << std::endl
        << "PORT: " << server.getPort() << std::endl
        << "Client Max Body Size: " << server.getBodySize() << std::endl
        << "Root Directory: " << ((server.getRootDir().size()) > 0 ? server.getRootDir() : "No root directory") << std::endl
        << "Index HTML: " << ((server.getIndex().size()) > 0 ? server.getIndex() : "No index html") << std::endl
        << "Upload Folder: " << ((server.getUploadsFilesFolder().size()) ? server.getUploadsFilesFolder() : "No uploads folder set") << std::endl
        << "AUTO INDEX: " << ((server.checkBits(HttpServer::AUTO_INDEX_)) > 0 ? "Auto Index ON" : "Auto Index OFF") << std::endl
        << "FILE_UPLOAD: " << ((server.checkBits(HttpServer::FILE_UPLOAD_)) > 0? "FILE_UPLOAD ON" : "FILE_UPLOAD OFF") << std::endl;
        const HttpServer& instance = server;
        print_map_error_page(instance);
        if (serv_it_ != serv_it_end)
        {
            std::cout << "Server_name: ";
            for(; serv_it_ != serv_it_end; serv_it_++)
                std::cout << *serv_it_ << " ";
        }
        std::cout << std::endl;
        for (; m_it != m_end; m_it++)
            std::cout << "cgi: " << m_it -> first << " " << m_it -> second << std::endl;
        for(; loca_it_ != loca_it_end; loca_it_++)
        {
            print_location_config(*loca_it_);
            if (loca_it_ + 1 != loca_it_end)
                std::cout << "--------Next Location----\n" << std::endl;
        }
}

void    init_static_data(void)
{
    RequestChecker::tab[0] = RequestChecker::checkHeader;
    RequestChecker::tab[1] = RequestChecker::checkGetMethod;
    RequestChecker::tab[2] = RequestChecker::checkPostMethod;
    RequestChecker::tab[3] = RequestChecker::checkDeleteMethod;
    RequestChecker::tab[4] = RequestChecker::checkAllowedMethod;
    RequestChecker::tab[5] = RequestChecker::checkBodySize;
    HttpServer::initMimeTypes();
    HttpServer::initHttpResponses();
    HttpServer::initHttpMethods();
    HttpServer::initKnownDirectives();
    Method::_tab[0] = Method::createGet;
    Method::_tab[1] = Method::createPost;
    Method::_tab[2] = Method::createDelete;
    Method::_tab[3] = Method::createError;
}

int main (int argc, char **argv)
{
    try
    {
        signal(SIGINT, HttpServer::switch_off_signal);
        signal(SIGPIPE, SIG_IGN);
        init_static_data();
        HttpServer http_server;
        http_server.settingUpServer(argc > 1 ? argv[1] : NULL);
        http_server.runningUpServer();
        http_server.makeServerServe();
        /*std::vector<Server> tmp(http_server.getServers());
        std::vector<Server>::iterator it = tmp.begin();
        std::vector<Server>::iterator end = tmp.end();
        for ( ; it != end ; it++)
        {
            std::cout << "-----------SERVER-------------\n\n";
            print_server_config(*it);
        }*/
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}