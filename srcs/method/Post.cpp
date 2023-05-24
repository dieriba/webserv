# include "../../includes/method/Post.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
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
    if (req.checkBits(HttpResponse::STARTED) == 0)
    {
        const std::map<std::string, std::string>& _map = req.getHeaders();
        const std::string& boundary = _map.find(BOUNDARY) -> second;
        std::string& s_buffer = req.getBuffer();

        std::cout << s_buffer << std::endl;
        size_t pos = s_buffer.find(boundary);

        if (pos == std::string::npos) return IO::IO_SUCCESS;

        s_buffer.erase(0, boundary.size());

        pos = s_buffer.find(CRLF);

        if (pos == std::string::npos) return IO::IO_SUCCESS;

        std::vector<std::string> vec = UtilityMethod::stringSpliter(s_buffer.substr(0, pos), ";");
        
        req.setOptions(HttpResponse::STARTED, SET);

        std::cout << s_buffer.substr(0, pos + LEN_CRLF) << std::endl;
        s_buffer.erase(0, pos + LEN_CRLF);
        exit(1);
    }
    else
    {

    }
    
    return IO::IO_SUCCESS;
}

int Post::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    if (!res.checkBits(HttpResponse::REDIRECT_SET))
    {
        if (event.getEvents() & EPOLLIN)
        {
            if (res.checkBits(HttpResponse::MULTIPART_DATA))
                handleMultipartData(req);
            if (res.checkBits(HttpResponse::NO_ENCODING))
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
