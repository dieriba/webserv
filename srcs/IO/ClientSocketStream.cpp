# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/TcpServer.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
ClientSocketStream::ClientSocketStream(){};
ClientSocketStream::ClientSocketStream(const int& fd, Server* server):IO(fd, server)
{
    _type = IO::CLIENT_SOCKET;
};
ClientSocketStream::ClientSocketStream(const ClientSocketStream& rhs):IO(rhs){};
ClientSocketStream& ClientSocketStream::operator=(const ClientSocketStream& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    _server = rhs._server;
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
    IO *_ev = (IO *)event.data.ptr;
    if (!validSocketClient(_ev -> getFd(), event)) return ;
    
    if (event.events & EPOLLIN)
    {
        char buffer[REQUEST_SIZE] = {0};
        int size = recv(_ev -> getFd(), buffer, REQUEST_SIZE, 0);
        _request.appendToBuffer(buffer);
        std::string s_buffer(buffer);
        if (s_buffer.find(CRLF) != std::string::npos)
        {
            std::cout << _request.getBuffer();
            /*event.events = EPOLLOUT;
            event.data.ptr = event.data.ptr;
            if (epoll_ctl(_ws, EPOLL_CTL_MOD, _ev -> getFd(), &event) == -1)
                close(_ev -> getFd());*/
        }
        (void)size;
        (void)_ws;
    }
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
