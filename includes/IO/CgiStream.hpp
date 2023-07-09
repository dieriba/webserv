# ifndef __CGI_STREAM_HPP__
# define __CGI_STREAM_HPP__

# include "IO.hpp"

class CgiStream: public IO
{
    public:
        CgiStream();
        CgiStream(const int&, IO*, int*);
        CgiStream(const CgiStream&);
        CgiStream& operator=(const CgiStream&);
        ~CgiStream();
        
        /*GETTERS*/
        long getCgiBeginTimestamp(void) const;
        const pid_t& getPid(void) const;

        /*SETTERS*/
        void setBeginTimeStamp(void);
        void setPid(const pid_t&);
        void setPipes(int *);

        /*MEMBER FUNCTION*/
        int resetCgi(IO&, const int&);

        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int handleIoOperation(const int&, struct epoll_event&);
    
        enum
        {
            STARTED,
        };
    
    private:
        IO *_io;
        int *_pipes;
        char _buffer[REQUEST_SIZE + 1];
        long _cgi_begin;
        pid_t _pid;
};

# endif
