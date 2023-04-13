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
        std::list<Server> getServers(void) const;

        /*SETTERS*/
        void pushNewServer(const Server& server);
    private:
        /*
        
        */
        TcpServer(const TcpServer& rhs);
        TcpServer& operator=(const TcpServer& rhs);
        TcpServer();
        
        std::list<Server> _servers;
        static std::map<std::string, short int> httpResponses;
};
#endif