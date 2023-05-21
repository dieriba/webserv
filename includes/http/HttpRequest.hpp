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
            CHUNKED_FINISHED
        };

        /*GETTERS*/
        const size_t& getHeaderSize(void) const;
        const std::string& getChunkBody(void) const;
        std::string& getChunkBody(void);
        std::ofstream& getOutfile(void);

        /*MEMBER FUNCTION*/
        int fillChunkBody(IO& object);
        int open_file(IO& event);
        int parseRequest(IO& object);
        int checkValidHeader(const int& _ws, struct epoll_event event) const;
        void appendToChunkBody(const std::string& chunk, const ssize_t& size);
        void clear(void);
        void appendToBuffer(const char *toAppend, const ssize_t& size);

    private:
        std::string _chunk_body;
        size_t _header_size;
        std::ofstream outfile;
        bool _start;
};

# endif