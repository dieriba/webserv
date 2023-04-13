#ifndef __SERVER_HPP_
#define __SERVER_HPP_
#include "CommonLib.hpp"
class Location;
class Server
{
    public:
        Server();
        Server(const Server& rhs);
        Server& operator=(const Server& rhs);
        ~Server();

        /*GETTERS*/
        unsigned int  getServOptions(void) const;
        const unsigned int& getPort(void) const;
        const unsigned int& getBodySize(void) const;
        const std::string& getRootDir(void) const;
        const std::string& getIndex(void) const;
        std::vector<Location> getLocations(void) const;
        std::vector<std::string> getServerNames(void) const;

        /*Setters*/
        void    setServOption(const unsigned int& nbit, char actions);
        void    setPort(const unsigned int& port);
        void    setBodySize(const unsigned int& body);
        void    setRootDir(const std::string& root_dir);
        void    setIndex(const std::string& root_dir);
        void    pushNewServerName(const std::string& server_name);
        void    pushNewLocation(const Location& server_name);

        /*MEMBER FUNCTION*/
        bool    checkBits(const unsigned int& nbit) const;
    private:
        unsigned int    _serv_options;
        unsigned int    _port;
        unsigned int    _body_size;
        std::string     _root_dir;
        std::string     _index;
        std::vector<std::string> _server_names;
        std::vector<Location> _locations;
};
#endif