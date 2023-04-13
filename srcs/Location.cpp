# include "../includes/Location.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Location::Location()
    :_location_options(0),_body_size(0),_index(""),_root_dir(""),_uri(""),_redirect(""),_server(0){};
Location::Location(const Location& rhs)
{
    _location_options = rhs._location_options;
    _body_size = rhs._body_size;
    _root_dir = rhs._root_dir;
    _index = rhs._index;
    _uri = rhs._uri;
    _sub_locations = rhs._sub_locations;
    _server = rhs._server;
}
Location& Location::operator=(const Location& rhs)
{
    if (this == &rhs) return *this;
    _location_options = rhs._location_options;
    _body_size = rhs._body_size;
    _root_dir = rhs._root_dir;
    _index = rhs._index;
    _uri = rhs._uri;
    _sub_locations = rhs._sub_locations;
    _server = rhs._server;
    return *this;
}
Location::~Location(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
unsigned int&  Location::getLocationsOptions(void) {return _location_options;};
const unsigned int& Location::getBodySize(void) const {return _body_size;};
const std::string& Location::getRootDir(void) const {return _root_dir;};
const std::string& Location::getIndex(void) const {return _index;};
const std::string& Location::getUri(void) const {return _uri;};
const std::string& Location::getRedirect(void) const {return _redirect;};
std::list<Location> Location::getSubLocations(void) const {return _sub_locations;};
const Server* Location::getServer(void) const {return _server; };
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void    Location::setBodySize(const unsigned int& body)
{
    _body_size = body;
};
void    Location::setIndex(const std::string& index)
{
    _index = index;
};
void    Location::setRootDir(const std::string& root_dir)
{
    _root_dir = root_dir;
};
void    Location::setUri(const std::string& uri)
{
    _uri = uri;
};

void    Location::setRedirect(const std::string& redirect)
{
    _redirect = redirect;    
}

void    Location::pushNewLocation(const Location& location)
{
    _sub_locations.push_back(location);
};
void    Location::setServer(Server *server)
{
    _server = server;
};

/*

*/
void    Location::setLocationOption(const unsigned int& nbit, char actions)
{
    if (actions == SET)
        bitset(_location_options, nbit);
    else if (actions == CLEAR)
        bitclear(_location_options, nbit);
};
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/
bool    Location::checkBits(const unsigned int& nbit) const
{
    return (bitcheck(_location_options, nbit) > 0);
}
/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/
