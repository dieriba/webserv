#ifndef __SERVER_HPP_
#define __SERVER_HPP_
#include "CommonLib.hpp"
#include "BitsManipulation.hpp"
#include "TcpServer.hpp"
class Location;
class Server: public BitsManipulation, public TcpServer
{
    public:
        Server();
        Server(const Server& rhs);
        Server& operator=(const Server& rhs);
        ~Server();

        /*GETTERS*/
        const unsigned int& getPort(void) const;
        std::vector<Location> getLocations(void) const;
        std::vector<std::string> getServerNames(void) const;

        /*Setters*/
        void    setPort(const unsigned int& port);
        void    pushNewServerName(const std::string& server_name);
        void    pushNewLocation(const Location& server_name);

    private:
        unsigned int    _port;
        std::vector<std::string> _server_names;
        std::vector<Location> _locations;
};
#endif