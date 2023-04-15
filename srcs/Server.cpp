# include "../includes/Server.hpp"
# include "../includes/Location.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Server::Server():BitsManipulation(),TcpServer(){};

Server::Server(const Server& rhs):BitsManipulation(rhs),TcpServer(rhs)
{
    _serv_socket = rhs._serv_socket;
    _ip = rhs._ip;
    _port = rhs._port;
    _server_names = rhs._server_names;
    _locations = rhs._locations;
    _cgi = rhs._cgi;
}
Server& Server::operator=(const Server& rhs)
{
    if (this == &rhs) return *this;
    _serv_socket = rhs._serv_socket;
    _ip = rhs._ip;
    _options = rhs._options;
    _body_size = rhs._body_size;
    _port = rhs._port;
    _root_dir = rhs._root_dir;
    _index = rhs._index;
    _server_names = rhs._server_names;
    _locations = rhs._locations;
    _cgi = rhs._cgi;
    return *this;
}
Server::~Server(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const int& Server::getServSocket(void) const {return _serv_socket;};
const std::string& Server::getIp(void) const {return _ip;};
const unsigned int& Server::getPort(void) const {return _port;};
bool Server::getCgiPath(const std::string& key, std::string& path)
{
    it_map it = _cgi.find(key);
    path = (it != _cgi.end()) ? it -> second : "";
    return it != _cgi.end();
};
std::vector<Location> Server::getLocations(void) const {return _locations;};
std::vector<std::string> Server::getServerNames(void) const {return _server_names;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

void    Server::setIp(const std::string& ip)
{
    _ip = ip;
}

void    Server::setPort(const unsigned int& port)
{
    _port = port;
};

void    Server::pushNewServerName(const std::string& server_name)
{
    _server_names.push_back(server_name);
};

void    Server::pushNewLocation(const Location& location)
{
    _locations.push_back(location);
};

void    Server::pushNewCGI(const std::string& key, const std::string& value)
{
    _cgi[key] = value;
}

void    Server::setServSocket(const int& socket)
{
    _serv_socket = socket;
}

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/