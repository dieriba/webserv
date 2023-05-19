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

        enum
        {
            CONTENT_LENGTH,
            TRANSFER_ENCODING,
            FINISH_BODY,
            RESPONSE_HEADER_FINISHED,
        };

        /*GETTERS*/
        const size_t& getHeaderSize(void) const;
        std::ofstream& getOutfile(void);

        /*MEMBER FUNCTION*/
        void clear(void);
        void appendToBuffer(const char *toAppend, ssize_t size);
        int open_file(IO& event);
        int parseRequest(IO& object);
        int checkValidHeader(int _ws, struct epoll_event event) const;

    private:
        size_t _header_size;
        std::ofstream outfile;
        bool _start;
};

# endif