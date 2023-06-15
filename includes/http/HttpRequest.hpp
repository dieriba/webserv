# ifndef __HTTP_REQUEST_HPP__
# define __HTTP_REQUEST_HPP__

# include "HttpMessage.hpp"
# include "../utils/BitsManipulation.hpp"

class IO;
class Post;

typedef struct multipart
{
    std::string boundary;
    std::string end_boundary;
    std::string crlf_boundary;
    std::string crlf_end_boundary;
}   mulitipart;

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
            CGI_GET = 8,
            CGI_POST,
            STARTED,
            MULTIPART_DATA,
            NO_ENCODING,
            DIRECTORY
        };

        /*GETTERS*/
        const size_t& getHeaderSize(void) const;
        const size_t& getChunkSize(void) const;
        const size_t& getCurrentChunkSize(void) const;
        const std::string& getChunkBody(void) const;
        std::string& getChunkBody(void);
        std::ofstream& getOutfile(void);
        const std::string& getBoundary(void) const;
        const std::string& getCrlfBoundary(void) const;
        const std::string& getEndBoundary(void) const;
        const std::string& getCrlfEndBoundary(void) const;

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
        void setBoundary(const std::string& boundary);
        void setCrlfBoundary(const std::string& CrlfBoundary);
        void setEndBoundary(const std::string& Endboundary);
        void setCrlfEndBoundary(const std::string& CrlfEndboundary);

    private:
        std::string _chunk_body;
        size_t _header_size;
        size_t _chunk_size;
        size_t _current_chunk_size;
        std::ofstream outfile;
        bool _start;
        mulitipart _form_data;
};

# endif