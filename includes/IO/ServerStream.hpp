# ifndef __SERVERSTREAM_HPP__
# define __SERVERSTREAM_HPP__

# include "IO.hpp"

class ServerStream: public IO
{
    public:
        ServerStream();
        ServerStream(const int& fd, Server *server);
        ServerStream(const ServerStream& rhs);
        ServerStream& operator=(const ServerStream& rhs);
        ~ServerStream();

        int handleIoOperation(const int& _ws, struct epoll_event&);
};

# endif