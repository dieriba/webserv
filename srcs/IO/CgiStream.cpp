# include "../../includes/IO/CgiStream.hpp"
# include "../../includes/method/Get.hpp"
# include "../../includes/utils/UtilityMethod.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
CgiStream::CgiStream(){};
CgiStream::CgiStream(const int& fd, IO *event, int *pipes):_io(event),_pipes(pipes)
{
    setFD(fd);
    _cgi_timestamp = 0;
    _type = IO::CGI_STREAM;
};
CgiStream::CgiStream(const CgiStream& rhs):IO(rhs),_io(rhs._io),_pipes(rhs._pipes)
{};
CgiStream& CgiStream::operator=(const CgiStream& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    _cgi_timestamp = 0;
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
long CgiStream::getCgiTimeStamp(void) const { return _cgi_timestamp; }
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void CgiStream::setPipes(int *pipes) { _pipes = pipes; }
void CgiStream::updateCgiTimeStamp(void) { _cgi_timestamp = std::clock(); };
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
    object.getReponse().clearReadEnd();
    return IO::IO_SUCCESS;
}

int CgiStream::handleIoOperation(const int& _ws, struct epoll_event& /* event */)
{
    try
    {
        IO& object = *(getIO());

        if (object.checkBits(IO::CGI_ON) == 0) return IO::IO_SUCCESS;

        if (checkBits(CgiStream::STARTED) == 0)
        {
            Get object;
            object.makeStatusLine((*this), OK);
            object.appendToResponse(CONTENT_TYP, "text/html");
            object.appendToResponse(TRANSFERT_ENCODING, "chunked");
            object.addEndHeaderCRLF();

            _response.getBuffer().append(object.getResponse());
            setOptions(CgiStream::STARTED, SET);
        }

        updateCgiTimeStamp();

        int bytes = read(_pipes[0], _buffer, REQUEST_SIZE);

        if (bytes < 0) return resetCgi(object, _ws);
        
        std::string hex = UtilityMethod::decimalToHex(bytes);

        _response.appendToBuffer(hex.c_str(), hex.size());
        _response.appendToBuffer(CRLF, LEN_CRLF);
        _response.appendToBuffer(_buffer, bytes);
        _response.appendToBuffer(CRLF, LEN_CRLF);

        std::cout << _response.getBuffer() << std::endl;

        std::string& resp = _response.getBuffer();

        if (UtilityMethod::sendBuffer(object.getFd(), resp.data(), resp.size()) == IO::IO_ERROR)
        {
            UtilityMethod::deleteEventFromEpollInstance(_ws, _fd);
            object.setOptions(IO::KILL_MYSELF, SET);
            resp.clear();
            return IO::IO_SUCCESS;
        }
    
        resp.clear();
        if (bytes == 0) return resetCgi(object, _ws);
    }
    catch(const std::exception& e)
    {
        return IO::IO_ERROR;
    }

    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
