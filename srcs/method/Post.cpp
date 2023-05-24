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

int Post::writeToFile(IO& object, HttpRequest& req, const size_t& bytes)
{
    std::ofstream& outfile = req.getOutfile();
    try
    {
        std::string& alias = req.getBuffer();
        
        if (bytes)
        {
            outfile.write(alias.data(), bytes);
            updateSize(bytes);
        }
        else
        {
            std::cout << _request_body_size << std::endl;
            object.setOptions(HttpRequest::FINISH_BODY, SET);
            clearRequestBodySize();
            outfile.close();
        }
    }
    catch(const std::exception& e)
    {
        outfile.close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}


int Post::writeToFile(IO& object, HttpRequest& req)
{
    std::ofstream& outfile = req.getOutfile();
    try
    {
        std::string& alias = req.getBuffer();
        
        outfile.write(alias.data(), alias.size());
        updateSize(alias.size());
        if (getRequestBodySize() >= req.getBodySize())
        {
            std::cout << _request_body_size << std::endl;
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
    if (!res.checkBits(HttpResponse::REDIRECT_SET))
    {
        if (event.getEvents() & EPOLLIN)
        {
            if (res.checkBits(HttpResponse::MULTIPART_DATA))
            {
                //std::cout << req.getBuffer().find(req.getHeaders().find(END_BOUNDARY) -> second) << std::endl;
                exit(1);
                handleMultipartData(req);
            }
            else if (res.checkBits(HttpResponse::NO_ENCODING))
            {
                if (event.checkBits(HttpRequest::CONTENT_LENGTH))
                    return writeToFile(event, req);
                req.fillChunkBody(event, *this);
            }
        }
        else
        {
            if (sendBuffer(event.getFd(), SERVER_SUCCESS_POST_RESPONSE, UtilityMethod::myStrlen(SERVER_SUCCESS_POST_RESPONSE)))
            {
                res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
                return IO::IO_ERROR;
            }
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
        }
    }
    else
    {
        if (sendBuffer(event.getFd(), FOUND_REDIRECT_POST, UtilityMethod::myStrlen(FOUND_REDIRECT_POST)))
            return IO::IO_ERROR;
        const std::string& link = event.getServer() -> getInstance() -> getRedirect();
        
        if (sendBuffer(event.getFd(), link.c_str(), link.size()) || sendBuffer(event.getFd(), CRLF CRLF, UtilityMethod::myStrlen(CRLF CRLF)))
            return IO::IO_ERROR;
            
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
