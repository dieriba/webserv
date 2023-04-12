#include "./includes/CommonLib.hpp"
#include "./includes/TcpServer.hpp"
#include "./includes/Server.hpp"








int main (int argc, char **argv)
{
    try
    {
        TcpServer tcp_servers(argc > 1 ? argv[1] : "");
        std::list<Server> servers(tcp_servers.getServers());
        std::list<Server>::const_iterator it = servers.begin();
        std::cout << "Value of server port is currently: " << it -> getPort() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
}