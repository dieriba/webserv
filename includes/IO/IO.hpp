# ifndef __IO_HPP__
# define __IO_HPP__

# include "../CommonLib.hpp"

class IO
{
    public:
        IO();
        IO(const int& fd);
        IO(const IO& rhs);
        IO& operator=(const IO& rhs);
        virtual ~IO();

        virtual void handleIoOperation(struct epoll_event event) = 0;
    
    /*GETTERS*/
    const int& getFd(void) const;

    /*SETTERS*/
    void setFD(const int& fd);

    enum type
    {
        VIRTUAL_SERV,
        CLIENT_SOCKET,
        CGI_PIPE
    };
    
    protected:
        int _fd;
        enum type _type;
        std::clock_t _begin;
        std::clock_t _end;
};

# endif