# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
IO::IO(){};
IO::IO(const int& fd, Server *server):_fd(fd),_err(0),_tcp_server(NULL),_server(server){};
IO::IO(const IO& rhs):_fd(rhs._fd),_server(rhs._server){};
IO& IO::operator=(const IO& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    _server = rhs._server;
    _err = rhs._err;
    return (*this);
};
IO::~IO(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const int& IO::getFd(void) const { return _fd ;};
TcpServer *IO::getTcpServer(void) const {return _tcp_server;};
const int& IO::getErrStatus(void) const {return _err;}
Server* IO::getServer(void) const { return _server;}
const HttpRequest& IO::getRequest(void) const {return _request;}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void IO::setFD(const int& fd) {_fd = fd;}
void IO::setErrorStatus(const int& err) {_err = err;}
void IO::setTcpServer(TcpServer *tcp_server) {_tcp_server = tcp_server;}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
bool IO::validSocketClient(int _fd, struct epoll_event event)
{
    if ((event.events & EPOLLERR) || (event.events & EPOLLHUP) || (!(event.events & EPOLLIN)))
	{
	    close(_fd);
        return false;
	}
    return true ;
}

void IO::switchEvents(int _ws, struct epoll_event event) const
{
    event.events = EPOLLOUT;
    event.data.ptr = event.data.ptr;
    if (epoll_ctl(_ws, EPOLL_CTL_MOD, getFd(), &event) == -1)
        close(getFd());
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
