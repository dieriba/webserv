# ifndef __SERVERSTREAM_HPP__
# define __SERVERSTREAM_HPP__

# include "IO.hpp"

class ServerStream: public IO
{
    public:
        ServerStream();
        ServerStream(const int& fd);
        ServerStream(const ServerStream& rhs);
        ServerStream& operator=(const ServerStream& rhs);
        ~ServerStream();

        void handleIoOperation(struct epoll_event);
};

# endif