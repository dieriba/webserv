# include "../../includes/method/Post.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/IO/CgiStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Post::Post():FileWriter(HttpServer::HTTP_SERVER_POST){};
Post::Post(const Post& rhs):Method(rhs),FileWriter(rhs){};
Post& Post::operator=(const Post& rhs)
{
    if (this == &rhs) return *this;
    _request_body_size = rhs._request_body_size;
    return *this;
};
Post::~Post(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void Post::create_file(std::string& filepath)
{
    while (1)
    {
        std::string num = "_" + UtilityMethod::numberToString(_nb);

        if (access(filepath.c_str(), F_OK) == 0)
            filepath.insert(filepath.rfind('.'), num);
        else
            break ;

        FileWriter::_nb++;

        if (access(filepath.c_str(), F_OK) == 0)
        {
            filepath.erase(filepath.rfind('_'), num.size());
            filepath.insert(filepath.rfind('.'), "_" + UtilityMethod::numberToString(_nb));
            FileWriter::_nb += 5;
        }
        else
            break ;
    }
}

int Post::open_file(ClientSocketStream& client)
{
    std::map<std::string, std::string>& headers = client.getRequest().getHeaders();
    std::string& path = headers.find(PATH) -> second; 
    std::string fileExtenstion = UtilityMethod::getFileExtension(headers.find(CONTENT_TYP) -> second, 0);
    std::string filepath(headers.find(FULLPATH) -> second);
    
    /*ADD THIS INTO A TRY CATCH BLOCK*/
    if (path == (client.getServer() -> getInstance() -> getIndexPath()))
        filepath += DEFAULT_FILE_NAME + UtilityMethod::numberToString(_nb) + fileExtenstion;

    if (_outfile.is_open()) _outfile.close();
    
    _outfile.clear();

    create_file(filepath);

    client.setFilename(filepath);
    setFilename(filepath);
    _outfile.open(filepath.c_str(), std::ios::out);

    if (_outfile.fail()) return FORBIDEN;
    
    FileWriter::_nb++;

    return IO::IO_SUCCESS;
}

int Post::open_file(ClientSocketStream& client, std::string& filepath)
{
    std::string& path = client.getRequest().getHeaders().find(PATH) -> second; 
    HttpServer& instance = *(client.getServer() -> getInstance());
    /*ADD THIS INTO A TRY CATCH BLOCK*/
    std::string root_dir;

    if (instance.getUploadsFilesFolder().size() == 0)
        filepath = instance.getRootDir() + path + "/" + filepath;
    else
        filepath = instance.getUploadsFilesFolder() + "/" + filepath;

    if (_outfile.is_open()) _outfile.close();
    
    _outfile.clear();

    create_file(filepath);
    client.setFilename(filepath);
    setFilename(filepath);
    
    
    _outfile.open(filepath.c_str(), std::ios::out);

    if (_outfile.fail()) return FORBIDEN;
    
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

int Post::handleCgiPost(ClientSocketStream& event, HttpRequest& req, HttpResponse& res)
{
    const std::map<std::string, std::string>& map = req.getHeaders();

    if (map.find(CONTENT_LEN) != map.end() && (req.getBuffer().size() < req.getBodySize())) return IO::IO_SUCCESS;

    if (map.find(TRANSFERT_ENCODING) != map.end() && (req.getBuffer().find(END_CHUNK) == std::string::npos)) return IO::IO_SUCCESS;

    if (pipe(res.getPipes()) == -1) return INTERNAL_SERVER_ERROR;

    if (HttpServer::makeNonBlockingFd(res.getReadEnd()) == -1) return INTERNAL_SERVER_ERROR;
    
    if (UtilityMethod::basicCgiSetup(event, res) == IO::IO_ERROR) return IO::IO_ERROR;

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

    req.setOptions(HttpRequest::HTTP_REQUEST_FINISH_BODY, SET);

    event.setOptions(IO::IO_CGI_ON, SET);

    CgiStream& cgi = static_cast<CgiStream&>(*(event.getIO()));
        
    cgi.updateTimeStamp();

    return IO::IO_SUCCESS;
}

int Post::sendResponse(ClientSocketStream& client, HttpRequest& req, HttpResponse& res)
{
    if (res.checkBits(HttpResponse::HTTP_RESPONSE_REDIRECT_SET)) return sendRedirect(client, res, FOUND_REDIRECT_POST);

    if (client.getEvents() & EPOLLIN)
    {
        if (req.checkBits(HttpRequest::HTTP_REQUEST_CGI_))
            return handleCgiPost(client, req, res);
        if (req.checkBits(HttpRequest::HTTP_REQUEST_MULTIPART_DATA))
            return handleMultipartData(client, req);
        else
        {
            if (req.checkBits(HttpRequest::HTTP_REQUEST_CONTENT_LENGTH))
                return writeToFile(req);
            return fillChunkBody(req);
        }
    }
    else
    {
        if (UtilityMethod::sendBuffer(client.getFd(), SERVER_SUCCESS_POST_RESPONSE, UtilityMethod::myStrlen(SERVER_SUCCESS_POST_RESPONSE)) == IO::IO_ERROR)
        {
            res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
            return IO::IO_ERROR;
        }

        res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Post::clone(void) const {return new Post; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
