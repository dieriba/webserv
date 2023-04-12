# include "../includes/Server.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Server::Server()
    :_serv_options(0),_port(0),_root_dir(""),_index(""),_body_size(0){};
Server::Server(const Server& rhs)
{
    _serv_options = rhs._serv_options;
    _body_size = rhs._body_size;
    _port = rhs._port;
    _root_dir = rhs._root_dir;
    _index = rhs._index;
    _server_names = rhs._server_names;
    _locations = rhs._locations;
}
Server& Server::operator=(const Server& rhs)
{
    if (this == &rhs) return *this;
    _serv_options = rhs._serv_options;
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
unsigned int&  Server::getServOptions(void) {return _serv_options;};
const unsigned int& Server::getPort(void) const {return _port;};
const unsigned int& Server::getBodySize(void) const {return _body_size;};
const std::string& Server::getRootDir(void) const {return _root_dir;};
const std::string& Server::getIndex(void) const {return _index;};
const std::list<std::string>& Server::getServerNames(void) const {return _server_names;};
const std::list<Location>& Server::getLocations(void) const {return _locations;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void    Server::setPort(const unsigned int& port)
{
    _port = port;
};
void    Server::setBodySize(const unsigned int& body)
{
    _body_size = body;
};
void    Server::setRootDir(const std::string& root_dir)
{
    _root_dir = root_dir;
};
void    Server::setIndex(const std::string& index)
{
    _index = index;
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


/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/
