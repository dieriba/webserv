#include "./includes/CommonLib.hpp"
#include "./includes/TcpServer.hpp"
#include "./includes/Server.hpp"
#include "./includes/ExceptionThrower.hpp"

int main (int argc, char **argv)
{
    try
    {
        throw ExceptionThrower("I Sent An Execption");
        TcpServer tcp_servers(argc > 1 ? argv[1] : "");
        std::list<Server> servers(tcp_servers.getServers());
        std::list<Server>::const_iterator it = servers.begin();
        std::cout << "Server is currently running on port: " << it -> getPort() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
}