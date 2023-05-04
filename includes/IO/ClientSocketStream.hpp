# ifndef __CLIENTSOCKETSTREAM_HPP__
# define __CLIENTSOCKETSTREAM_HPP__

# include "IO.hpp"

class ClientSocketStream: public IO
{
    public:
        ClientSocketStream();
        ClientSocketStream(const int& fd, Server* server);
        ClientSocketStream(const ClientSocketStream& rhs);
        ClientSocketStream& operator=(const ClientSocketStream& rhs);
        ~ClientSocketStream();

        void handleIoOperation(const int& _ws, struct epoll_event&);
    private:
        void readFromSocket(const int& _ws, struct epoll_event& event, IO *_ev);
        void writeToSocket(const int& _ws, struct epoll_event& event, IO& _ev);
};

# endif