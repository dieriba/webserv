# include "../includes/Location.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Location::Location():BitsManipulation(),TcpServer(),_server(0){};

Location::Location(const Location& rhs):BitsManipulation(rhs),TcpServer(rhs)
{
    _sub_locations = rhs._sub_locations;
    _server = rhs._server;
}

Location& Location::operator=(const Location& rhs)
{
    if (this == &rhs) return *this;
    _options = rhs._options;
    _body_size = rhs._body_size;
    _root_dir = rhs._root_dir;
    _index = rhs._index;
    _sub_locations = rhs._sub_locations;
    _server = rhs._server;
    return *this;
}
Location::~Location(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
std::list<Location> Location::getSubLocations(void) const {return _sub_locations;};
const Server* Location::getServer(void) const {return _server; };
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void    Location::pushNewLocation(const Location& location)
{
    _sub_locations.push_back(location);
};
void    Location::setServer(Server *server)
{
    _server = server;
};

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/
