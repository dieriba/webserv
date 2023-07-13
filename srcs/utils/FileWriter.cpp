# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/utils/FileWriter.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
FileWriter::FileWriter():_request_body_size(0){};
FileWriter::FileWriter(const short int& method):_request_body_size(0),_method(method){};
FileWriter::FileWriter(const FileWriter& rhs):_request_body_size(rhs._request_body_size),_method(rhs._method){};
FileWriter& FileWriter::operator=(const FileWriter& rhs)
{
    if (this == &rhs) return *this;
    _nb = rhs._nb;
    _method = rhs._method;
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

void FileWriter::setFilename(const std::string& filename)
{
    _filename = filename;
    _filename_c_str = _filename.c_str();
}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void FileWriter::clearAndUpdateRequestBodySize(const size_t& request_size)
{
    _request_body_size = request_size;
}

void FileWriter::erase_file(void)
{
    if (_filename_c_str == NULL) return;
  
    std::remove(_filename_c_str);
    _filename = "";
    _filename_c_str = NULL;
}

void FileWriter::clearRequestBodySize(void)
{
    _request_body_size = 0;
}

int FileWriter::fillChunkBody(HttpRequest& req)
{

    while (1)
    {
        size_t pos = 0;
        
        std::string& s_buffer = req.getBuffer();

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
            _outfile.close();
            req.setOptions(HttpRequest::HTTP_REQUEST_FINISH_BODY, SET);
            return IO::IO_SUCCESS;
        }

        if (s_buffer.size() <= LEN_CRLF) return IO::IO_SUCCESS;
    }

    return IO::IO_SUCCESS;
}

int FileWriter::writeToFileMutltipartData(ClientSocketStream& client, const size_t& size)
{
    try
    {
        if (access(_filename_c_str, F_OK) != 0)
            throw std::runtime_error("An error occurred while trying reaching for the file located at: " + _filename);

        _outfile.write(client.getRequest().getBuffer().data(), size);

        if (_outfile.bad()) 
            throw std::runtime_error("An error occurred while writing to the file!");
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        _outfile.close();
        erase_file();
        if (client.getPrevBodySize() != client.getRequest().getBodySize())
        {
            client.setOptions(IO::IO_SOCKET_NOT_FINISH, SET);
            client.setOptions(IO::IO_CONTENT_LEN, SET);
        }
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}

int FileWriter::writeToFile(HttpRequest& req, const size_t& pos, const size_t& bytes)
{
    try
    {
        if (access(_filename_c_str, F_OK) != 0)
            throw std::runtime_error("An error occurred while trying reaching for the file located at: " + _filename);
        
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
        std::cout << e.what() << std::endl;
        erase_file();
        _outfile.close();
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }

    return IO::IO_SUCCESS;
}


int FileWriter::writeToFile(HttpRequest& req)
{
    std::string& alias = req.getBuffer();

    try
    {
        if (access(_filename_c_str, F_OK) != 0)
            throw std::runtime_error("An error occurred while trying reaching for the file located at: " + _filename);

        size_t len = alias.size();

        if (len + getRequestBodySize() > req.getBodySize())
        {
            req.setOptions(HttpRequest::HTTP_REQUEST_SAVE_STRING, SET);
            req.setRequestSize(len + getRequestBodySize() - req.getBodySize());  
            len = req.getBodySize() - getRequestBodySize();
        }

        _outfile.write(alias.data(), len);

        if (_outfile.fail())
            throw std::runtime_error("An error occurred while writing into the file located at: " + _filename);

        updateSize(len);
        
        if (getRequestBodySize() >= req.getBodySize())
        {
            std::cout << "Request body: " << _request_body_size << std::endl;
            req.setOptions(HttpRequest::HTTP_REQUEST_FINISH_BODY, SET);
            clearRequestBodySize();
            _outfile.close();
        }

        if (req.checkBits(HttpRequest::HTTP_REQUEST_SAVE_STRING) == 0) alias.clear();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        alias.clear();
        erase_file();
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
                
                if (UtilityMethod::getFileExtension(vector_filename[1] , true) != UtilityMethod::getFileExtension(vec_content_type[1] , false))
                {
                    if (client.getPrevBodySize() != req.getBodySize())
                        client.setOptions(IO::IO_SOCKET_NOT_FINISH, SET);
                    client.setOptions(IO::IO_CONTENT_LEN, SET);
                    return BAD_REQUEST;
                }
                
                if (vector_filename[1].size()) open_file(client, vector_filename[1]);

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
            
            if (_outfile.is_open() && size > 0)
            {
                int res = writeToFileMutltipartData(client, size);

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
            
            if (_outfile.is_open())
            {
                _outfile.clear();
                _outfile.close();
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

/*----------------------------------------STATIC VARIABLES----------------------------------------*/
unsigned int FileWriter::_nb = 0;
/*----------------------------------------STATIC VARIABLES----------------------------------------*/
