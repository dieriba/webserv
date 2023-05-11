# include "../../includes/method/Get.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/UtilityMethod.hpp"
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

int Get::handleChunkedRequest(HttpResponse& res, std::string& s_buffer)
{
    _response = UtilityMethod::decimalToHex(s_buffer.size()) + CRLF + s_buffer + CRLF;

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
        char buffer[REQUEST_SIZE + 1] = {0};

        std::ifstream& file = res.getFile();
        
        file.read(buffer, REQUEST_SIZE);
    
        if (sendBuffer(event.getFd(), buffer, file.gcount()))
        {
            std::cout << "Socket Ended" << std::endl;
            return (-1);
        }

        if (file.fail() && file.eof())
        {
            res.resetOptions();
            file.close();
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
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
    DIR *directory;

    if (path != "/") directory = opendir(res.getPath().c_str());
        
    if ((path == instance.getIndexPath() || (directory == NULL && (errno == ENOENT || errno == ENOTDIR))))
    {
        std::ifstream& file = res.getFile();

        if (req.getHeaders().find(PATH) -> second == instance.getIndexPath())
            res.setPath(instance.getRootDir() + "/" + instance.getIndex());

        file.open(res.getPath().c_str(), std::ifstream::in | std::ifstream::binary);
            
        if (!file) 
        {
            delete res.getHttpMethod();
            res.setMethodObj(new Error);
            return (1);
        }
    
        file.seekg(0, std::ios::end);
        res.setBodySize(file.tellg());
        file.seekg(0, std::ios::beg);

        if (file.fail())
        {
            delete res.getHttpMethod();
            res.setMethodObj(new Error);
            return (1);
        }

        makeStatusLine(OK);

        std::string ressource(res.getPath());
        
        appendToResponse(CONTENT_TYP, UtilityMethod::getMimeType(ressource, instance.getFullIndexPath(), instance.getIndex()));
        appendToResponse(CONTENT_LEN, UtilityMethod::numberToString(res.getBodySize()));
        _response += CRLF;

        if (sendBuffer(event.getFd(), _response.c_str(), _response.size()))
        {
            std::cout << "Closing socket: " << event.getFd() << std::endl;
            return (IO::IO_ERROR);
        }

        _response.clear();
        
        res.setOptions(HttpResponse::FILE, SET);
    }
    else if (directory)
    {
        res.setOptions(HttpResponse::DIRECTORY, SET);
    }
    res.setOptions(HttpResponse::STARTED, SET);
    return 0;
}

int Get::handleDirectoryRessource(IO& event, DIR *directory)
{
    (void)event;
    (void)directory;
    return (0);
}

int Get::sendResponse(IO& event, const HttpRequest& req, HttpResponse& res)
{
    if (!res.checkBits(HttpResponse::STARTED))
    {
        int err = firstStep(event, req, res);

        if (err) return err;
    }

    if (res.checkBits(HttpResponse::FILE))
    {
        handleFileRessource(event, req, res);
    }

    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
