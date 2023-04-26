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
    struct epoll_event ev;

    if (event.events & EPOLLIN)
    {
        std::cout << "Entered CLIENT SOCKET STREAM" << std::endl;
        IO *_ev = (IO *)event.data.ptr;
        char buffer[REQUEST_SIZE] = {0};
        int size = recv(_ev -> getFd(), buffer, REQUEST_SIZE, 0);
        std::string s_buffer(buffer);
        TcpServer::setClientBuffers(_ev -> getFd(), s_buffer);
        if (s_buffer.find("\r\n\r\n") != std::string::npos)
        {
            std::cout << "NEW INCOMING REQUEST" << std::endl;
            std::cout << TcpServer::getClientBuffers(_ev -> getFd());
        }
        (void)ev;
        (void)_ws;
        (void)size;
        /*ev.events = EPOLLOUT;
        ev.data.ptr = event.data.ptr;
        if (epoll_ctl(_ws, EPOLL_CTL_MOD, _ev -> getFd(), &ev) == -1)
            close(_ev -> getFd());*/
    }
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
