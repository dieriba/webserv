# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/TcpServer.hpp"
# include "../../includes/http/RequestChecker.hpp"

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
    _response.serveResponse((*this), getRequest());
    //Server *server = _ev -> getServer();
}

void ClientSocketStream::readFromSocket(int _ws, struct epoll_event& event, IO *_ev)
{
        char buffer[REQUEST_SIZE] = {0};
        Server *server = _ev -> getServer();

        int size = recv(_ev -> getFd(), buffer, REQUEST_SIZE, 0);

        if (size <= 0)
        {
            server -> getTcpServer() -> deleteFromVectorEvents(_ws, _ev);
            return ;
        }

        _request.appendToBuffer(buffer);
        
        if (_request.getBuffer().size() >= MAX_HEADER_SIZE)
        {
            switchEvents(_ws, event);
            setErrorStatus(TOO_LARGE_CONTENT);
            return ;
        }

        std::string s_buffer(buffer);
        
        if (s_buffer.find(CRLF CRLF) != std::string::npos || (server -> checkBits(C_LEN) || server -> checkBits(T_ENC)))
        {
            s_buffer = _request.getBuffer();
            
            int req = _request.parseRequest(*this);
            
            if ((server -> checkBits(C_LEN) || server -> checkBits(T_ENC)) && !server -> checkBits(FINISH_BODY))
                return ;
            
            req = RequestChecker::checkAll(*server, _request);

            _response.setMethodObj((_request.getMethod() < 3 ? Method::_tab[_request.getMethod()]() : Method::_tab[3]()));

            if (server -> checkBits(FINISH_BODY) != 0)
            {
                _request.appendToBuffer(NEW_LINE);
                std::cout << _request.getBuffer();
                server -> setOptions(FINISH_BODY, CLEAR);
            }
            else if (server -> checkBits(GET) != 0)
                std::cout << s_buffer;
            setErrorStatus(req);
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
