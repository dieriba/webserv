# ifndef __HTTP_REQUEST_HPP__
# define __HTTP_REQUEST_HPP__

# include "HttpMessage.hpp"
# include "../utils/BitsManipulation.hpp"

class IO;
class Post;

class HttpRequest: public HttpMessage, public BitsManipulation
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
            CHUNK_SET,
            CHUNKED_FINISHED,
            CARRIAGE_FEED,
            CGI_STREAM = 8,
            STARTED,
            MULTIPART_DATA,
            NO_ENCODING,
        };

        /*GETTERS*/
        const size_t& getHeaderSize(void) const;
        const size_t& getChunkSize(void) const;
        const size_t& getCurrentChunkSize(void) const;
        const std::string& getChunkBody(void) const;
        std::string& getChunkBody(void);
        std::ofstream& getOutfile(void);

        /*MEMBER FUNCTION*/
        int fillChunkBody(Post& post);
        int open_file(IO& event);
        int open_file(IO& event, std::string& filepath);
        int parseRequest(IO& object);
        int checkValidHeader(const int& _ws, struct epoll_event event) const;
        void clearCurrentChunkSize(void);
        void setChunkSize(const size_t& chunk_size);
        void updateCurrentChunkSize(const size_t& current_chunk_size);
        void appendToChunkBody(const std::string& chunk, const ssize_t& size);
        void clear(void);
        void appendToBuffer(const char *toAppend, const ssize_t& size);

    private:
        std::string _chunk_body;
        size_t _header_size;
        size_t _chunk_size;
        size_t _current_chunk_size;
        std::ofstream outfile;
        bool _start;
};

# endif