#include "./includes/CommonLib.hpp"
#include "./includes/TcpServer.hpp"
#include "./includes/Server.hpp"
#include "./includes/ExceptionThrower.hpp"
#include "./includes/Location.hpp"

void    print_location_config(const Location& location)
{
    std::cout << std::endl << std::endl ;
    std::cout << "----------Location----------";
    std::cout << std::endl << std::endl
        << "METHOD GET: " << (location.checkBits(GET) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD POST: " << (location.checkBits(POST) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD DELETE: " << (location.checkBits(DELETE) > 0 ? "Enabled" : "Disabled") << std::endl << std::endl
        << "Client Max Body Size: " << location.getBodySize() << std::endl
        << "Index HTML: " << (location.getIndex().size() > 0 ? location.getIndex() : "No root index") << std::endl
        << "Sub Path: " << location.getSubPath() << std::endl
        << "Root Directory: " << (location.getRootDir().size() > 0 ? location.getRootDir() : "No root directory") << std::endl
        << "Redirect: "  << (location.getRedirect().size() > 0 ? location.getRedirect() : "No redirect") << std::endl
        << "Server: " << location.getServer() << std::endl;
}

void    print_server_config(const Server server)
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
        << "METHOD GET: " << (server.checkBits(GET) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD POST: " << (server.checkBits(POST) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD DELETE: " << (server.checkBits(DELETE) > 0 ? "Enabled" : "Disabled") << std::endl << std::endl
        << "IP: " << (server.getIp().size() > 0 ? server.getIp() : "NO IP SET") << std::endl
        << "Redirect: " << (server.getRedirect().size() > 0 ? server.getRedirect(): "No redirection") << std::endl
        << "PORT: " << server.getPort() << std::endl
        << "Client Max Body Size: " << server.getBodySize() << std::endl
        << "Root Directory: " << ((server.getRootDir().size()) > 0 ? server.getRootDir() : "No root directory") << std::endl
        << "Index HTML: " << ((server.getIndex().size()) > 0 ? server.getIndex() : "No index html") << std::endl;
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

int main (int argc, char **argv)
{
    try
    {
        TcpServer tcp_servers;
        TcpServer::initMimeTypes();
        TcpServer::initHttpResponses();
        TcpServer::initHttpMethods();
        TcpServer::initKnownDirectives();
        tcp_servers.settingUpServer(argc > 1 ? argv[1] : NULL);
        tcp_servers.runningUpServer();
        tcp_servers.makeServerServe();
        /*std::vector<Server> tmp(tcp_servers.getServers());
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