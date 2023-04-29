# ifndef __HTTP_REQUEST_HPP__
# define __HTTP_REQUEST_HPP__

# include "HttpMessage.hpp"

class IO;

class HttpRequest: public HttpMessage
{
    public:
        HttpRequest();
        HttpRequest(const HttpRequest& rhs);
        HttpRequest& operator=(const HttpRequest& rhs);
        ~HttpRequest();

        /*MEMBER FUNCTION*/
        int parseRequest(IO& object);
        int checkValidHeader(int _ws, struct epoll_event event) const;
};

# endif