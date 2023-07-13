# include "../../includes/IO/ServerStream.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/server/HttpServer.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
ServerStream::ServerStream(){};
ServerStream::ServerStream(const int& fd, Server *server):IO(fd, server)
{
    _type = IO::IO_VIRTUAL_SERV;
};
ServerStream::ServerStream(const ServerStream& rhs):IO(rhs){};
ServerStream& ServerStream::operator=(const ServerStream& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    _err = rhs._err;
    _server = rhs._server;
    _event = rhs._event;
    _io = rhs._io;
    _request = rhs._request;
    _response = rhs._response;
    _options = rhs._options;
    return (*this);
};
ServerStream::~ServerStream(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int ServerStream::handleIoOperation(const int& _ws, struct epoll_event& /* event */)
{

    while (1)
    {
        std::cout << "Entered" << std::endl;
        int client_fd = accept(_fd, NULL, NULL);
        if (client_fd == -1) return IO::IO_SUCCESS;
                        
        struct epoll_event _ev;

        _ev.data.ptr = new ClientSocketStream(_ws, client_fd, getServer(), getServer());
        _ev.events = EPOLLIN;

        this -> getServer() -> addToEventsMap((IO*)_ev.data.ptr);
        if (HttpServer::makeNonBlockingFd(client_fd) || epoll_ctl(_ws, EPOLL_CTL_ADD, client_fd, &_ev))
            return IO::IO_ERROR;
    }
    
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
