# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/utils/FileWriter.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
FileWriter::FileWriter():_request_body_size(0){};
FileWriter::FileWriter(const FileWriter& rhs):_request_body_size(rhs._request_body_size){};
FileWriter& FileWriter::operator=(const FileWriter& rhs)
{
    if (this == &rhs) return *this;
    _request_body_size = rhs._request_body_size;
    return *this;
};
FileWriter::~FileWriter(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const size_t& FileWriter::getRequestBodySize(void) const {return _request_body_size;}
std::ofstream& FileWriter::getOutfile(void) { return _outfile; };
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
void FileWriter::updateSize(const size_t& size)
{
    _request_body_size += size;
};
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void FileWriter::clearRequestBodySize(void)
{
    _request_body_size = 0;
}

int FileWriter::open_file(HttpServer& instance, std::map<std::string, std::string>& headers, std::string& filepath)
{
    static int _nb;
    std::string& path = headers.find(PATH) -> second; 
    
    /*ADD THIS INTO A TRY CATCH BLOCK*/
    std::string root_dir;

    if (instance.getUploadsFilesFolder().size() == 0)
        filepath = instance.getRootDir() + path + "/" + filepath;
    else
        filepath = instance.getUploadsFilesFolder() + "/" + filepath;

    if (_outfile.is_open()) _outfile.close();
    
    _outfile.clear();
    
    _outfile.open(filepath.c_str(), std::ios::out);
    
    if (_outfile.fail()) return FORBIDEN;
    
    _nb++;

    return IO::IO_SUCCESS;
}

int FileWriter::open_file(HttpServer& instance, std::map<std::string, std::string>& headers)
{
    static int _nb;
    std::string& path = headers.find(PATH) -> second; 
    std::string fileExtenstion = UtilityMethod::getFileExtension(headers.find(CONTENT_TYP) -> second, 0);
    std::string filepath(headers.find(FULLPATH) -> second);
    
    /*ADD THIS INTO A TRY CATCH BLOCK*/
    if (path == instance.getIndexPath())
        filepath += DEFAULT_FILE_NAME + UtilityMethod::numberToString(_nb) + fileExtenstion;

    if (_outfile.is_open()) _outfile.close();
    
    _outfile.clear();
    
    _outfile.open(filepath.c_str(), std::ios::out);
    
    if (_outfile.fail()) return FORBIDEN;
    _nb++;
    
    return IO::IO_SUCCESS;
}

int FileWriter::fillChunkBody(HttpRequest& req)
{

    while (1)
    {
        size_t pos = 0;
        
        std::string& s_buffer= req.getBuffer();

        if (req.checkBits(HttpRequest::HTTP_REQUEST_CHUNK_SET) == 0)
        {
            size_t start = 0;
        
            if (req.checkBits(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED))
            {
                size_t pos = s_buffer.find(CRLF);

                if (pos != 0 || (pos == std::string::npos && s_buffer.size() > LEN_CRLF)) return BAD_REQUEST;

                start = LEN_CRLF;

                req.setOptions(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED, CLEAR);
            }

            if (req.getCurrentChunkSize() > 0) req.clearCurrentChunkSize();

            pos = s_buffer.find(CRLF, start);

            if (pos == std::string::npos)  return (req.setOptions(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED, SET), IO::IO_SUCCESS);
            
            pos += LEN_CRLF;

            char c = s_buffer[pos];
            s_buffer[pos] = 0;

            if (s_buffer.find_first_not_of(BASE_16 CRLF) != pos) return BAD_REQUEST;

            s_buffer[pos] = c;
            req.setChunkSize(UtilityMethod::hexToDecimal(s_buffer.substr(start, pos - LEN_CRLF)));
            
            if (req.getChunkSize() == std::string::npos) return BAD_REQUEST;

            req.setOptions(HttpRequest::HTTP_REQUEST_CHUNK_SET, SET);
        }

        size_t size = req.getChunkSize() - req.getCurrentChunkSize();

        if (size > s_buffer.size() - pos)
            size = s_buffer.size();
            
        req.updateCurrentChunkSize(size - pos);

        int err = writeToFile(req, pos, size - pos);
        
        if (err) return err;

        s_buffer.erase(0, size);

        if (req.getCurrentChunkSize() == req.getChunkSize())
        {
            req.setOptions(HttpRequest::HTTP_REQUEST_CHUNK_SET, CLEAR);
            req.setOptions(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED, SET);
        };

        if (s_buffer.find(END_CHUNK) == 0)
        {
            if (s_buffer.size() != LEN_END_CHUNK) return BAD_REQUEST;
            req.updateCurrentChunkSize(LEN_END_CHUNK - LEN_CRLF);
            std::cout << "Total body size: " << getRequestBodySize() << std::endl;
            clearRequestBodySize();
            req.getOutfile().close();
            req.setOptions(HttpRequest::HTTP_REQUEST_FINISH_BODY, SET);
            return IO::IO_SUCCESS;
        }

        if (s_buffer.size() <= LEN_CRLF) return IO::IO_SUCCESS;
    }

    return IO::IO_SUCCESS;
}

int FileWriter::writeToFileMutltipartData(HttpRequest& req, const size_t& size)
{
    try
    {
        _outfile.write(req.getBuffer().data(), size);
    }
    catch(const std::exception& e)
    {
        _outfile.close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}

int FileWriter::writeToFile(HttpRequest& req, const size_t& pos, const size_t& bytes)
{
    try
    {
        std::string& alias = req.getBuffer();
        if (bytes)
        {
            const char *buffer = alias.data();
            _outfile.write(&buffer[pos], bytes);
            updateSize(bytes);
        }
    }
    catch(const std::exception& e)
    {
        _outfile.close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}


int FileWriter::writeToFile(HttpRequest& req)
{
    try
    {
        std::string& alias = req.getBuffer();
        
        _outfile.write(alias.data(), alias.size());
        updateSize(alias.size());
        if (getRequestBodySize() >= req.getBodySize())
        {
            std::cout << _request_body_size << std::endl;
            req.setOptions(HttpRequest::HTTP_REQUEST_FINISH_BODY, SET);
            clearRequestBodySize();
            _outfile.close();
        }
        alias.clear();
    }
    catch(const std::exception& e)
    {
        _outfile.close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}

int FileWriter::handleMultipartData(ClientSocketStream& client, HttpRequest& req)
{
    client.updatePrevBodySize(req.getBuffer().size());

    while (1)
    {
        if (req.checkBits(HttpRequest::HTTP_REQUEST_STARTED) == 0)
        {
            size_t start = 0;
            std::string& s_buffer = req.getBuffer();
            const std::string& boundary = req.getBoundary();
            
            if (req.checkBits(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED))
            {
                size_t pos = s_buffer.find_first_not_of(CRLF);

                if (pos == std::string::npos || pos != LEN_CRLF) return BAD_REQUEST;

                start = pos;
                req.setOptions(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED, CLEAR);
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
                    != UtilityMethod::getFileExtension(vec_content_type[1] ,0))
                {
                    if (client.getPrevBodySize() != req.getBodySize()) client.setOptions(IO::IO_SOCKET_NOT_FINISH, SET);

                    return BAD_REQUEST;
                }
                
                if (vector_filename[1].size()) 
                    open_file(*(client.getServer() -> getInstance()), req.getHeaders(),vector_filename[1]);

                pos_two = pos_three;
            }

            updateSize(pos_two + LEN_CRLF + LEN_CRLF);

            s_buffer.erase(0, pos_two + LEN_CRLF + LEN_CRLF);
            
            req.setOptions(HttpRequest::HTTP_REQUEST_STARTED, SET);
        }
  
        if (req.checkBits(HttpRequest::HTTP_REQUEST_STARTED))
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
                req.setOptions(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED, SET);
            
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
                req.setOptions(HttpRequest::HTTP_REQUEST_FINISH_BODY, SET);
                break;
            }

            req.setOptions(HttpRequest::HTTP_REQUEST_STARTED, CLEAR);
        }
    }
    return IO::IO_SUCCESS;
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
