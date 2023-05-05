# include "../../includes/method/Get.hpp"
# include "../../includes/http/HttpRequest.hpp"
<<<<<<< HEAD
# include "../../includes/http/HttpResponse.hpp"
=======
>>>>>>> origin/main
# include "../../includes/IO/IO.hpp"
# include "../../includes/utilityMethod.hpp"
# include "../../includes/TcpServer.hpp"

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
<<<<<<< HEAD

int Get::handleChunkedRequest(HttpResponse& res, std::string& s_buffer)
{
    _response = utilityMethod::decimalToHex(s_buffer.size()) + CRLF + s_buffer + CRLF;

    if (res.getFile().eof() && res.getFile().fail())
    {
        _response += "0";
        _response += CRLF CRLF;
        res.resetOptions();
        res.getFile().close();
        res.getFile().clear();
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    //std::cout << _response;
    return (0);
}

int Get::handleFileRessource(IO& event, const HttpRequest& req, HttpResponse& res)
{
    try
    {
        TcpServer& instance = *(event.getServer() -> getInstance());
        bool f_read = res.checkBits(HttpResponse::FIRST_READ);
        (void)req;
        std::string s_buffer;
        std::string ressource(res.getPath());

        char buffer[REQUEST_SIZE + 1] = {0};
        
        res.getFile().read(buffer, REQUEST_SIZE);
        s_buffer = buffer;
        
        /**/
        appendToResponse(CONTENT_TYP, utilityMethod::getMimeType(ressource, instance.getFullIndexPath(), instance.getIndex()));
        if (f_read && (res.getFile().eof() && res.getFile().fail()))
        {
            appendToResponse(CONTENT_LEN, utilityMethod::numberToString(s_buffer.size()));
            _response += CRLF;
            _response += s_buffer;
            send(event.getFd(), _response.c_str(), _response.size(), MSG_NOSIGNAL);
            res.resetOptions();
            res.getFile().close();
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
        }
        else if (f_read)
        {
            std::cout << "1" << std::endl;
            res.setOptions(HttpResponse::TRANSFERT_ENC, SET);
            appendToResponse(TRANSFERT_ENCODING, "chunked");
            _response += CRLF;
            send(event.getFd(), _response.c_str(), _response.size(), MSG_NOSIGNAL);
            //std::cout << _response;
        }
        if (res.checkBits(HttpResponse::TRANSFERT_ENC))
        {
            std::cout << "3" << std::endl;
            handleChunkedRequest(res, s_buffer);
            send(event.getFd(), _response.c_str(), _response.size(), MSG_NOSIGNAL);
        }
    }
    catch(const std::exception& e)
    {
        event.setErrorStatus(500);
        res.getErrorMethod().sendResponse(event, req, res);
        res.getFile().close();
        res.resetOptions();
        return 1;
    }
    return 0;
}

int Get::firstStep(IO& event, const HttpRequest& req, HttpResponse& res)
{
    TcpServer& instance = *(event.getServer() -> getInstance());
    std::string path(req.getHeaders().find(PATH) -> second);
    res.setPath(instance.getRootDir() + req.getHeaders().find(PATH) -> second);
    DIR *directory = NULL;

    if (path != "/") directory = opendir(res.getPath().c_str());
    
    makeStatusLine(OK);
    
    if ((path == instance.getIndexPath() || (directory == NULL && (errno == ENOENT || errno == ENOTDIR))))
    {
        std::ifstream& file = res.getFile();
        if (req.getHeaders().find(PATH) -> second == instance.getIndexPath())
            res.setPath(instance.getRootDir() + "/" + instance.getIndex());
        file.open(res.getPath().c_str(), std::ifstream::in | std::ifstream::binary);
            
        if (!file) return (res.getErrorMethod().sendResponse(event, req, res), 1);
        
        res.setOptions(HttpResponse::FILE, SET);
    }
    else if (directory)
    {
        res.setOptions(HttpResponse::DIRECTORY, SET);
    }
    res.setOptions(HttpResponse::FIRST_READ, SET);
    res.setOptions(HttpResponse::STARTED, SET);
=======
int Get::handleFileRessource(TcpServer& instance, IO& event, const HttpRequest& req, std::string& ressource)
{
    std::ifstream file;
    std::stringstream buffer;

    if (req.getHeaders().find(PATH) -> second == instance.getIndexPath())
        ressource = instance.getRootDir() + "/" + instance.getIndex();
    
    file.open(ressource.c_str(), std::ifstream::in | std::ifstream::binary);
    
    if (!file) return (event.getReponse().getErrorMethod().sendResponse(event, req), 1);

    buffer << file.rdbuf();
    
    appendToResponse(CONTENT_TYP, utilityMethod::getMimeType(ressource, instance.getRootDir() + "/" + instance.getIndexPath(), instance.getIndex()));
    appendToResponse(CONTENT_LEN, utilityMethod::numberToString(buffer.str().size()));
    _response += CRLF;
    _response += buffer.str();

>>>>>>> origin/main
    return 0;
}

int Get::handleDirectoryRessource(IO& event, DIR *directory)
{
    (void)event;
    (void)directory;
    return (0);
}

<<<<<<< HEAD
void Get::sendResponse(IO& event, const HttpRequest& req, HttpResponse& res)
{


    if (res.checkBits(HttpResponse::FIRST_READ)) res.setOptions(HttpResponse::FIRST_READ, CLEAR);

    if (!res.checkBits(HttpResponse::STARTED) && firstStep(event, req, res)) return ;

    if (res.checkBits(HttpResponse::FILE))
    {
        handleFileRessource(event, req, res);
    }
    //send(event.getFd(), _response.data(), _response.size(), 0);
=======
void Get::sendResponse(IO& event, const HttpRequest& req)
{
    TcpServer& instance = *(event.getServer() -> getInstance());
    std::string path(req.getHeaders().find(PATH) -> second);
    std::string ressource(instance.getRootDir() + req.getHeaders().find(PATH) -> second);
    DIR *directory = NULL;

    if (path != "/") directory = opendir(ressource.c_str());
    
    makeStatusLine(OK);
    
    if ((path == instance.getIndexPath() || (directory == NULL && (errno == ENOENT || errno == ENOTDIR)))
        && handleFileRessource(instance, event, req, ressource))
        return ;
    else if (directory)
        handleDirectoryRessource(event, directory);

    send(event.getFd(), _response.data(), _response.size(), 0);
>>>>>>> origin/main
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
