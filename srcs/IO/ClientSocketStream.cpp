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

void ClientSocketStream::writeToSocket(int _ws, struct epoll_event& event, IO* _ev)
{
    (void)_ws;
    (void)event;
    (void)_ev;
    //Server *server = _ev -> getServer();
}

void ClientSocketStream::readFromSocket(int _ws, struct epoll_event& event, IO *_ev)
{
        char buffer[REQUEST_SIZE] = {0};
        int size = recv(_ev -> getFd(), buffer, REQUEST_SIZE, 0);

        if (size <= 0) return ;

        _request.appendToBuffer(buffer);
        
        if (_request.getBuffer().size() >= MAX_HEADER_SIZE)
        {
            switchEvents(_ws, event);
            setErrorStatus(TOO_LARGE_HEADER);
            return ;
        }
        
        std::string s_buffer(buffer);
        
        if (s_buffer.find(CRLF) != std::string::npos)
        {
            _request.appendToBuffer("\n");
            _request.parseRequest(*this);
            std::cout << _request.getBuffer();
            switchEvents(_ws, event);
        }
}

void ClientSocketStream::handleIoOperation(int _ws, struct epoll_event& event)
{
    if (event.events & EPOLLIN)
        readFromSocket(_ws, event, (IO *)event.data.ptr);
    else
        writeToSocket(_ws, event, (IO *)event.data.ptr);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
