# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
IO::IO(){};
IO::IO(const int& ws, const int& fd, Server *server):BitsManipulation(),_ws(ws),_fd(fd),_timestamp(getCurrentTimestampMs()),_server(server),_event(EPOLLIN),_io(NULL){};
IO::IO(const int& fd, Server *server):BitsManipulation(),_ws(-1),_fd(fd),_timestamp(getCurrentTimestampMs()),_server(server),_event(EPOLLIN),_io(NULL){};
IO::IO(const IO& rhs):BitsManipulation(rhs),_ws(rhs._ws),_fd(rhs._fd),_timestamp(rhs._timestamp),_server(rhs._server),_event(rhs._event),_io(rhs._io)
,_request(rhs._request),_response(rhs._response){};
IO& IO::operator=(const IO& rhs)
{
    if (this == &rhs) return (*this);
    _ws = rhs._ws;
    _fd = rhs._fd;
    _timestamp = rhs._timestamp;
    _server = rhs._server;
    _event = rhs._event;
    _io = rhs._io;
    _request = rhs._request;
    _response = rhs._response;
    _options = rhs._options;
    return (*this);
};
IO::~IO()
{
    if (_fd != -1) close(_fd);
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
int IO::getType(void) const {return _type;}
long IO::getTimeStamp(void) const {return _timestamp;};
const int& IO::getFd(void) const { return _fd ;};
const HttpResponse& IO::getResponse(void) const {return _response;};
const HttpRequest& IO::getRequest(void) const {return _request;}
const uint32_t& IO::getEvents(void) const {return _event;};
const int& IO::getWs(void) const {return _ws;};
Server* IO::getServer(void) const { return _server;}
Server* IO::getBaseServer(void) { return _base_server;}
HttpRequest& IO::getRequest(void)  {return _request;}
HttpResponse& IO::getResponse(void) {return _response;};
IO* IO::getIO(void) const {return _io;}; 
const std::string& IO::getCookie(void) const { return _cookies ;}
/*----------------------------------------GETTER----------------------------------------*/
long IO::getCurrentTimestampMs(void) const 
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec * 1000) + (now.tv_usec / 1000));
}
/*----------------------------------------SETTER----------------------------------------*/
void IO::setCookie(const std::string& cookie) { _cookies = cookie; }
void IO::setFD(const int& fd) {_fd = fd;}
void IO::setEvents(const uint32_t& event) {_event = event;};
void IO::setIO(IO *io) {_io = io;};
void IO::setWs(const int& ws) {_ws = ws;}
void IO::updateTimeStamp(void) { _timestamp = std::clock(); };
void IO::updateTimeStamp(const long& timestamp) { _timestamp = timestamp; };
void IO::setServer(Server *server) { _server = server;};
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int IO::deleteAndResetIO(HttpResponse& res)
{
    close(res.getReadEnd());
    res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    return IO::IO_SUCCESS;
}

long IO::getTimestampInMillisecond(const long& timestamp) const
{
    return static_cast<long>(static_cast<double>(timestamp) / (CLOCKS_PER_SEC / 1000));
}

void IO::clear(void)
{
    _request.clear();
    _response.clear();
    _timestamp = 0;
    resetAndPreserverSomeFlags(checkBits(IO::IO_SOCKET_NOT_FINISH));
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
