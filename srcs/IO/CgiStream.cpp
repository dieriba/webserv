# include "../../includes/IO/CgiStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
CgiStream::CgiStream(){};
CgiStream::CgiStream(const int& fd, Server *server):IO(fd, server)
{
    _type = IO::CGI_PIPE;
};
CgiStream::CgiStream(const CgiStream& rhs):IO(rhs){};
CgiStream& CgiStream::operator=(const CgiStream& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    return (*this);
};
CgiStream::~CgiStream(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void CgiStream::handleIoOperation(int _ws, struct epoll_event& event)
{
    (void)_ws;
    (void)event;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
