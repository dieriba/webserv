# include "../includes/Server.hpp"
# include "../includes/Location.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Server::Server():TcpServer(),_serv_socket(-1){};

Server::Server(const Server& rhs):TcpServer(rhs)
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
    _redirect = rhs._redirect;
    return *this;
}
Server::~Server()
{
    if (_serv_socket != -1) close(_serv_socket);
};
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
const std::vector<Location>& Server::getLocations(void) const {return _locations;};
std::vector<Location>& Server::getLocations(void) {return _locations;}
std::vector<std::string> Server::getServerNames(void) const {return _server_names;};
TcpServer *Server::getInstance(void) const {return _instance;}
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

void Server::setInstance(TcpServer* instance)
{
    _instance = instance;
}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
std::string Server::launchServer(void)
{
    int option = 1;

    if ((_serv_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return ("Cannot Create Socket");   
    memset((char *)&_serv_address, 0, sizeof(_serv_address));
    
    _serv_address.sin_family = AF_INET;
    _serv_address.sin_addr.s_addr = htonl(INADDR_ANY);
    _serv_address.sin_port = htons(_port);

    if (setsockopt(_serv_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) < 0)
        return "Failed to set socket options";

    if (bind(_serv_socket,(struct sockaddr *)&_serv_address,sizeof(_serv_address)) < 0)
        return "Failed to bind socket";

    if (TcpServer::makeNonBlockingFd(_serv_socket) < 0)
        return "Failled To Set Server Socket As Non Blocking";

    if (listen(_serv_socket, SOMAXCONN))
        return "Listen System Call Failled";

    return "";
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/