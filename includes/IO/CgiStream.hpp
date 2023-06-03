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
        const int& getBytes(void) const;
        const char* getBuffer(void) const;

        /*SETTERS*/
        void setPipes(int *pipes);
        void setBytes(const int& bytes);
        enum
        {
            NO_DATA_AVAILABLE = -2,
        };

        int handleIoOperation(const int& _ws, struct epoll_event&);
    private:
        IO *_io;
        int *_pipes;
        int _bytes;
        char _buffer[REQUEST_SIZE + 1];

        int resetCgi(IO* object, const int& _ws);
};

# endif
