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

        /*GETTERS*/
        const size_t& getHeaderSize(void) const;
        const size_t& getRequestBodySize(void) const;

        /*SETTERS*/
        void updateSize();
        void clearRequestBodySize(void);
        
        /*MEMBER FUNCTION*/
        int open_file(void);
        int handlePostMethod(IO& object);
        void appendToBuffer(const char *toAppend, ssize_t size);
        int parseRequest(IO& object);
        int checkValidHeader(int _ws, struct epoll_event event) const;

    private:
        size_t _header_size;
        size_t _request_body_size;
        std::ofstream outfile;
        bool _start;
};

# endif