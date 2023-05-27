# include "../../includes/method/Get.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/server/TcpServer.hpp"

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

int Get::handleFileRessource(IO& event, HttpRequest& req, HttpResponse& res)
{
    try
    {
        (void)req;
        char buffer[REQUEST_SIZE + 1] = {0};

        std::ifstream& file = res.getFile();
        
        file.read(buffer, REQUEST_SIZE);
    
        if (sendBuffer(event.getFd(), buffer, file.gcount()))
        {
            std::cout << "Socket Ended" << std::endl;
            return (IO::IO_ERROR);
        }

        if (file.fail() && file.eof())
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    catch(const std::exception& e)
    {
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}

int Get::firstStep(IO& event, const HttpRequest& req, HttpResponse& res)
{
    TcpServer& instance = *(event.getServer() -> getInstance());
    std::string full_path(req.getHeaders().find(FULLPATH) -> second);
    
    int directory = UtilityMethod::is_a_directory(res.getPath().c_str());

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
    
    return IO::IO_SUCCESS;
}

int Get::handleDirectoryRessource(IO& event, DIR *directory)
{
    (void)event;
    (void)directory;
    return (0);
}

int Get::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    if (!res.checkBits(HttpResponse::REDIRECT_SET))
    {
        if (!res.checkBits(HttpResponse::STARTED))
        {
            int err = firstStep(event, req, res);
            if (err) return err;
        }

        if (res.checkBits(HttpResponse::FILE))
        {
            return handleFileRessource(event, req, res);
        }
    }
    else
    {
        if (sendBuffer(event.getFd(), FOUND_REDIRECT, UtilityMethod::myStrlen(FOUND_REDIRECT)))
            return IO::IO_ERROR;
        const std::string& link = event.getServer() -> getInstance() -> getRedirect();
        
        if (sendBuffer(event.getFd(), link.c_str(), link.size()) || sendBuffer(event.getFd(), CRLF CRLF, UtilityMethod::myStrlen(CRLF CRLF)))
            return IO::IO_ERROR;
            
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
