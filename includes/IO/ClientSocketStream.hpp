# ifndef __CLIENTSOCKETSTREAM_HPP__
# define __CLIENTSOCKETSTREAM_HPP__

# include "IO.hpp"

class ClientSocketStream: public IO
{
    public:
        ClientSocketStream();
        ClientSocketStream(const int&, const int&, Server*);
        ClientSocketStream(const ClientSocketStream&);
        ClientSocketStream& operator=(const ClientSocketStream&);
        ~ClientSocketStream();

        /*GETTERS*/
        const unsigned int& getPort(void) const;
        
        int handleIoOperation(const int&, struct epoll_event&);
    private:
        int readFromSocket(const int&, struct epoll_event&);
        int writeToSocket(const int&, struct epoll_event&);
        unsigned int _port;
};

# endif