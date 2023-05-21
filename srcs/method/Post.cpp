# include "../../includes/method/Post.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Post::Post():_request_body_size(0){};
Post::Post(const Post& rhs):Method(rhs),_request_body_size(rhs._request_body_size){};
Post& Post::operator=(const Post& rhs)
{
    if (this == &rhs) return *this;
    _request_body_size = rhs._request_body_size;
    return *this;
};
Post::~Post(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const size_t& Post::getRequestBodySize(void) const {return _request_body_size;}
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
void Post::updateSize(const size_t& size)
{
    _request_body_size += size;
};
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void Post::clearRequestBodySize(void)
{
    _request_body_size = 0;
}

int Post::writeToFile(IO& object, HttpRequest& req)
{
    std::ofstream& outfile = req.getOutfile();
    try
    {
        const TcpServer& instance = *(object.getServer() -> getInstance());
        bool content_len = object.checkBits(HttpRequest::CONTENT_LENGTH);
        bool end_chunk = object.checkBits(HttpRequest::CHUNKED_FINISHED);
        std::string& alias = content_len ? req.getBuffer() : req.getChunkBody();
        
        if (!content_len && (alias.size() + getRequestBodySize()) > instance.getBodySize())
            return TOO_LARGE_CONTENT;
        outfile.write(alias.data(), alias.size());

        updateSize(alias.size());
        if ((content_len && getRequestBodySize() >= req.getBodySize()) || (!content_len && end_chunk))
        {
            object.setOptions(HttpRequest::FINISH_BODY, SET);
            clearRequestBodySize();
            outfile.close();
        }
        alias.clear();
    }
    catch(const std::exception& e)
    {
        outfile.close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}

int Post::handleMultipartData(HttpRequest& req)
{
    static int _nb;
    const std::map<std::string, std::string>& _map = req.getHeaders();
    std::string& s_buffer = req.getBuffer();

    while (1)
    {
        size_t pos = s_buffer.find(_map.find(BOUNDARY CRLF) -> second);
        if (pos != std::string::npos)
        {
            std::cout << s_buffer;
            //std::cout << pos << std::endl;
            std::vector<std::string> form_header = UtilityMethod::stringSpliter(s_buffer.substr(pos, s_buffer.find(CRLF CRLF)), CRLF);
            
            /*for (size_t i = 0; i < form_header.size(); i++)
            {
                std::cout << form_header[i] << std::endl;
            }*/
            
            s_buffer.erase(pos, s_buffer.find(CRLF CRLF) + 4);
        }

        /*pos = s_buffer.find(_map.find(END_BOUNDARY CRLF) -> second);
        if (pos == 0) break ;*/
    }

    _nb++;

    return IO::IO_SUCCESS;
}

int Post::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    if (event.getEvents() & EPOLLIN)
    {
        (void)req;
        if (res.checkBits(HttpResponse::MULTIPART_DATA))
        {
            std::cout << req.getBuffer().find(req.getHeaders().find(END_BOUNDARY) -> second) << std::endl;
            handleMultipartData(req);
        }
        else if (res.checkBits(HttpResponse::NO_ENCODING))
            return writeToFile(event, req);
    }
    else
    {
        std::cout << "Entered OUT" << std::endl;
        if (sendBuffer(event.getFd(), SERVER_SUCCESS_POST_RESPONSE, strlen(SERVER_SUCCESS_POST_RESPONSE)))
        {
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
            return IO::IO_ERROR;
        }
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
