# ifndef __CGISTREAM_HPP__
# define __CGISTREAM_HPP__

# include "IO.hpp"

class CgiStream: public IO
{
    public:
        CgiStream();
        CgiStream(const int& fd);
        CgiStream(const CgiStream& rhs);
        CgiStream& operator=(const CgiStream& rhs);
        ~CgiStream();

        void handleIoOperation(struct epoll_event);
};

# endif