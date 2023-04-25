# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/TcpServer.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
ClientSocketStream::ClientSocketStream(){};
ClientSocketStream::ClientSocketStream(const int& fd):IO(fd)
{
    _type = IO::CLIENT_SOCKET;
};
ClientSocketStream::ClientSocketStream(const ClientSocketStream& rhs):IO(rhs._fd){};
ClientSocketStream& ClientSocketStream::operator=(const ClientSocketStream& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    return (*this);
};
ClientSocketStream::~ClientSocketStream(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void ClientSocketStream::handleIoOperation(int _ws, struct epoll_event event)
{
    struct epoll_event ev;

    if (event.events & EPOLLIN)
    {
        IO* _ev = (IO*)event.data.ptr;
        char buffer[REQUEST_SIZE] = {0};
        int size = recv(_ev -> getFd(), buffer, REQUEST_SIZE, 0);
        std::string s_buffer(buffer);
        TcpServer::setClientBuffers(_ev -> getFd(), s_buffer);
        ev.events = EPOLLOUT;
        ev.data.ptr = event.data.ptr;
        if (epoll_ctl(_ws, EPOLL_CTL_MOD, _ev -> getFd(), &ev) == -1)
            close(_ev -> getFd());
    }
    else if (event.events & EPOLLOUT)
    {
        
    }
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
