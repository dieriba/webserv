#ifndef __TcpServer_HPP_
#define __TcpServer_HPP_

#include "CommonLib.hpp"
#include <fstream>
class Server;
typedef std::map<short int, std::string>::iterator vec_it;
class TcpServer
{
    public:
        TcpServer(const char *filename);
        ~TcpServer();

        /*GETTERS*/
        std::vector<Server> getServers(void) const;

        /*SETTERS*/
        void pushNewServer(const Server& server);

        /*MEMBER FUNCTION*/
        void getServerConfig(std::ifstream& file);

        /*STATIC MEMBER FUNCTION*/
        static void initHttpResponses(void);
        static const vec_it getHttpResponse(const short int& code);
        static std::map<short int, std::string> httpResponses;
    private:
        /*
        
        */
        TcpServer(const TcpServer& rhs);
        TcpServer& operator=(const TcpServer& rhs);
        TcpServer();
        
        std::vector<Server> _servers;
};
#endif