# ifndef __CLIENTSOCKETSTREAM_HPP__
# define __CLIENTSOCKETSTREAM_HPP__

# include "IO.hpp"
# include "../server/Server.hpp"

class ClientSocketStream: public IO
{
    public:
        ClientSocketStream();
        ClientSocketStream(const int&, const int&, Server*, Server*);
        ClientSocketStream(const ClientSocketStream&);
        ClientSocketStream& operator=(const ClientSocketStream&);
        ~ClientSocketStream();

        /*GETTERS*/
        const unsigned int& getPort(void) const;
        const unsigned int& getPrevContentLength(void) const;
        const unsigned int& getPrevBodySize(void) const;
        
        /*SETTERS*/
        void setPrevContentLength(const unsigned int&);
        void updatePrevBodySize(const unsigned int&);

        int handleIoOperation(const int&, struct epoll_event&);
    private:
        int readFromSocket(const int&, struct epoll_event&);
        int writeToSocket(const int&, struct epoll_event&);
        unsigned int _port;
        unsigned int _prev_body_size;
        unsigned int _prev_content_len;
};

# endif