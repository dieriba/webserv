# include "../../includes/server/Server.hpp"
# include "../../includes/server/Location.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Server::Server():HttpServer(),_serv_socket(-1),_tcp_server(NULL){};

Server::Server(const Server& rhs):HttpServer(rhs)
{
    _serv_socket = rhs._serv_socket;
    _ip = rhs._ip;
    _port = rhs._port;
    _server_names = rhs._server_names;
    _locations = rhs._locations;
    _tcp_server = rhs._tcp_server;
}
Server& Server::operator=(const Server& rhs)
{
    if (this == &rhs) return *(this);
    _upload_file_folders = rhs._upload_file_folders;
    _serv_socket = rhs._serv_socket;
    _error_pages = rhs._error_pages;
    _options = rhs._options;
    _tcp_server = rhs._tcp_server;
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
    _index_path = rhs._index_path;
    return *(this);
}
Server::~Server()
{
    if (_serv_socket != -1) close(_serv_socket);
    
    if (getHttpServer() != NULL)
    {
        std::map<const IO*, const IO*>::const_iterator it = _events.begin();
        std::map<const IO*, const IO*>::const_iterator end = _events.end();
        int _ws = getHttpServer() -> getEpollWs();
        
        for ( ; it != end; it++)
        {
            epoll_ctl(_ws, EPOLL_CTL_DEL, (it -> first) -> getFd(), NULL);
            close((it -> first) -> getFd());
            delete it -> first;
        }
    }
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const int& Server::getServSocket(void) const {return _serv_socket;};
const std::string& Server::getIp(void) const {return _ip;};
const unsigned int& Server::getPort(void) const {return _port;};
const std::vector<Location>& Server::getLocations(void) const {return _locations;};
HttpServer *Server::getHttpServer(void) const {return _tcp_server;};
std::vector<Location>& Server::getLocations(void) {return _locations;}
const std::vector<std::string>& Server::getServerNames(void) const {return _server_names;};
HttpServer *Server::getInstance(void) const {return _instance;}
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

void    Server::setServSocket(const int& socket)
{
    _serv_socket = socket;
}

void Server::setInstance(HttpServer* instance)
{
    _instance = instance;
}

void Server::setHttpServer(HttpServer *tcp_server) 
{
    _tcp_server = tcp_server;
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

    if (HttpServer::makeNonBlockingFd(_serv_socket) < 0)
        return "Failled To Set Server Socket As Non Blocking";

    if (listen(_serv_socket, SOMAXCONN))
        return "Listen System Call Failled";

    return "";
}

void Server::addToEventsMap(const IO* event)
{
    _events[event];
}

void Server::deleteFromEventsMap(const IO *event)
{
    if (_events.find(event) == _events.end()) return;
    _events.erase(event);
    if (event -> getFd() > 0) epoll_ctl(getEpollWs(), EPOLL_CTL_DEL, event -> getFd(), NULL);
    delete event;
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/