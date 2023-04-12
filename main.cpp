#include "./includes/CommonLib.hpp"
#include "./includes/TcpServer.hpp"
#include "./includes/Server.hpp"
#include "./includes/ExceptionThrower.hpp"

void    print_server_config(Server server)
{
    std::list<std::string>::const_iterator serv_it = server.getServerNames().begin();
    std::cout
        << "METHOD GET: " << (bitset(server.getServOptions(), GET) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD POST: " << (bitset(server.getServOptions(), POST) > 0 ? "Enabled" : "Disabled") << std::endl
        << "METHOD DELETE: " << (bitset(server.getServOptions(), DELETE) > 0 ? "Enabled" : "Disabled") << std::endl << std::endl
        << "PORT: " << server.getPort() << std::endl
        << "Client Max Body Size: " << server.getBodySize() << std::endl
        << "Root Directory: " << ((server.getRootDir().size()) > 0 ? server.getRootDir() : "No root directory") << std::endl
        << "Index HTML: " << ((server.getIndex().size()) > 0 ? server.getIndex() : "No root directory") << std::endl
        for(; serv_it != server.getServerNames().end(); serv_it++)
            << "Server_name: " << *serv_it << std::endl
        for(; loca_it != server.getLocations().end(); loca_it++)
        {
            print_location_config(*loca_it);
            << "Next Location" << std::endl
        }
        ;
}
/*void    print_location_config(const Location& location)
{
    std::cout
        << "METHOD GET: " << bitset(location.getLocationsOptions(), GET) > 0 ? "Enabled" : "Disabled" << std::endl
        << "METHOD POST: " << bitset(location.getLocationsOptions(), POST) > 0 ? "Enabled" : "Disabled" << std::endl
        << "METHOD DELETE: " << bitset(location.getLocationsOptions(), DELETE) > 0 ? "Enabled" : "Disabled" << std::endl << std::endl
        << "Client Max Body Size: " << location.getBodySize() << std::endl
        << "Index HTML: " << << (location.getIndex().size()) > 0 ? location.getIndex() : "No root directory" << std::endl
        << "Root Directory: " << (location.getRootDir().size()) > 0 ? location.getRootDir() : "No root directory" << std::endl
        << "Index HTML: " << << (location.getUri().size()) > 0 ? location.getUri() : "No root directory" << std::endl
        << "Server: " << location.getServer() << std::endl;
}*/

int main (int argc, char **argv)
{
    try
    {
        //throw ExceptionThrower("I Sent An Execption");
        TcpServer tcp_servers(argc > 1 ? argv[1] : "");
        std::list<Server> servers(tcp_servers.getServers());
        std::list<Server>::iterator it = servers.begin();
        it -> setPort(9000);
        print_server_config(*it);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
}