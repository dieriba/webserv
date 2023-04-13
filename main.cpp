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
        << "Index HTML: "  << (location.getUri().size() > 0 ? location.getUri() : "No root directory") << std::endl
        << "Server: " << location.getServer() << std::endl;
}

void    print_server_config(const Server server)
{
    std::list<std::string>::iterator serv_it = server.getServerNames().begin();
    std::list<Location> loca(server.getLocations());
    std::list<Location>::iterator loca_it_ = loca.begin();
    std::list<Location>::iterator loca_it_end = loca.end();
    std::cout
        << "METHOD GET: " << (server.checkBits(GET) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD POST: " << (server.checkBits(POST) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD DELETE: " << (server.checkBits(DELETE) > 0 ? "Enabled" : "Disabled") << std::endl << std::endl
        << "PORT: " << server.getPort() << std::endl
        << "Client Max Body Size: " << server.getBodySize() << std::endl
        << "Root Directory: " << ((server.getRootDir().size()) > 0 ? server.getRootDir() : "No root directory") << std::endl
        << "Index HTML: " << ((server.getIndex().size()) > 0 ? server.getIndex() : "No index html") << std::endl;
        for(; serv_it != server.getServerNames().end(); serv_it++)
            std::cout << "Server_name: " << *serv_it << std::endl;
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
        TcpServer tcp_servers(argc > 1 ? argv[1] : "");
        Server a;
        Server b;
        Server c;
        a.setPort(1500);
        b.setPort(2500);
        c.setPort(3500);
        a.setServOption(GET, SET);
        b.setServOption(ALL_METHODS, SET);
        c.setServOption(DELETE, SET);
        c.setServOption(DELETE, CLEAR);
        tcp_servers.pushNewServer(a);
        tcp_servers.pushNewServer(b);
        tcp_servers.pushNewServer(c);
        std::list<Server> tmp(tcp_servers.getServers());
        std::list<Server>::iterator it = tmp.begin();
        std::list<Server>::iterator end = tmp.end();
        for ( ; it != end ; it++)
        {
            print_server_config(*it);
            std::cout << "-----------NEXT_SERV-------------\n\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
}