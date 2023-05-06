# include "../../includes/IO/ServerStream.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/TcpServer.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
ServerStream::ServerStream(){};
ServerStream::ServerStream(const int& fd, Server *server):IO(fd, server)
{
    _type = IO::VIRTUAL_SERV;
};
ServerStream::ServerStream(const ServerStream& rhs):IO(rhs){};
ServerStream& ServerStream::operator=(const ServerStream& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    return (*this);
};
ServerStream::~ServerStream(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int ServerStream::handleIoOperation(const int& _ws, struct epoll_event& event)
{
    int client_fd;
    struct epoll_event _ev;

    if (event.events & EPOLLIN)
    {
        while (1)
        {
            client_fd = accept(_fd, NULL, NULL);

            if (client_fd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                return -1;
                        
            _ev.data.ptr = new ClientSocketStream(client_fd, getServer());
            _ev.events = EPOLLIN;

            this -> getServer() -> addToEventsMap((const IO*)_ev.data.ptr);
                
            if (TcpServer::makeNonBlockingFd(client_fd) || epoll_ctl(_ws, EPOLL_CTL_ADD, client_fd, &_ev))
            {
                this -> getServer() -> deleteFromEventsMap(static_cast<const IO*>(_ev.data.ptr));
            }
                        
        }
    }
    return 0;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
