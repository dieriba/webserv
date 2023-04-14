#ifndef __SERVER_HPP_
#define __SERVER_HPP_
#include "CommonLib.hpp"
#include "BitsManipulation.hpp"
#include "TcpServer.hpp"

typedef std::vector<std::string>::iterator it_vec;

class Location;
class Server: public BitsManipulation, public TcpServer
{
    public:
        Server();
        Server(const Server& rhs);
        Server& operator=(const Server& rhs);
        ~Server();

        /*GETTERS*/
        const it_vec getCgiPath(const std::string& key);
        const unsigned int& getPort(void) const;
        std::vector<Location> getLocations(void) const;
        std::vector<std::string> getServerNames(void) const;

        /*Setters*/
        void    setPort(const unsigned int& port);
        void    pushNewServerName(const std::string& server_name);
        void    pushNewLocation(const Location& server_name);
        void    pushNewCGI(const std::string& value);

    private:
        unsigned int    _port;
        std::vector<std::string> _server_names;
        std::vector<std::string> _cgi;
        std::vector<Location> _locations;
};
#endif