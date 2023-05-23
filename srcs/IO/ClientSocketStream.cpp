# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/server/TcpServer.hpp"
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
    
    if (_response.checkBits(HttpResponse::FINISHED_RESPONSE) || res > 0)
    {
        UtilityMethod::switchEvents(_ws, EPOLLIN, event, (*this));
        setErrorStatus(res);
        clear();
    }

    return res;
}

int ClientSocketStream::readFromSocket(const int& _ws, struct epoll_event& event)
{
    char buffer[REQUEST_SIZE];

    int size = recv(this -> getFd(), buffer, REQUEST_SIZE, 0);

    std::cout << "Recv return value is: " << size << std::endl;
    if (size <= 0) return IO::IO_ERROR;
        
    _request.appendToBuffer(buffer, size);

    if (_request.getHeaderSize() >= MAX_HEADER_SIZE)
    {
        UtilityMethod::switchEvents(_ws, EPOLLOUT, event, *(this));
        setErrorStatus(TOO_LARGE_CONTENT);
        return IO::IO_SUCCESS;
    }

    char *end_header = UtilityMethod::mystrstr(_request.getBuffer().c_str(), CRLF CRLF);
    
    if (end_header != NULL || (checkBits(HttpRequest::CONTENT_LENGTH) || checkBits(HttpRequest::TRANSFER_ENCODING)))
    {
        int req = _request.parseRequest(*this);
        if (!req && ((checkBits(HttpRequest::CONTENT_LENGTH) || checkBits(HttpRequest::TRANSFER_ENCODING)) && !checkBits(HttpRequest::FINISH_BODY)))
        {
            _response.serveResponse((*this), _request);
            if (!checkBits(HttpRequest::FINISH_BODY)) return IO::IO_SUCCESS;
        }

        if (_response.getHttpMethod() == NULL)
            _response.setMethodObj((req < 10 ? Method::_tab[_request.getMethod()]() : Method::_tab[3]()));

        resetOptions();

        setErrorStatus(req);

        _request.getBuffer().clear();
        
        UtilityMethod::switchEvents(_ws, EPOLLOUT, event, *(this));
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
