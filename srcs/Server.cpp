# include "../includes/Server.hpp"
# include "../includes/Location.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Server::Server():BitsManipulation(),TcpServer(){};

Server::Server(const Server& rhs):BitsManipulation(rhs),TcpServer(rhs)
{
    _port = rhs._port;
    _server_names = rhs._server_names;
    _locations = rhs._locations;
}
Server& Server::operator=(const Server& rhs)
{
    if (this == &rhs) return *this;
    _options = rhs._options;
    _body_size = rhs._body_size;
    _port = rhs._port;
    _root_dir = rhs._root_dir;
    _index = rhs._index;
    _server_names = rhs._server_names;
    _locations = rhs._locations;
    return *this;
}
Server::~Server(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const unsigned int& Server::getPort(void) const {return _port;};
std::vector<Location> Server::getLocations(void) const {return _locations;};
std::vector<std::string> Server::getServerNames(void) const {return _server_names;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
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

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/