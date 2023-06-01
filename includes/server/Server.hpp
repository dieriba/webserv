#ifndef __SERVER_HPP_
#define __SERVER_HPP_

# include "HttpServer.hpp"
# include "../IO/IO.hpp"
# include "../utils/CommonLib.hpp"

typedef std::map<std::string, std::string>::iterator it_map;

class Location;

class Server: public HttpServer
{
    public:
        Server();
        Server(const Server& rhs);
        Server& operator=(const Server& rhs);
        ~Server();

        /*GETTERS*/
        HttpServer *getHttpServer(void) const;
        const int& getServSocket(void) const;
        const std::string& getIp(void) const;
        const unsigned int& getPort(void) const;
        const std::vector<Location>& getLocations(void) const;
        std::vector<Location>& getLocations(void);
        std::vector<std::string> getServerNames(void) const;
        HttpServer *getInstance(void) const;

        /*Setters*/
        void    setHttpServer(HttpServer *tcp_server);
        void    setPort(const unsigned int& port);
        void    pushNewServerName(const std::string& server_name);
        void    pushNewLocation(const Location& server_name);
        void    setIp(const std::string& ip);
        void    setServSocket(const int& socket);
        void    setInstance(HttpServer* instance);

        /*MEMBER FUNCTION*/
        void addToEventsMap(const IO* event);
        void deleteFromEventsMap(const IO* event);
        std::string launchServer(void);

    private:
        int _serv_socket;
        struct sockaddr_in _serv_address;
        unsigned int    _port;
        std::string _ip;
        HttpServer *_instance; //POLYMORPHISM
        HttpServer *_tcp_server; //REAL TCP SERVER POINTER
        std::map<const IO*, const IO*> _events;
        std::vector<std::string> _server_names;
        std::vector<Location> _locations;
};
#endif