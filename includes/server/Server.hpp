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
        Server(const Server&);
        Server& operator=(const Server&);
        ~Server();

        /*GETTERS*/
        HttpServer *getHttpServer(void) const;
        const int& getServSocket(void) const;
        const std::string& getIp(void) const;
        const unsigned int& getPort(void) const;
        const std::vector<Location>& getLocations(void) const;
        std::vector<Location>& getLocations(void);
        std::map<const IO*, const IO*>& getEventsMap();
        const std::vector<std::string>& getServerNames(void) const;
        HttpServer *getInstance(void) const;

        /*Setters*/
        void    setHttpServer(HttpServer *);
        void    setPort(const unsigned int&);
        void    pushNewServerName(const std::string&);
        void    pushNewLocation(const Location&);
        void    setIp(const std::string&);
        void    setServSocket(const int&);
        void    setInstance(HttpServer*);

        /*MEMBER FUNCTION*/
        void addToEventsMap(IO*);
        void deleteFromEventsMap(IO&);
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