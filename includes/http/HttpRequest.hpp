# ifndef __HTTP_REQUEST_HPP__
# define __HTTP_REQUEST_HPP__

# include "HttpMessage.hpp"
# include "../utils/BitsManipulation.hpp"

class ClientSocketStream;
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
        HttpRequest(const HttpRequest&);
        HttpRequest& operator=(const HttpRequest&);
        ~HttpRequest();

        enum
        {
            HTTP_REQUEST_CONTENT_LENGTH,
            HTTP_REQUEST_TRANSFER_ENCODING,
            HTTP_REQUEST_FINISH_BODY,
            HTTP_REQUEST_RESPONSE_HEADER_FINISHED,
            HTTP_REQUEST_CHUNK_SET,
            HTTP_REQUEST_CHUNKED_FINISHED,
            HTTP_REQUEST_CARRIAGE_FEED,
            HTTP_REQUEST_CGI_ = 8,
            HTTP_REQUEST_STARTED,
            HTTP_REQUEST_MULTIPART_DATA,
            HTTP_REQUEST_NO_ENCODING,
            HTTP_REQUEST_DIRECTORY
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
        int fillChunkBody(Post&);
        int open_file(ClientSocketStream&);
        int open_file(ClientSocketStream&, std::string&);
        int parseRequest(ClientSocketStream&);
        void clearCurrentChunkSize(void);
        void setChunkSize(const size_t&);
        void updateCurrentChunkSize(const size_t&);
        void appendToChunkBody(const std::string&, const ssize_t&);
        void clear(void);
        void appendToBuffer(const char*, const ssize_t&);
        void setBoundary(const std::string&);
        void setCrlfBoundary(const std::string&);
        void setEndBoundary(const std::string&);
        void setCrlfEndBoundary(const std::string&);

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