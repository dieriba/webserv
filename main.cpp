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

void    print_header_map(const HttpServer& instance)
{
    std::map<std::string, std::string>::const_iterator it = instance.getHeadersMap().begin();
    std::map<std::string, std::string>::const_iterator end = instance.getHeadersMap().end();
    for (; it != end; it++)
        std::cout << "Custom_header:  Key: " << it -> first << ", Value: " << it -> second << std::endl;
}

void    print_allowed_method(const HttpServer& instance)
{   
    for (std::map<std::string, short int>::const_iterator it = HttpServer::_httpMethods.begin(); it != HttpServer::_httpMethods.end(); it++)
    {
        std::cout << "Method: " << it -> first << (instance.checkBits(it -> second) > 0 ? " Enabled" : " Disabled") << std::endl;
    }
}

void    print_location_config(const Location& location)
{
    std::map<std::string, std::string> map(location.getCgiMap());
    std::map<std::string, std::string>::iterator m_it = map.begin();
    std::map<std::string, std::string>::iterator m_end = map.end();
    std::cout << std::endl << std::endl ;
    std::cout << "----------Location----------" << std::endl;
    print_allowed_method(location);
    std::cout << std::endl
        << "Client Max Body Size: " << location.getBodySize() << std::endl
        << "Index HTML: " << (location.getIndex().size() > 0 ? location.getIndex() : "No root index") << std::endl
        << "Sub Path: " << location.getIndexPath() << std::endl
        << "Root Directory: " << (location.getRootDir().size() > 0 ? location.getRootDir() : "No root directory") << std::endl
        << "Redirect: "  << (location.getRedirect().size() > 0 ? location.getRedirect() : "No redirect") << std::endl
        << "Upload Folder: " << ((location.getUploadsFilesFolder().size()) ? location.getUploadsFilesFolder() : "No uploads folder set") << std::endl
        << "AUTO INDEX: " << ((location.checkBits(HttpServer::HTTP_SERVER_AUTO_INDEX_)) > 0 ? "Auto Index ON" : "Auto Index OFF") << std::endl
        << "FILE_UPLOAD: " << ((location.checkBits(HttpServer::HTTP_SERVER_FILE_UPLOAD_)) > 0? "FILE_UPLOAD ON" : "FILE_UPLOAD OFF") << std::endl
        << "Server: " << location.getServer() << std::endl;
        std::cout << std::endl;
        for (; m_it != m_end; m_it++)
            std::cout << "cgi: " << m_it -> first << " " << m_it -> second << std::endl;
        const HttpServer& instance = location;
        print_map_error_page(instance);
        print_header_map(instance);
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
    print_allowed_method(server);
    std::cout
        << "IP: " << (server.getIp().size() > 0 ? server.getIp() : "NO IP SET") << std::endl
        << "Redirect: " << (server.getRedirect().size() > 0 ? server.getRedirect(): "No redirection") << std::endl
        << "PORT: " << server.getPort() << std::endl
        << "Client Max Body Size: " << server.getBodySize() << std::endl
        << "Root Directory: " << ((server.getRootDir().size()) > 0 ? server.getRootDir() : "No root directory") << std::endl
        << "Index HTML: " << ((server.getIndex().size()) > 0 ? server.getIndex() : "No index html") << std::endl
        << "Upload Folder: " << ((server.getUploadsFilesFolder().size()) ? server.getUploadsFilesFolder() : "No uploads folder set") << std::endl
        << "AUTO INDEX: " << ((server.checkBits(HttpServer::HTTP_SERVER_AUTO_INDEX_)) > 0 ? "Auto Index ON" : "Auto Index OFF") << std::endl
        << "FILE_UPLOAD: " << ((server.checkBits(HttpServer::HTTP_SERVER_FILE_UPLOAD_)) > 0? "FILE_UPLOAD ON" : "FILE_UPLOAD OFF") << std::endl;
        const HttpServer& instance = server;
        print_map_error_page(instance);
        print_header_map(instance);
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
    RequestChecker::tab.reserve(10);
    RequestChecker::tab.push_back(RequestChecker::checkHeader);
    RequestChecker::tab.push_back(RequestChecker::checkAllowedMethod);
    RequestChecker::tab.push_back(RequestChecker::checkBodySize);
    RequestChecker::tab.push_back(RequestChecker::checkGetHeadMethod);
    RequestChecker::tab.push_back(RequestChecker::checkPostPutMethod);
    RequestChecker::tab.push_back(RequestChecker::checkOptionsMethod);
    RequestChecker::tab.push_back(RequestChecker::checkDeleteMethod);
    HttpServer::initMimeTypes();
    HttpServer::initHttpResponses();
    HttpServer::initHttpMethods();
    HttpServer::initKnownDirectives();
    Method::_tab.reserve(10);
    Method::_tab.push_back(Method::createHead);
    Method::_tab.push_back(Method::createGet);
    Method::_tab.push_back(Method::createPost);
    Method::_tab.push_back(Method::createPut);
    Method::_tab.push_back(Method::createOptions);
    Method::_tab.push_back(Method::createDelete);
    HttpServer::number_of_methods = Method::_tab.size();
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