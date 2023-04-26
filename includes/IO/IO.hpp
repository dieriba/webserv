# ifndef __IO_HPP__
# define __IO_HPP__

# include "../CommonLib.hpp"
# include "../Server.hpp"
# include "../http/HttpRequest.hpp"
# include "../http/HttpResponse.hpp"

class IO
{
    public:
        IO();
        IO(const int& fd, Server *server);
        IO(const IO& rhs);
        IO& operator=(const IO& rhs);
        virtual ~IO();

        virtual void handleIoOperation(int _ws, struct epoll_event event) = 0;
    
    /*GETTERS*/
    const int& getFd(void) const;
    Server* getServer(void) const;

    /*SETTERS*/
    void setFD(const int& fd);

    /*MEMBER FUNCTION*/
    bool validSocketClient(int _fd, struct epoll_event event);

    enum type
    {
        VIRTUAL_SERV,
        CLIENT_SOCKET,
        CGI_PIPE
    };
    
    protected:
        int _fd;
        Server *_server;
        HttpRequest _request;
        HttpResponse _response;
        enum type _type;
        std::clock_t _begin;
        std::clock_t _end;
};

# endif