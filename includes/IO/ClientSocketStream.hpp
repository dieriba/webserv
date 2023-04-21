# ifndef __CLIENTSOCKETSTREAM_HPP__
# define __CLIENTSOCKETSTREAM_HPP__

# include "IO.hpp"

class ClientSocketStream: public IO
{
    public:
        ClientSocketStream();
        ClientSocketStream(const int& fd);
        ClientSocketStream(const ClientSocketStream& rhs);
        ClientSocketStream& operator=(const ClientSocketStream& rhs);
        ~ClientSocketStream();

        void handleIoOperation(struct epoll_event);
};

# endif