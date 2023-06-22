# ifndef __CGI_STREAM_HPP__
# define __CGI_STREAM_HPP__

# include "IO.hpp"

class CgiStream: public IO
{
    public:
        CgiStream();
        CgiStream(const int& fd, IO *event, int *pipes);
        CgiStream(const CgiStream& rhs);
        CgiStream& operator=(const CgiStream& rhs);
        ~CgiStream();
        
        /*GETTERS*/
        long getCgiTimeStamp(void) const;
        long getCgiBeginTimestamp(void) const;
        const pid_t& getPid(void) const;

        /*SETTERS*/
        void setBeginTimeStamp(void);
        void setPid(const pid_t& pid);
        void updateCgiTimeStamp(void);
        void setPipes(int *pipes);

        /*MEMBER FUNCTION*/
        int resetCgi(IO& object, const int& _ws);
        enum
        {
            STARTED,
        };

        int handleIoOperation(const int& _ws, struct epoll_event&);
    private:
        IO *_io;
        int *_pipes;
        char _buffer[REQUEST_SIZE + 1];
        long _cgi_timestamp;
        long _cgi_begin;
        pid_t _pid;
};

# endif
