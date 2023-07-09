# ifndef __SERVERSTREAM_HPP__
# define __SERVERSTREAM_HPP__

# include "IO.hpp"

class ServerStream: public IO
{
    public:
        ServerStream();
        ServerStream(const int&, Server*);
        ServerStream(const ServerStream&);
        ServerStream& operator=(const ServerStream&);
        ~ServerStream();

        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int handleIoOperation(const int&, struct epoll_event&);
};

# endif