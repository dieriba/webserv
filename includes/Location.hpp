#ifndef __LOCATION_HPP_
#define __LOCATION_HPP_
#include "CommonLib.hpp"
#include "TcpServer.hpp"
class Server;
class Location: public TcpServer
{
    public:
        Location();
        Location(const Location& rhs);
        Location& operator=(const Location& rhs);
        ~Location();
    
        /*GETTERS*/
        const Server* getServer(void) const;
        const std::string& getSubPath() const;
        std::list<Location> getSubLocations(void) const;

        /*Setters*/
        void    pushNewLocation(const Location& location);
        void    setServer(Server *server);
        void    setSubPath(const std::string& sub_path);

    private:
        std::list<Location> _sub_locations;
        std::string _sub_path;
        Server  *_server;
};
#endif