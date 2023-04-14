#ifndef __LOCATION_HPP_
#define __LOCATION_HPP_
#include "CommonLib.hpp"
#include "BitsManipulation.hpp"
#include "TcpServer.hpp"
class Server;
class Location: public BitsManipulation, public TcpServer
{
    public:
        Location();
        Location(const Location& rhs);
        Location& operator=(const Location& rhs);
        ~Location();
    
        /*GETTERS*/
        const Server* getServer(void) const;
        std::list<Location> getSubLocations(void) const;

        /*Setters*/
        void    pushNewLocation(const Location& location);
        void    setServer(Server *server);

    private:
        std::list<Location> _sub_locations;
        Server  *_server;
};
#endif