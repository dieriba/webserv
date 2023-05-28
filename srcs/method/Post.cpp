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

int Post::writeToFileMutltipartData(IO& event, HttpRequest& req, const size_t& size)
{
    std::ofstream& outfile = req.getOutfile();
    std::string& s_buffer = req.getBuffer();
    
    try
    {
        (void)event;
        outfile.write(s_buffer.data(), size);
    }
    catch(const std::exception& e)
    {
        outfile.close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
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

int Post::handleMultipartData(IO& event, HttpRequest& req)
{
    const std::map<std::string, std::string>& _map = req.getHeaders();
    const std::string& boundary = _map.find(BOUNDARY) -> second;
    const std::string& end_boundary = req.getHeaders().find(END_BOUNDARY) -> second;
    std::string& s_buffer = req.getBuffer();

    while (1)
    {
        if (req.checkBits(HttpResponse::STARTED) == 0)
        {
            std::cout << "Entered " << std::endl;
            size_t start = 0;

            if (event.checkBits(HttpRequest::CARRIAGE_FEED))
            {
                size_t pos = s_buffer.find_first_not_of(CRLF);

                if (pos == std::string::npos || pos != LEN_CRLF) return BAD_REQUEST;

                start = pos;
                event.setOptions(HttpRequest::CARRIAGE_FEED, CLEAR);
            }

            size_t pos_one = s_buffer.find(CRLF, start);

            if (pos_one == std::string::npos) return (IO::IO_SUCCESS);

            if (s_buffer.find(boundary, start) != start) return BAD_REQUEST;
            
            size_t pos_two = s_buffer.find(CRLF, start + boundary.size() + LEN_CRLF);

            if (pos_two == std::string::npos) return (IO::IO_SUCCESS);

            std::string content_disp = s_buffer.substr(start + boundary.size() + LEN_CRLF, pos_two - (start + boundary.size() + LEN_CRLF));

            std::vector<std::string> vec = UtilityMethod::stringSpliter(content_disp, ";");

            if (vec.size() < 2 || vec.size() > 3) return BAD_REQUEST;

            if (vec.size() == 3)
            {
                size_t pos_three = s_buffer.find(CRLF, pos_two + LEN_CRLF);
                
                std::vector<std::string> vector_filename = UtilityMethod::stringSpliter(*(vec.rbegin()), "=");
                
                if (vector_filename.size() != 2) return BAD_REQUEST;

                std::string content = s_buffer.substr(pos_two + LEN_CRLF, pos_three - (pos_two + LEN_CRLF));
                
                std::vector<std::string> vec_content_type = UtilityMethod::stringSpliter(content, ": ");
                
                if (vec_content_type.size() != 2) return BAD_REQUEST;

                vector_filename[1].erase(vector_filename[1].size() - 1);
                vector_filename[1].erase(vector_filename[1].begin());
                
                if (UtilityMethod::getFileExtension(vector_filename[1] ,1)
                    != UtilityMethod::getFileExtension(vec_content_type[1] ,0)) return BAD_REQUEST;
                
                if (vector_filename[1].size()) req.open_file(event, vector_filename[1]);

                pos_two = pos_three;
            }

            updateSize(pos_two + LEN_CRLF + LEN_CRLF);

            s_buffer.erase(0, pos_two + LEN_CRLF + LEN_CRLF);
            
            req.setOptions(HttpResponse::STARTED, SET);
        }
  
        if (req.checkBits(HttpResponse::STARTED))
        {
            size_t boundary_br = s_buffer.find(CRLF + boundary);
            size_t size = boundary_br;
            size_t pos_crlf = 0;

            if (size == std::string::npos)
            {
                while ((pos_crlf = s_buffer.find('\r', pos_crlf)) != std::string::npos)
                {
                    if ((pos_crlf + (UtilityMethod::myStrlen(CRLF) + boundary.size()) - 1) > s_buffer.size())
                        break;
                    pos_crlf++;
                }
                if (pos_crlf == std::string::npos)
                    size = s_buffer.size();
                else
                    size = pos_crlf;
            }
            else
                event.setOptions(HttpRequest::CARRIAGE_FEED, SET);
            
            if (req.getOutfile().is_open() && size > 0)
            {
                int res = writeToFileMutltipartData(event, req, size);

                if (res) return res;
            }
            
            if (size)
            {
                updateSize(size);
                s_buffer.erase(0, size);
            }

            if (s_buffer.size() == 0 || (pos_crlf != std::string::npos && boundary_br == std::string::npos))
                break;
            
            if (req.getOutfile().is_open())
            {
                req.getOutfile().clear();
                req.getOutfile().close();
            }

            if (s_buffer.find(CRLF + end_boundary) == 0)
            {
                updateSize(end_boundary.size() + LEN_CRLF + LEN_CRLF);
                std::cout << "request body: " << _request_body_size << std::endl;
                req.resetOptions();
                event.setOptions(HttpRequest::FINISH_BODY, SET);
                break;
            }
            req.setOptions(HttpResponse::STARTED, CLEAR);

        }
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
                return handleMultipartData(event, req);
            else
            {
                if (event.checkBits(HttpRequest::CONTENT_LENGTH))
                    return writeToFile(event, req);
                return req.fillChunkBody(event, *this);
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
