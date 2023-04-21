# include "../../includes/IO/ServerStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
ServerStream::ServerStream(){};
ServerStream::ServerStream(const int& fd):IO(fd)
{
    _type = IO::VIRTUAL_SERV;
};
ServerStream::ServerStream(const ServerStream& rhs):IO(rhs._fd){};
ServerStream& ServerStream::operator=(const ServerStream& rhs)
{
    if (this == &rhs) return (*this);
    _fd = rhs._fd;
    return (*this);
};
ServerStream::~ServerStream(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void ServerStream::handleIoOperation(struct epoll_event event)
{

}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
