# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
IO::IO(){};
IO::IO(const int& fd, Server *server):_fd(fd),_server(server){};
IO::IO(const IO& rhs):_fd(rhs._fd),_server(rhs._server){};
IO& IO::operator=(const IO& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    _server = rhs._server;
    return (*this);
};
IO::~IO(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const int& IO::getFd(void) const { return _fd ;};
Server* IO::getServer(void) const { return _server;}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void IO::setFD(const int& fd)
{
    _fd = fd;
}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
