#include "../includes/TcpServer.hpp"
#include "../includes/Server.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
TcpServer::TcpServer(const std::string& filename)
{
    (void)filename;
}
TcpServer::~TcpServer(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
std::list<Server> TcpServer::getServers(void) const {return _servers; };
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void TcpServer::pushNewServer(const Server& server)
{
    _servers.push_back(server);
};
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/
std::map<std::string, short int> TcpServer::httpResponses;