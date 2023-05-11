#ifndef __SERVER_HPP_
#define __SERVER_HPP_

# include "TcpServer.hpp"
# include "../IO/IO.hpp"
# include "../utils/CommonLib.hpp"

typedef std::map<std::string, std::string>::iterator it_map;

class Location;

class Server: public TcpServer
{
    public:
        Server();
        Server(const Server& rhs);
        Server& operator=(const Server& rhs);
        ~Server();

        /*GETTERS*/
        TcpServer *getTcpServer(void) const;
        const int& getServSocket(void) const;
        const std::string& getIp(void) const;
        const unsigned int& getPort(void) const;
        bool getCgiPath(const std::string& key, std::string& path);
        const std::vector<Location>& getLocations(void) const;
        std::vector<Location>& getLocations(void);
        std::map<std::string, std::string> getCgiMap() const {return _cgi;};
        std::vector<std::string> getServerNames(void) const;
        TcpServer *getInstance(void) const;

        /*Setters*/
        void    setTcpServer(TcpServer *tcp_server);
        void    setPort(const unsigned int& port);
        void    pushNewServerName(const std::string& server_name);
        void    pushNewLocation(const Location& server_name);
        void    pushNewCGI(const std::string& key, const std::string& value);
        void    setIp(const std::string& ip);
        void    setServSocket(const int& socket);
        void    setInstance(TcpServer* instance);

        /*MEMBER FUNCTION*/
        void addToEventsMap(const IO* event);
        void deleteFromEventsMap(const IO* event);
        std::string launchServer(void);

    private:
        int _serv_socket;
        struct sockaddr_in _serv_address;
        unsigned int    _port;
        std::string _ip;
        TcpServer *_instance; //POLYMORPHISM
        TcpServer *_tcp_server; //REAL TCP SERVER POINTER
        std::map<const IO*, const IO*> _events;
        std::vector<std::string> _server_names;
        std::map<std::string, std::string> _cgi;
        std::vector<Location> _locations;
};
#endif