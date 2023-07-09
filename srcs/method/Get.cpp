# include "../../includes/method/Get.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/IO/CgiStream.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/server/HttpServer.hpp"


/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Get::Get(){};
Get::Get(const Get& rhs):Method(rhs){};
Get& Get::operator=(const Get& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Get::~Get(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/


/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

int Get::getCgiHandler(ClientSocketStream& client , const HttpRequest&  req, HttpResponse& res)
{

    if (UtilityMethod::basicCgiSetup(client, res) == IO::IO_ERROR) return IO::IO_ERROR;
    CgiStream& cgi = static_cast<CgiStream&>(*(client.getIO()));

    cgi.setPid(fork());

    if (cgi.getPid() == -1) return INTERNAL_SERVER_ERROR;
    
    if (cgi.getPid() == 0)
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
        const std::string& query_string = "QUERY_STRING=" + path.substr(path.find('?') + 1, std::string::npos);
        
        const char *executable = map.find(CGI_EXECUTABLE) -> second.c_str();

        res.clearWriteEnd();

        std::string method = "REQUEST_METHOD=GET";

        char *envp[] = {(char *)query_string.c_str(), (char *)method.c_str() ,NULL};

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

int Get::directoryCgi(ClientSocketStream& client, const HttpRequest& req, HttpResponse& res)
{
    if (access(PATH_TO_DIRECTORY_LISTING_SCRIPT, F_OK) != 0) return NOT_FOUND;
        
    if ((access(PATH_TO_DIRECTORY_LISTING_SCRIPT, X_OK | R_OK) != 0)) return FORBIDEN;

    if (UtilityMethod::basicCgiSetup(client, res) == IO::IO_ERROR) return IO::IO_ERROR;

    HttpServer& instance = *(client.getServer() -> getInstance());
    CgiStream& cgi = static_cast<CgiStream&>(*(client.getIO()));
    
    cgi.setPid(fork());

    if (cgi.getPid() == -1) return INTERNAL_SERVER_ERROR;

    if (cgi.getPid() == 0)
    {
        close(res.getReadEnd());

        if (dup2(res.getWriteEnd(), STDOUT_FILENO) == IO::IO_ERROR)
        {
            write(res.getWriteEnd(), SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR, UtilityMethod::myStrlen(SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR));   
            close(res.getWriteEnd());
            exit(EXIT_FAILURE);
        }
            
        close(res.getWriteEnd());

        const std::string& path(PATH"=" + req.getHeaders().find(PATH)-> second);
        std::string root(ROOT"=" + instance.getRootDir() + req.getHeaders().find(PATH)-> second);

        char *envp[] = {(char *)root.c_str(), (char *)path.c_str(), NULL};

        char *argv[] = {NULL};

        execve(PATH_TO_DIRECTORY_LISTING_SCRIPT, argv, envp);
            
        exit(EXIT_FAILURE);
    }

    return IO::IO_SUCCESS;
}

int Get::firstStep(ClientSocketStream& client, const HttpRequest& req, HttpResponse& res)
{
    HttpServer& instance = *(client.getServer() -> getInstance());
    std::string full_path(req.getHeaders().find(FULLPATH) -> second);
    
    bool directory = req.checkBits(HttpRequest::HTTP_REQUEST_DIRECTORY);
    bool cgi_get = req.checkBits(HttpRequest::HTTP_REQUEST_CGI_);

    if (cgi_get == false && ((directory == true && instance.checkBits(HttpServer::HTTP_SERVER_AUTO_INDEX_) > 0) || directory == 0))
    {
        std::ifstream& file = res.getFile();

        file.open(full_path.c_str(), std::ifstream::in | std::ifstream::binary);

        if (!file) return FORBIDEN;
        
        file.seekg(0, std::ios::end);
        res.setBodySize(file.tellg());
        file.seekg(0, std::ios::beg);

        if (file.fail()) return INTERNAL_SERVER_ERROR;

        makeStatusLine(client, OK);
        addCustomHeader(instance);

        std::string ressource(full_path);
        
        appendToResponse(CONTENT_TYP, UtilityMethod::getMimeType(ressource, instance.getFullIndexPath(), instance.getIndex(), true));
        appendToResponse(CONTENT_LEN, UtilityMethod::numberToString(res.getBodySize()));
        _response += CRLF;
        
        res.setOptions(HttpResponse::HTTP_RESPONSE_FILE, SET);
    
        res.setOptions(HttpResponse::HTTP_RESPONSE_STARTED, SET);
    }
    else if (directory == true || cgi_get == true)
    {
        if (pipe(res.getPipes()) == -1) return INTERNAL_SERVER_ERROR;

        if (HttpServer::makeNonBlockingFd(res.getReadEnd()) == -1) return INTERNAL_SERVER_ERROR;

        int resp = 0;

        if (directory)
            resp = directoryCgi(client, req, res);
        else
            resp = getCgiHandler(client, req, res);

        res.clearWriteEnd();
        
        if (resp > 0)
        {
            UtilityMethod::deleteEventFromEpollInstance(client.getWs(), res.getReadEnd());
            res.clearReadEnd();
            return resp; 
        }

        client.setOptions(IO::IO_CGI_ON, SET);
        
        CgiStream& cgi = static_cast<CgiStream&>(*(client.getIO()));
        
        cgi.updateTimeStamp();
    }
    
    if (UtilityMethod::sendBuffer(client.getFd(), _response.c_str(), _response.size()) == IO::IO_ERROR) return (IO::IO_ERROR);
    
    _response.clear();
    
    return IO::IO_SUCCESS;
}

int Get::sendResponse(ClientSocketStream& client, HttpRequest& req, HttpResponse& res)
{
    if (res.checkBits(HttpResponse::HTTP_RESPONSE_REDIRECT_SET)) return sendRedirect(client, res, FOUND_REDIRECT);

    if (!res.checkBits(HttpResponse::HTTP_RESPONSE_STARTED))
    {
        int err = firstStep(client, req, res);

        if (err) return err;
    }

    if (res.checkBits(HttpResponse::HTTP_RESPONSE_FILE)) return handleFileRessource(client, res);
    
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Get::clone(void) const {return new Get; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
