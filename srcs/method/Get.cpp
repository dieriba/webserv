# include "../../includes/method/Get.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/IO.hpp"
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

int Get::handleFileRessource(IO& event, HttpRequest& req, HttpResponse& res)
{
    try
    {
        static size_t len;
        (void)req;
        char buffer[REQUEST_SIZE + 1] = {0};

        std::ifstream& file = res.getFile();
        
        file.read(buffer, REQUEST_SIZE);

        if (sendBuffer(event.getFd(), buffer, file.gcount())) return (IO::IO_ERROR);

        len += file.gcount();
        if (file.fail() || file.eof())
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
        std::cout << "Entered" << std::endl;
    }
    catch(const std::exception& e)
    {
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}

int Get::firstStep(IO& event, const HttpRequest& req, HttpResponse& res)
{
    HttpServer& instance = *(event.getServer() -> getInstance());
    std::string full_path(req.getHeaders().find(FULLPATH) -> second);
    
    bool directory = res.checkBits(HttpResponse::DIRECTORY);

    if ((directory && instance.getAutoIndexValue()) || directory == 0)
    {
        std::ifstream& file = res.getFile();

        file.open(full_path.c_str(), std::ifstream::in | std::ifstream::binary);

        if (!file) return FORBIDEN;
        
        file.seekg(0, std::ios::end);
        res.setBodySize(file.tellg());
        file.seekg(0, std::ios::beg);

        if (file.fail()) return INTERNAL_SERVER_ERROR;

        makeStatusLine(OK);

        std::string ressource(full_path);
        
        appendToResponse(CONTENT_TYP, UtilityMethod::getMimeType(ressource, instance.getFullIndexPath(), instance.getIndex(), true));
        appendToResponse(CONTENT_LEN, UtilityMethod::numberToString(res.getBodySize()));
        _response += CRLF;
        
        res.setOptions(HttpResponse::FILE, SET);
    }
    else if (directory)
    {
        if (pipe(res.getPipes()) == -1) return INTERNAL_SERVER_ERROR;

        if (HttpServer::makeNonBlockingFd(res.getReadEnd()) == -1) return INTERNAL_SERVER_ERROR;

        if (access(PATH_TO_DIRECTORY_LISTING_SCRIPT, F_OK) != 0) return NOT_FOUND;
        
        if ((access(PATH_TO_DIRECTORY_LISTING_SCRIPT, X_OK | R_OK) != 0)) return FORBIDEN;
        
        makeStatusLine(OK);
        appendToResponse(CONTENT_TYP, MIME_HTML);
        appendToResponse(TRANSFERT_ENCODING, "chunked");
        _response += CRLF;
        
        pid_t pid = fork();

        if (pid == -1) return INTERNAL_SERVER_ERROR;

        if (pid == 0)
        {
            close(res.getReadEnd());

            if (dup2(res.getWriteEnd(), STDOUT_FILENO) == -1)
            {
                write(res.getWriteEnd(), SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR, UtilityMethod::myStrlen(SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR));   
                close(res.getWriteEnd());
                exit(1);
            }
            
            close(res.getWriteEnd());

            const std::string& path(PATH"=" + req.getHeaders().find(PATH)-> second);
            std::string root(ROOT"=" + instance.getRootDir() + req.getHeaders().find(PATH)-> second);

            char *envp[] = {(char *)root.c_str(), (char *)path.c_str(), NULL};

            char *argv[] = {NULL};

            execve(PATH_TO_DIRECTORY_LISTING_SCRIPT, argv, envp);
            
            exit(1);
        }

        close(res.getWriteEnd());
    }

    if (sendBuffer(event.getFd(), _response.c_str(), _response.size())) return (IO::IO_ERROR);

    _response.clear();
    
    res.setOptions(HttpResponse::STARTED, SET);
    
    return IO::IO_SUCCESS;
}

int Get::handleDirectoryRessource(IO& event, const HttpRequest& req, HttpResponse& res)
{
    static int i;
    (void)event;
    (void)req;
    char buffer[REQUEST_SIZE + 1] = {0};
    
    int bytes = read(res.getReadEnd(), buffer, REQUEST_SIZE);
    
    if (bytes > 0)
        std::cout << bytes << std::endl;
    i++;
    return (0);
}

int Get::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    if (res.checkBits(HttpResponse::REDIRECT_SET)) return sendRedirect(event, res, FOUND_REDIRECT);

    if (!res.checkBits(HttpResponse::STARTED))
    {
        int err = firstStep(event, req, res);
        if (err) return err;
    }

    if (res.checkBits(HttpResponse::FILE)) return handleFileRessource(event, req, res);
    
    if (res.checkBits(HttpResponse::DIRECTORY)) return handleDirectoryRessource(event, req, res);

    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
