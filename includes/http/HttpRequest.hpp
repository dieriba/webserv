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
            CONTENT_LENGTH,
            TRANSFER_ENCODING,
            FINISH_BODY,
            RESPONSE_HEADER_FINISHED,
            CHUNK_SET,
            CHUNKED_FINISHED,
            CARRIAGE_FEED,
            CGI_ = 8,
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