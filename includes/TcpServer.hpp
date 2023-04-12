#ifndef __TcpServer_HPP_
#define __TcpServer_HPP_

#include "CommonLib.hpp"
class Server;
class TcpServer
{
    public:
        TcpServer(const std::string& filename);
        ~TcpServer();

        /*GETTERS*/
        const std::list<Server>& getServers(void) const;
    private:
        TcpServer();
        TcpServer(const TcpServer& rhs);
        TcpServer& operator=(const TcpServer& rhs);
        std::list<Server> _servers;
        /* data */
};
#endif