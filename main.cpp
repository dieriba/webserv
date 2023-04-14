#include "./includes/CommonLib.hpp"
#include "./includes/TcpServer.hpp"
#include "./includes/Server.hpp"
#include "./includes/ExceptionThrower.hpp"
#include "./includes/Location.hpp"

void    print_location_config(const Location& location)
{
    std::cout
        << "METHOD GET: " << (location.checkBits(GET) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD POST: " << (location.checkBits(POST) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD DELETE: " << (location.checkBits(DELETE) > 0 ? "Enabled" : "Disabled") << std::endl << std::endl
        << "Client Max Body Size: " << location.getBodySize() << std::endl
        << "Index HTML: " << (location.getIndex().size() > 0 ? location.getIndex() : "No root directory") << std::endl
        << "Root Directory: " << (location.getRootDir().size() > 0 ? location.getRootDir() : "No root directory") << std::endl
        << "Index HTML: "  << (location.getRedirect().size() > 0 ? location.getRedirect() : "No root directory") << std::endl
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
    std::cout
        << "METHOD GET: " << (server.checkBits(GET) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD POST: " << (server.checkBits(POST) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD DELETE: " << (server.checkBits(DELETE) > 0 ? "Enabled" : "Disabled") << std::endl << std::endl
        << "PORT: " << server.getPort() << std::endl
        << "Client Max Body Size: " << server.getBodySize() << std::endl
        << "Root Directory: " << ((server.getRootDir().size()) > 0 ? server.getRootDir() : "No root directory") << std::endl
        << "Index HTML: " << ((server.getIndex().size()) > 0 ? server.getIndex() : "No index html") << std::endl;
        for(; serv_it_ != serv_it_end; serv_it_++)
            std::cout << "Server_name: " << *serv_it_ << std::endl;
        for(; loca_it_ != loca_it_end; loca_it_++)
        {
            print_location_config(*loca_it_);
            std::cout << "Next Location" << std::endl;
        }
}


int main (int argc, char **argv)
{
    try
    {
        TcpServer tcp_servers;
        TcpServer::initHttpResponses();
        tcp_servers.settingUpServer(argc > 1 ? argv[1] : NULL);
        std::vector<Server> tmp(tcp_servers.getServers());
        std::vector<Server>::iterator it = tmp.begin();
        std::vector<Server>::iterator end = tmp.end();
        for ( ; it != end ; it++)
        {
            print_server_config(*it);
            std::cout << "-----------NEXT_SERV-------------\n\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
}