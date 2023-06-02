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
int CgiStream::handleIoOperation(const int& _ws, struct epoll_event&)
{
    _bytes = read(_pipes[0], _buffer, REQUEST_SIZE);
    IO* object = getIO();

    if (_bytes <= 0)
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

    std::string hex = UtilityMethod::decimalToHex(_bytes);

    

    /*if (UtilityMethod::sendBuffer(object -> getFd(), _buffer, _bytes) == IO::IO_ERROR)
    {
        UtilityMethod::deleteEventFromEpollInstance(_ws, _fd);
        return IO::IO_ERROR;
    }*/

    std::cout << _bytes << std::endl;
    
    exit(1);
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
