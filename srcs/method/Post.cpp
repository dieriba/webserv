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

int Post::writeToFileMutltipartData(HttpRequest& req, const size_t& size)
{
    std::ofstream& outfile = req.getOutfile();
    
    try
    {
        outfile.write(req.getBuffer().data(), size);
    }
    catch(const std::exception& e)
    {
        outfile.close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}

int Post::writeToFile(HttpRequest& req, const size_t& pos, const size_t& bytes)
{
    try
    {
        std::string& alias = req.getBuffer();
        if (bytes)
        {
            const char *buffer = alias.data();
            req.getOutfile().write(&buffer[pos], bytes);
            updateSize(bytes);
        }
    }
    catch(const std::exception& e)
    {
        req.getOutfile().close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}


int Post::writeToFile(HttpRequest& req)
{
    try
    {
        std::string& alias = req.getBuffer();
        
        req.getOutfile().write(alias.data(), alias.size());
        updateSize(alias.size());
        if (getRequestBodySize() >= req.getBodySize())
        {
            std::cout << _request_body_size << std::endl;
            req.setOptions(HttpRequest::FINISH_BODY, SET);
            clearRequestBodySize();
            req.getOutfile().close();
        }
        alias.clear();
    }
    catch(const std::exception& e)
    {
        req.getOutfile().close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}

int Post::handleMultipartData(IO& event, HttpRequest& req)
{
    while (1)
    {
        if (req.checkBits(HttpRequest::STARTED) == 0)
        {
            size_t start = 0;
            std::string& s_buffer = req.getBuffer();
            const std::string& boundary = req.getBoundary();
            
            if (req.checkBits(HttpRequest::CARRIAGE_FEED))
            {
                size_t pos = s_buffer.find_first_not_of(CRLF);

                if (pos == std::string::npos || pos != LEN_CRLF) return BAD_REQUEST;

                start = pos;
                req.setOptions(HttpRequest::CARRIAGE_FEED, CLEAR);
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
            
            req.setOptions(HttpRequest::STARTED, SET);
        }
  
        if (req.checkBits(HttpRequest::STARTED))
        {
            size_t boundary_br = req.getBuffer().find(req.getCrlfBoundary());
            size_t size = boundary_br;
            size_t pos_crlf = 0;

            if (size == std::string::npos)
            {
                pos_crlf = req.getBuffer().rfind('\r');
                if (pos_crlf == std::string::npos || (pos_crlf + (req.getCrlfBoundary().size()) - 1) <= req.getBuffer().size())
                    size = req.getBuffer().size();
                else
                    size = pos_crlf;
            }
            else
                req.setOptions(HttpRequest::CARRIAGE_FEED, SET);
            
            if (req.getOutfile().is_open() && size > 0)
            {
                int res = writeToFileMutltipartData(req, size);

                if (res) return res;
            }
            
            if (size)
            {
                updateSize(size);
                if (size == req.getBuffer().size())
                    req.getBuffer().clear();
                else
                    req.getBuffer().erase(0, size);
            }

            if (req.getBuffer().size() == 0 || (pos_crlf != std::string::npos && boundary_br == std::string::npos))
                break;
            
            if (req.getOutfile().is_open())
            {
                req.getOutfile().clear();
                req.getOutfile().close();
            }

            if (req.getBuffer().find(req.getCrlfEndBoundary()) == 0)
            {
                updateSize(req.getEndBoundary().size() + LEN_CRLF + LEN_CRLF);
                std::cout << "request body: " << _request_body_size << std::endl;
                req.setOptions(HttpRequest::FINISH_BODY, SET);
                break;
            }

            req.setOptions(HttpRequest::STARTED, CLEAR);
        }
    }
    return IO::IO_SUCCESS;
}

int Post::postCgiHandler(HttpRequest& req, HttpResponse& res)
{
    pid_t pid = fork();

    if (pid == -1) return INTERNAL_SERVER_ERROR;
    
    if (pid == 0)
    {
        res.clearReadEnd();

        if (dup2(res.getWriteEnd(), STDOUT_FILENO) == IO::IO_ERROR)
        {
            write(res.getWriteEnd(), SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR, UtilityMethod::myStrlen(SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR));   
            close(res.getWriteEnd());
            exit(EXIT_FAILURE);
        }
        
        const std::map<std::string, std::string>& map = req.getHeaders();
        
        std::string path = map.find(PATH) -> second;
        
        const char *executable = map.find(CGI_EXECUTABLE) -> second.c_str();

        res.clearWriteEnd();

        std::string method = "REQUEST_METHOD=POST";

        req.getBuffer().insert(0, "BODY=");

        char *envp[] = {(char *)req.getBuffer().c_str(), (char *)method.c_str() ,NULL};

        char *argv[] = {(char *)executable, (char *)map.find(CGI_ARGS)->second.c_str(),  NULL};

        if (access(executable, F_OK) != 0)
        {
            write(res.getWriteEnd(), SERVER_ERROR_PAGE_NOT_FOUND, UtilityMethod::myStrlen(SERVER_ERROR_PAGE_NOT_FOUND));
            exit(EXIT_FAILURE);
        };

        if ((access(executable, X_OK) != 0))
        {
            write(res.getWriteEnd(), SERVER_ERROR_PAGE_FORBIDDEN, UtilityMethod::myStrlen(SERVER_ERROR_PAGE_FORBIDDEN));
            exit(EXIT_FAILURE);
        }

        execve(executable, argv, envp);
    
        write(res.getWriteEnd(), SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR, UtilityMethod::myStrlen(SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR));   

        exit(EXIT_FAILURE);
    }

    return IO::IO_SUCCESS;
}

int Post::handleCgiPost(IO& event, HttpRequest& req, HttpResponse& res)
{
    const std::map<std::string, std::string>& map = req.getHeaders();

    if (map.find(CONTENT_LEN) != map.end())
        std::cout << "Buffer size: " << req.getBuffer().size() << " Body size: " << req.getBodySize() << std::endl;

    if (map.find(CONTENT_LEN) != map.end() && (req.getBuffer().size() < req.getBodySize())) return IO::IO_SUCCESS;

    if (map.find(TRANSFERT_ENCODING) != map.end())
        std::cout << "END_CHUNK: " << req.getBuffer().find(END_CHUNK) << std::endl;

    if (map.find(TRANSFERT_ENCODING) != map.end() && (req.getBuffer().find(END_CHUNK) == std::string::npos)) return IO::IO_SUCCESS;

    if (pipe(res.getPipes()) == -1) return INTERNAL_SERVER_ERROR;

    if (HttpServer::makeNonBlockingFd(res.getReadEnd()) == -1) return INTERNAL_SERVER_ERROR;
    
    if (UtilityMethod::basicCgiSetup(event, res, *this, "text/plain") == IO::IO_ERROR) return IO::IO_ERROR;

    int resp = postCgiHandler(req, res);

    res.clearWriteEnd();

    if (resp > 0)
    {
        UtilityMethod::deleteEventFromEpollInstance(event.getWs(), res.getReadEnd());
        res.clearReadEnd();
        return resp; 
    }

    if (UtilityMethod::sendBuffer(event.getFd(), _response.c_str(), _response.size()) == IO::IO_ERROR) return (IO::IO_ERROR);
    
    _response.clear();

    req.setOptions(HttpRequest::FINISH_BODY, SET);

    event.setOptions(IO::CGI_ON, SET);

    return IO::IO_SUCCESS;
}

int Post::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    if (res.checkBits(HttpResponse::REDIRECT_SET)) return sendRedirect(event, res, FOUND_REDIRECT_POST);

    if (event.getEvents() & EPOLLIN)
    {
        if (req.checkBits(HttpRequest::CGI_))
            return handleCgiPost(event, req, res);
        if (req.checkBits(HttpRequest::MULTIPART_DATA))
            return handleMultipartData(event, req);
        else
        {
            if (req.checkBits(HttpRequest::CONTENT_LENGTH))
                return writeToFile(req);
            return req.fillChunkBody(*this);
        }
    }
    else
    {
        if (UtilityMethod::sendBuffer(event.getFd(), SERVER_SUCCESS_POST_RESPONSE, UtilityMethod::myStrlen(SERVER_SUCCESS_POST_RESPONSE)) == IO::IO_ERROR)
        {
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
            return IO::IO_ERROR;
        }
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Post::clone(void) const {return new Post; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
