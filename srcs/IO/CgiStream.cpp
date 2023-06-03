# include "../../includes/IO/CgiStream.hpp"
# include "../../includes/utils/UtilityMethod.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
CgiStream::CgiStream(){};
CgiStream::CgiStream(const int& fd, IO *event, int *pipes):_io(event),_pipes(pipes),_bytes(CgiStream::NO_DATA_AVAILABLE)
{
    setFD(fd);
    _type = IO::CGI_STREAM;
};
CgiStream::CgiStream(const CgiStream& rhs):IO(rhs),_io(rhs._io),_pipes(rhs._pipes),_bytes(rhs._bytes)
{};
CgiStream& CgiStream::operator=(const CgiStream& rhs)
{
    if (this == &rhs) return (*this);
    _bytes = rhs._bytes;
    _fd = rhs._fd;
    _err = rhs._err;
    _server = rhs._server;
    _event = rhs._event;
    _io = rhs._io;
    _pipes = rhs._pipes;
    _request = rhs._request;
    _response = rhs._response;
    _options = rhs._options;
    return (*this);
};
CgiStream::~CgiStream(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const int& CgiStream::getBytes(void) const {return _bytes;};
const char *CgiStream::getBuffer(void) const {return _buffer;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void CgiStream::setBytes(const int& bytes) { _bytes = bytes; }
void CgiStream::setPipes(int *pipes) { _pipes = pipes; }
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int CgiStream::resetCgi(IO* object, const int& _ws)
{
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = object;
    UtilityMethod::switchEvents(_ws, EPOLLIN, ev, (*object));
    UtilityMethod::deleteEventFromEpollInstance(_ws, _fd);
    object -> clear();
    this -> clear();

    return IO::IO_SUCCESS;
}

int CgiStream::handleIoOperation(const int& _ws, struct epoll_event&)
{
    try
    {
        _bytes = read(_pipes[0], _buffer, REQUEST_SIZE);

        IO* object = getIO();
        
        if (_bytes < 0) return resetCgi(object, _ws);
        
        std::string hex = UtilityMethod::decimalToHex(_bytes);

        _response.appendToBuffer(hex.c_str(), hex.size());
        _response.appendToBuffer(CRLF, LEN_CRLF);
        _response.appendToBuffer(_buffer, _bytes);
        _response.appendToBuffer(CRLF, LEN_CRLF);

        std::string& resp = _response.getBuffer();

        if (UtilityMethod::sendBuffer(object -> getFd(), resp.data(), resp.size()) == IO::IO_ERROR)
        {
            UtilityMethod::deleteEventFromEpollInstance(_ws, _fd);
            resp.clear();
            return IO::IO_ERROR;
        }
    
        resp.clear();

        if (_bytes == 0) return resetCgi(object, _ws);
    }
    catch(const std::exception& e)
    {
        return IO::IO_ERROR;
    }

    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
