# include "../../includes/method/Put.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/IO/CgiStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Put::Put():FileWriter(HttpServer::HTTP_SERVER_PUT){};
Put::Put(const Put& rhs):Method(rhs),FileWriter(rhs){};
Put& Put::operator=(const Put& rhs)
{
    if (this == &rhs) return *this;
    _request_body_size = rhs._request_body_size;
    return *this;
};
Put::~Put(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

int Put::open_file(ClientSocketStream& client)
{
    std::map<std::string, std::string>& headers = client.getRequest().getHeaders();
    std::string& path = headers.find(PATH) -> second; 
    std::string fileExtenstion = UtilityMethod::getFileExtension(headers.find(CONTENT_TYP) -> second, 0);
    std::string filepath(headers.find(FULLPATH) -> second);
    
    /*ADD THIS INTO A TRY CATCH BLOCK*/
    if (path == client.getServer() -> getInstance() -> getIndexPath())
        filepath += DEFAULT_FILE_NAME + UtilityMethod::numberToString(_nb) + fileExtenstion;

    if (_outfile.is_open()) _outfile.close();
    
    _outfile.clear();
    
    _outfile.open(filepath.c_str(), std::ios::out);
    
    if (_outfile.fail()) return FORBIDEN;

    updateNb() ;
       
    return IO::IO_SUCCESS;
}

int Put::open_file(ClientSocketStream& client, std::string& filepath)
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
    
    if (access(filepath.c_str(), F_OK) == 0)

    _outfile.open(filepath.c_str(), std::ios::out);
    if (_outfile.fail()) return FORBIDEN;
    
    _nb++;

    return IO::IO_SUCCESS;
}

int Put::sendResponse(ClientSocketStream& client, HttpRequest& req, HttpResponse& res)
{
    if (res.checkBits(HttpResponse::HTTP_RESPONSE_REDIRECT_SET)) return sendRedirect(client, res, FOUND_REDIRECT_POST);

    if (client.getEvents() & EPOLLIN)
    {
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

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Put::clone(void) const {return new Put; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
