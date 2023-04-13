#ifndef __LOCATION_HPP_
#define __LOCATION_HPP_
#include "CommonLib.hpp"
class Server;
class Location
{
    public:
        Location();
        Location(const Location& rhs);
        Location& operator=(const Location& rhs);
        ~Location();
    
        /*GETTERS*/
        unsigned int&  getLocationsOptions(void);
        const unsigned int& getBodySize(void) const;
        const std::string& getIndex(void) const;
        const std::string& getRootDir(void) const;
        const std::string& getUri(void) const;
        const Server* getServer(void) const;
        std::list<Location> getSubLocations(void) const;

        /*Setters*/
        void    setBodySize(const unsigned int& body);
        void    setIndex(const std::string& index);
        void    setRootDir(const std::string& root_dir);
        void    setUri(const std::string& index);
        void    pushNewLocation(const Location& location);
        void    setServer(Server *server);
        void    setLocationOption(const unsigned int& nbit, char actions);

        /*MEMBER FUNCTION*/
        bool    checkBits(const unsigned int& nbit) const;
    private:
        unsigned int    _location_options;
        unsigned int    _body_size;
        std::string _index;
        std::string _root_dir;
        std::string _uri;
        std::list<Location> _sub_locations;
        Server  *_server;
};
#endif