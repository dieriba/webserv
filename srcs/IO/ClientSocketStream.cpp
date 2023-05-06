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

int ClientSocketStream::writeToSocket(const int& _ws, struct epoll_event& event)
{
    int res = _response.serveResponse((*this), getRequest());
    
    if (_response.checkBits(HttpResponse::FINISHED_RESPONSE))
    {
        this -> resetOptions();
        utilityMethod::switchEvents(_ws, EPOLLIN, event, (*this));
        _response.resetOptions();
    }

    return res;
}

int ClientSocketStream::readFromSocket(const int& _ws, struct epoll_event& event)
{
    char buffer[REQUEST_SIZE] = {0};

    int size = recv(this -> getFd(), buffer, REQUEST_SIZE, 0);

    if (size <= 0) return IO::IO_ERROR;
        
    _request.appendToBuffer(buffer);
        
    if (_request.getBuffer().size() >= MAX_HEADER_SIZE)
    {
        utilityMethod::switchEvents(_ws, EPOLLOUT, event, *(this));
        setErrorStatus(TOO_LARGE_CONTENT);
        return IO::IO_SUCCESS;
    }

    std::string s_buffer(buffer);
        
    if (s_buffer.find(CRLF CRLF) != std::string::npos || (this -> checkBits(TcpServer::CONTENT_LENGTH) || this -> checkBits(TcpServer::TRANSFER_ENCODING)))
    {
        s_buffer = _request.getBuffer();
            
        int req = _request.parseRequest(*this);

        /*if ((this -> checkBits(TcpServer::CONTENT_LENGTH) || this -> checkBits(TcpServer::TRANSFER_ENCODING)) && !this -> checkBits(TcpServer::FINISH_BODY))
            return ;*/
            
        req = RequestChecker::checkAll(*(this -> getServer()), _request);

        _response.setMethodObj((req == 0 ? Method::_tab[_request.getMethod()]() : Method::_tab[3]()));

        if (this -> checkBits(TcpServer::FINISH_BODY) != 0)
        {
            _request.appendToBuffer(NEW_LINE);
            std::cout << _request.getBuffer();
            this -> setOptions(TcpServer::FINISH_BODY, CLEAR);
        }
        else if (this -> checkBits(TcpServer::GET) != 0)
            std::cout << s_buffer;
            
        setErrorStatus(req);
        utilityMethod::switchEvents(_ws, EPOLLOUT, event, *(this));
    }
    
    return IO::IO_SUCCESS;
}

int ClientSocketStream::handleIoOperation(const int& _ws, struct epoll_event& event)
{
    if (event.events & EPOLLIN)
        return readFromSocket(_ws, event);
    return writeToSocket(_ws, event);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
