# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/IO/CgiStream.hpp"
# include "../../includes/server/HttpServer.hpp"
# include "../../includes/http/RequestChecker.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
ClientSocketStream::ClientSocketStream(){};
ClientSocketStream::ClientSocketStream(const int& ws, const int& fd, Server* server):IO(ws, fd, server)
{
    _port = server -> getPort();
    _io = new CgiStream(-1, this, NULL);
    _io -> setIO(this);
    _type = IO::CLIENT_SOCKET;
};

ClientSocketStream::ClientSocketStream(const ClientSocketStream& rhs):IO(rhs){};

ClientSocketStream& ClientSocketStream::operator=(const ClientSocketStream& rhs)
{
    if (this == &rhs) return (*this);
    _ws = rhs._ws;
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

ClientSocketStream::~ClientSocketStream()
{
    delete _io;
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const unsigned int& ClientSocketStream::getPort(void) const
{
    return _port;
}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

int ClientSocketStream::writeToSocket(const int& _ws, struct epoll_event& event)
{

    int res = _response.serveResponse((*this), getRequest());
    std::cout << "RES: " << res << std::endl; 
    if (res > 0)
        _response.switchMethod((*this), HttpServer::HTTP_SERVER_ERROR, res);
    else if (_response.checkBits(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE))
    {
        UtilityMethod::switchEvents(_ws, EPOLLIN, event, (*this));
        clear();
    }

    return res;
}

int ClientSocketStream::readFromSocket(const int& _ws, struct epoll_event& event)
{
    char buffer[REQUEST_SIZE];

    int size = recv(this -> getFd(), buffer, REQUEST_SIZE, 0);

    if (size <= 0) return IO::IO_ERROR;
        
    _request.appendToBuffer(buffer, size);

    if (_request.getHeaderSize() >= MAX_HEADER_SIZE)
    {
        UtilityMethod::switchEvents(_ws, EPOLLOUT, event, *(this));
        setErrorStatus(TOO_LARGE_CONTENT);
        return IO::IO_SUCCESS;
    }

    char *end_header = UtilityMethod::mystrstr(_request.getBuffer().c_str(), CRLF CRLF);
    
    if (end_header != NULL || (_request.checkBits(HttpRequest::HTTP_REQUEST_CONTENT_LENGTH) || _request.checkBits(HttpRequest::HTTP_REQUEST_TRANSFER_ENCODING)))
    {
        int _req = _request.parseRequest(*this);

        if (!_req && ((_request.checkBits(HttpRequest::HTTP_REQUEST_CONTENT_LENGTH) || _request.checkBits(HttpRequest::HTTP_REQUEST_TRANSFER_ENCODING)) && !_request.checkBits(HttpRequest::HTTP_REQUEST_FINISH_BODY)))
        {
            _req = _response.serveResponse((*this), _request);
            if (_req)
                _response.switchMethod((*this), HttpServer::HTTP_SERVER_ERROR, _req);
            else if (!_request.checkBits(HttpRequest::HTTP_REQUEST_FINISH_BODY))
                return IO::IO_SUCCESS;
        }

        if (_response.getHttpMethod() == NULL)
            _response.setMethodObj((_req < 10 ? Method::_tab[_request.getMethod()]() : Method::_tab[HttpServer::HTTP_SERVER_ERROR]()));

        setErrorStatus(_req);

        _request.getBuffer().clear();
        UtilityMethod::switchEvents(_ws, EPOLLOUT, event, *(this));
    }
    
    return IO::IO_SUCCESS;
}

int ClientSocketStream::handleIoOperation(const int& _ws, struct epoll_event& event)
{
    if (event.events & EPOLLIN)
    {
        try
        {
            return readFromSocket(_ws, event);
        }
        catch(const std::exception& e)
        {
            _response.switchMethod((*this), HttpServer::HTTP_SERVER_ERROR, INTERNAL_SERVER_ERROR);
            resetOptions();
            _request.getBuffer().clear();
            UtilityMethod::switchEvents(_ws, EPOLLOUT, event, *(this));
        }
    }

    if (checkBits(IO::CGI_ON))
    {
        CgiStream& cgi = static_cast<CgiStream& >(*(getIO()));
        if ((getTimestampInMillisecond(std::clock()) - cgi.getTimestampInMillisecond(cgi.getCgiTimeStamp())) >= TIMEOUT_CGI)
        {
            epoll_ctl(_ws, EPOLL_CTL_DEL, cgi.getFd(), NULL);
            getReponse().clearReadEnd();
            if (cgi.checkBits(CgiStream::STARTED) != 0)
            {
                this -> clear();
                UtilityMethod::switchEvents(_ws, EPOLLIN, event, (*this));
                return IO::IO_SUCCESS;
            }
            _response.switchMethod((*this), HttpServer::HTTP_SERVER_ERROR, GATEWAY_TIMEOUT);
            setOptions(IO::CGI_ON, CLEAR);
        }
    }

    if (checkBits(IO::CGI_ON) == 0 && event.events & EPOLLOUT) return writeToSocket(_ws, event);

    return IO::IO_SUCCESS;
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
