# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/RequestChecker.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/method/Post.hpp"
# include "../../includes/server/TcpServer.hpp"
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpRequest::HttpRequest():HttpMessage(),_header_size(0),_chunk_size(0),_current_chunk_size(0),_start(true){};
HttpRequest::HttpRequest(const HttpRequest& rhs):HttpMessage(rhs),BitsManipulation(rhs)
{
    _header_size = rhs._header_size;
    _start = rhs._start;
};
HttpRequest& HttpRequest::operator=(const HttpRequest& rhs)
{
    if (this == &rhs) return *this;
    _start = rhs._start;
    _header_size = rhs._header_size;
    s_buffer = rhs.s_buffer;
    _method = rhs._method;
    _body = rhs._body;
    return *this;
};
HttpRequest::~HttpRequest(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const std::string& HttpRequest::getChunkBody(void) const {return _chunk_body;}
const size_t& HttpRequest::getChunkSize(void) const {return _chunk_size;}
const size_t& HttpRequest::getCurrentChunkSize(void) const {return _current_chunk_size;}
std::string& HttpRequest::getChunkBody(void) {return _chunk_body;}
const size_t& HttpRequest::getHeaderSize(void) const {return _header_size;}
std::ofstream& HttpRequest::getOutfile(void) {return outfile;}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void HttpRequest::setChunkSize(const size_t& chunk_size)
{
    _chunk_size = chunk_size;
}

void HttpRequest::updateCurrentChunkSize(const size_t& chunk_size)
{
    _current_chunk_size += chunk_size;
}

/*----------------------------------------SETTER----------------------------------------*/
void HttpRequest::appendToChunkBody(const std::string& chunk, const ssize_t& size)
{
    _chunk_body.append(chunk, size);
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void HttpRequest::clearCurrentChunkSize(void)
{
    _current_chunk_size = 0;
}

int HttpRequest::fillChunkBody(IO& object, Post& post)
{
    while (1)
    {
        if (object.checkBits(HttpRequest::CHUNK_SET) == 0)
        {
            size_t start = 0;

            if (object.checkBits(HttpRequest::CARRIAGE_FEED))
            {
                size_t pos = s_buffer.find_first_not_of(CRLF);

                if (pos == std::string::npos || pos != LEN_CRLF) return BAD_REQUEST;
                
                start = pos;

                object.setOptions(HttpRequest::CARRIAGE_FEED, CLEAR);
            }

            if (getCurrentChunkSize() > 0) clearCurrentChunkSize();

            size_t pos = s_buffer.find(CRLF, start);
            
            if (pos == std::string::npos)  return IO::IO_SUCCESS;
            
            pos += LEN_CRLF;

            char c = s_buffer[pos];
            s_buffer[pos] = 0;

            if (s_buffer.find_first_not_of(BASE_16 CRLF) != pos) return BAD_REQUEST;
            
            s_buffer[pos] = c;

            setChunkSize(UtilityMethod::hexToDecimal(s_buffer.substr(start, pos - LEN_CRLF)));
            
            if (getChunkSize() == std::string::npos) return BAD_REQUEST;

            s_buffer.erase(0, pos);

            object.setOptions(HttpRequest::CHUNK_SET, SET);
        }

        size_t size = getChunkSize() - getCurrentChunkSize();

        if (size > s_buffer.size())
            size = s_buffer.size();
            
        updateCurrentChunkSize(size);

        int err = post.writeToFile(object, (*this), size);
        
        if (err) return err;

        s_buffer.erase(0, size);

        if (getCurrentChunkSize() == getChunkSize())
        {
            object.setOptions(HttpRequest::CHUNK_SET, CLEAR);
            object.setOptions(HttpRequest::CARRIAGE_FEED, SET);
        };

        if (s_buffer.find(END_CHUNK) == 0)
        {
            if (s_buffer.size() != LEN_END_CHUNK) return BAD_REQUEST;
            object.setOptions(HttpRequest::FINISH_BODY, SET);
            return IO::IO_SUCCESS;
        }
        if (s_buffer.size() == 0) return IO::IO_SUCCESS;
    }

    return IO::IO_SUCCESS;
}

void HttpRequest::appendToBuffer(const char *toAppend, const ssize_t& size)
{
    size_t  len;
    s_buffer.append(toAppend, size);
    if (_header_size == 0 && ((len = s_buffer.find(CRLF CRLF)) != std::string::npos))
        _header_size = len + 1;
}

int HttpRequest::open_file(IO& event, std::string& filepath)
{
    static int _nb;
    TcpServer& instance = *(event.getServer() -> getInstance());
    std::string& path = getHeaders().find(PATH) -> second; 
    
    /*ADD THIS INTO A TRY CATCH BLOCK*/

    filepath = instance.getRootDir() + path + "/" + filepath;

    if (outfile.is_open()) outfile.close();
    
    outfile.clear();
    
    outfile.open(filepath.c_str(), std::ios::out);
    
    if (outfile.fail()) return FORBIDEN;
    
    _nb++;

    return IO::IO_SUCCESS;
}

int HttpRequest::open_file(IO& event)
{
    static int _nb;
    TcpServer& instance = *(event.getServer() -> getInstance());
    std::string& path = getHeaders().find(PATH) -> second; 
    std::string fileExtenstion = UtilityMethod::getFileExtension(getHeaders().find(CONTENT_TYP) -> second, 0);
    std::string filepath(getHeaders().find(FULLPATH) -> second);
    
    /*ADD THIS INTO A TRY CATCH BLOCK*/

    if (path == instance.getIndexPath())
        filepath += DEFAULT_FILE_NAME + UtilityMethod::numberToString(_nb) + fileExtenstion;

    if (outfile.is_open()) outfile.close();
    
    outfile.clear();
    
    outfile.open(filepath.c_str(), std::ios::out);
    
    if (outfile.fail()) return FORBIDEN;
    
    _nb++;

    return IO::IO_SUCCESS;
}

int HttpRequest::parseRequest(IO& object)
{
    if (object.checkBits(HttpRequest::CONTENT_LENGTH) || object.checkBits(HttpRequest::TRANSFER_ENCODING))
        return 0;
    
    std::vector<std::string> headers = UtilityMethod::stringSpliter(s_buffer.substr(0, s_buffer.find(CRLF CRLF)), CRLF);
    std::vector<std::string> header;
    std::map<std::string, std::string>::const_iterator _it_content;
    std::map<std::string, std::string>::const_iterator _it_transfert;
    size_t len = headers.size();

    if (headers.size() == 0) return BAD_REQUEST;

    header = UtilityMethod::stringSpliter(headers[0], " ");

    if (header.size() != 3) return BAD_REQUEST;

    _headers[METHOD] =  header[0];
    _headers[PATH] = UtilityMethod::remove_dup(header[1]);
    _headers[VERSION] = header[2];

    setMetod(TcpServer::getHttpMethod(header[0]));

    Server& server = *(object.getServer());
    server.setInstance((TcpServer *)RequestChecker::serverOrLocation(server, (*this)));
    const TcpServer *instance = server.getInstance();

    std::string full_path = instance -> getRootDir() + _headers[PATH];

    int directory = UtilityMethod::is_a_directory(full_path.c_str());

    if (getMethod() == TcpServer::GET)
    {
        if ((directory && instance -> getIndex().size()) && (_headers[PATH] == instance -> getIndexPath()))
            full_path += '/' + instance -> getIndex();
        else if (directory && instance -> getIndex().size() == 0)
            full_path = "";
    }
    
    _headers[FULLPATH] = full_path.size() > 0 ? UtilityMethod::remove_dup(full_path) : "";
    
    for (size_t i = 1; i < len; i++)
    {
        header = UtilityMethod::stringSpliter(headers[i], ":");

        if (header.size() <= 1) return BAD_REQUEST;

        if (i != 0)
            for (size_t i = 2; i < header.size(); i++)
                header[1] += ":" + header[i];
        
        _headers[header[0]] = header[1].erase(0, 1);
    }
    
    size_t lenq;

    if (server.getInstance() -> getRedirect().size() > 0)
    {
        object.getReponse().setOptions(HttpResponse::REDIRECT_SET, SET);
        return getMethod();
    }
    
    if ((lenq = s_buffer.find(CRLF CRLF)) != std::string::npos)
    {
        lenq += 4;
        std::cout << s_buffer.substr(0, lenq);
        s_buffer.erase(0, lenq);
    }

    _it_content = _headers.find(CONTENT_LEN);
    _it_transfert = _headers.find(TRANSFERT_ENCODING);

    if (_it_transfert != _headers.end())
        object.setOptions(HttpRequest::TRANSFER_ENCODING, SET);

    if (_it_content != _headers.end())
    {
        object.setOptions(HttpRequest::CONTENT_LENGTH, SET);
        setBodySize(_it_content -> second);
    }

    int _req = RequestChecker::checkAll(object, (*this), object.getReponse());

    std::cout << "Req value: " << _req << std::endl;
    
    if (_req != 0)  return _req;
    
    if (_it_transfert != _headers.end() || _it_content != _headers.end())
    {
        HttpRequest& req = object.getRequest();
        HttpResponse& res = object.getReponse();

        res.setMethodObj(Method::_tab[req.getMethod()]());

        if (res.checkBits(HttpResponse::NO_ENCODING)) open_file(object);

    }

    return IO::IO_SUCCESS;
}

int HttpRequest::checkValidHeader(const int& _ws, struct epoll_event event) const
{
    (void)_ws;
    (void)event;
    
    return 0;
}

void HttpRequest::clear(void)
{
    _headers.clear();
    s_buffer.clear();
    resetOptions();
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
