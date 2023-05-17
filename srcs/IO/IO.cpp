# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
IO::IO(){};
IO::IO(const int& fd, Server *server):BitsManipulation(),_fd(fd),_err(0),_server(server){};
IO::IO(const IO& rhs):BitsManipulation(rhs),_fd(rhs._fd),_server(rhs._server){};
IO& IO::operator=(const IO& rhs)
{
    if (this == &rhs) return (*this);
    _options = rhs._options;
    _fd = rhs._fd;
    _server = rhs._server;
    _err = rhs._err;
    return (*this);
};
IO::~IO()
{
    if (_fd != -1) close(_fd);
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const int& IO::getFd(void) const { return _fd ;};
const int& IO::getErrStatus(void) const {return _err;}
Server* IO::getServer(void) const { return _server;}
const HttpRequest& IO::getRequest(void) const {return _request;}
HttpRequest& IO::getRequest(void)  {return _request;}
const HttpResponse& IO::getReponse(void) const {return _response;};
HttpResponse& IO::getReponse(void) {return _response;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void IO::setFD(const int& fd) {_fd = fd;}
void IO::setErrorStatus(const int& err) {_err = err;}
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

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
