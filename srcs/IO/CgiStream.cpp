# include "../../includes/IO/CgiStream.hpp"
# include "../../includes/method/Get.hpp"
# include "../../includes/utils/UtilityMethod.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
CgiStream::CgiStream(){};
CgiStream::CgiStream(const int& fd, IO *client, int *pipes):_io(client),_pipes(pipes)
{
    _response.setFd(client -> getFd());
    setFD(fd);
    _pid = 0;
    _type = IO::IO_CGI_STREAM;
};
CgiStream::CgiStream(const CgiStream& rhs):IO(rhs),_io(rhs._io),_pipes(rhs._pipes)
{};
CgiStream& CgiStream::operator=(const CgiStream& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    _timestamp = rhs._timestamp;
    _server = rhs._server;
    _event = rhs._event;
    _io = rhs._io;
    _pipes = rhs._pipes;
    _request = rhs._request;
    _response = rhs._response;
    _options = rhs._options;
    return (*this);
};
CgiStream::~CgiStream()
{
    if (_pid > 0) kill(_pid, SIGTERM);
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
long CgiStream::getCgiBeginTimestamp(void) const { return _cgi_begin; }
const pid_t& CgiStream::getPid(void) const { return _pid; };
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void CgiStream::setPipes(int *pipes) { _pipes = pipes; }
void CgiStream::setBeginTimeStamp(void) { _cgi_begin = std::clock(); }
void CgiStream::setPid(const pid_t& pid) { _pid = pid; }
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int CgiStream::resetCgi(IO& object, const int& _ws)
{
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = &object;
    UtilityMethod::switchEvents(_ws, EPOLLIN, ev, object);
    UtilityMethod::deleteEventFromEpollInstance(_ws, _fd);
    object.clear();
    this -> clear();
    _pid = 0;
    object.getResponse().clearReadEnd();
    return IO::IO_SUCCESS;
}

int CgiStream::handleIoOperation(const int& _ws, struct epoll_event& /* event */)
{
    IO& object = *(getIO());

    if (object.checkBits(IO::IO_CGI_ON) == 0) return IO::IO_SUCCESS;

    if (checkBits(CgiStream::STARTED) == 0)
    {
        _response.makeStatusLine((*this), OK)
                 .setHeader(CONTENT_TYP, "text/html")
                 .setHeader(TRANSFERT_ENCODING, "chunked")
                 .addEndHeaderCRLF();
        setBeginTimeStamp();
        setOptions(CgiStream::STARTED, SET);
    }        

    updateTimeStamp();

    int bytes = read(_pipes[0], _buffer, REQUEST_SIZE);

    if (bytes < 0) return resetCgi(object, _ws);
        
    std::string hex = UtilityMethod::decimalToHex(bytes);

    _response.appendToResponse(hex.c_str(), hex.size());
    _response.appendToResponse(CRLF, LEN_CRLF);
    _response.appendToResponse(_buffer, bytes);
    _response.appendToResponse(CRLF, LEN_CRLF);

    if (_response.sendResponse() == IO::IO_ERROR)
    {
        UtilityMethod::deleteEventFromEpollInstance(_ws, _fd);
        object.setOptions(IO::IO_KILL_CGI, SET);
        return IO::IO_SUCCESS;
    }

    if (bytes == 0) return resetCgi(object, _ws);
    if ((getTimestampInMillisecond(std::clock()) - getTimestampInMillisecond(getCgiBeginTimestamp())) >= MAX_TIMEOUT_CGI)
    {
        kill(_pid, SIGTERM);
        std::cout << "TIMEOUT EPOLLOUT" << std::endl;
        if (_response.sendResponse("0\r\n\r\n", 5) == IO::IO_ERROR)
        {
            UtilityMethod::deleteEventFromEpollInstance(_ws, _fd);
            object.setOptions(IO::IO_KILL_CGI, SET);
            return IO::IO_SUCCESS;
        }

        return resetCgi(object, _ws);
    }

    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
