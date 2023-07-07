# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/RequestChecker.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/method/Post.hpp"
# include "../../includes/server/HttpServer.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpRequest::HttpRequest():HttpMessage(),_header_size(0),_chunk_size(0),_current_chunk_size(0),_start(true){};
HttpRequest::HttpRequest(const HttpRequest& rhs):HttpMessage(rhs),BitsManipulation(rhs)
{
    _header_size = rhs._header_size;
    _start = rhs._start;
    _form_data.boundary = rhs._form_data.boundary;
    _form_data.crlf_boundary = rhs._form_data.crlf_boundary;
    _form_data.end_boundary = rhs._form_data.end_boundary;
    _form_data.crlf_end_boundary = rhs._form_data.crlf_end_boundary;
};
HttpRequest& HttpRequest::operator=(const HttpRequest& rhs)
{
    if (this == &rhs) return *this;
    _form_data.boundary = rhs._form_data.boundary;
    _form_data.crlf_boundary = rhs._form_data.crlf_boundary;
    _form_data.end_boundary = rhs._form_data.end_boundary;
    _form_data.crlf_end_boundary = rhs._form_data.crlf_end_boundary;
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
const std::string& HttpRequest::getBoundary(void) const {return _form_data.boundary; }
const std::string& HttpRequest::getCrlfBoundary(void) const {return _form_data.crlf_boundary; }
const std::string& HttpRequest::getEndBoundary(void) const {return _form_data.end_boundary; }
const std::string& HttpRequest::getCrlfEndBoundary(void) const {return _form_data.crlf_end_boundary; }
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

void HttpRequest::setBoundary(const std::string& boundary)
{
    _form_data.boundary = boundary;
}

void HttpRequest::setEndBoundary(const std::string& end_boundary)
{
    _form_data.end_boundary = end_boundary;
}

void HttpRequest::setCrlfBoundary(const std::string& crlf_boundary)
{
    _form_data.crlf_boundary = crlf_boundary;
}

void HttpRequest::setCrlfEndBoundary(const std::string& crlf_end_boundary)
{
    _form_data.crlf_end_boundary = crlf_end_boundary;
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

int HttpRequest::fillChunkBody(Post& post)
{

    while (1)
    {
        size_t pos = 0;
        
        if (checkBits(HttpRequest::HTTP_REQUEST_CHUNK_SET) == 0)
        {
            size_t start = 0;

            if (checkBits(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED))
            {
                size_t pos = s_buffer.find(CRLF);

                if (pos != 0 || (pos == std::string::npos && s_buffer.size() > LEN_CRLF)) return BAD_REQUEST;

                start = LEN_CRLF;

                setOptions(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED, CLEAR);
            }

            if (getCurrentChunkSize() > 0) clearCurrentChunkSize();

            pos = s_buffer.find(CRLF, start);

            if (pos == std::string::npos)  return (setOptions(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED, SET), IO::IO_SUCCESS);
            
            pos += LEN_CRLF;

            char c = s_buffer[pos];
            s_buffer[pos] = 0;

            if (s_buffer.find_first_not_of(BASE_16 CRLF) != pos) return BAD_REQUEST;

            s_buffer[pos] = c;
            setChunkSize(UtilityMethod::hexToDecimal(s_buffer.substr(start, pos - LEN_CRLF)));
            
            if (getChunkSize() == std::string::npos) return BAD_REQUEST;

            setOptions(HttpRequest::HTTP_REQUEST_CHUNK_SET, SET);
        }

        size_t size = getChunkSize() - getCurrentChunkSize();

        if (size > s_buffer.size() - pos)
            size = s_buffer.size();
            
        updateCurrentChunkSize(size - pos);

        int err = post.writeToFile((*this), pos, size - pos);
        
        if (err) return err;

        s_buffer.erase(0, size);

        if (getCurrentChunkSize() == getChunkSize())
        {
            setOptions(HttpRequest::HTTP_REQUEST_CHUNK_SET, CLEAR);
            setOptions(HttpRequest::HTTP_REQUEST_CARRIAGE_FEED, SET);
        };

        if (s_buffer.find(END_CHUNK) == 0)
        {
            if (s_buffer.size() != LEN_END_CHUNK) return BAD_REQUEST;
            updateCurrentChunkSize(LEN_END_CHUNK - LEN_CRLF);
            std::cout << "Total body size: " << post.getRequestBodySize() << std::endl;
            post.clearRequestBodySize();
            outfile.close();
            setOptions(HttpRequest::HTTP_REQUEST_FINISH_BODY, SET);
            return IO::IO_SUCCESS;
        }

        if (s_buffer.size() <= LEN_CRLF) return IO::IO_SUCCESS;
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

int HttpRequest::open_file(ClientSocketStream& event, std::string& filepath)
{
    static int _nb;
    HttpServer& instance = *(event.getServer() -> getInstance());
    std::string& path = getHeaders().find(PATH) -> second; 
    
    /*ADD THIS INTO A TRY CATCH BLOCK*/
    std::string root_dir;

    if (instance.getUploadsFilesFolder().size() == 0)
        filepath = instance.getRootDir() + path + "/" + filepath;
    else
        filepath = instance.getUploadsFilesFolder() + "/" + filepath;

    if (outfile.is_open()) outfile.close();
    
    outfile.clear();
    
    outfile.open(filepath.c_str(), std::ios::out);
    
    if (outfile.fail()) return FORBIDEN;
    
    _nb++;

    return IO::IO_SUCCESS;
}

int HttpRequest::open_file(ClientSocketStream& event)
{
    static int _nb;
    HttpServer& instance = *(event.getServer() -> getInstance());
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

int HttpRequest::parseRequest(ClientSocketStream& client)
{
    if (checkBits(HttpRequest::HTTP_REQUEST_CONTENT_LENGTH) || checkBits(HttpRequest::HTTP_REQUEST_TRANSFER_ENCODING))
        return 0;
    
    std::vector<std::string> headers = UtilityMethod::stringSpliter(s_buffer.substr(0, s_buffer.find(CRLF CRLF)), CRLF);
    std::vector<std::string> header;
    std::map<std::string, std::string>::const_iterator _it_content;
    std::map<std::string, std::string>::const_iterator _it_transfert;
    size_t len = headers.size();

    for (size_t i = 1; i < len; i++)
    {
        header = UtilityMethod::stringSpliter(headers[i], ":");

        if (header.size() <= 1) return BAD_REQUEST;

        if (i != 0)
            for (size_t i = 2; i < header.size(); i++)
                header[1] += ":" + header[i];
        
        _headers[header[0]] = header[1].erase(0, 1);
    }
    
    if (_headers.find(HOST) != _headers.end()) _headers[HOST] =_headers[HOST].substr(0, _headers[HOST].find(':'));

    Server* server = (HttpServer::getHostnameServerMap(client.getPort(), _headers[HOST]));
    
    if (server == NULL) server = client.getServer();

    client.setServer(server);

    server -> setInstance(server);

    if (headers.size() == 0) return BAD_REQUEST;

    header = UtilityMethod::stringSpliter(headers[0], " ");

    if (header.size() != 3) return BAD_REQUEST;

    _headers[METHOD] = header[0];

    if (HttpServer::getMethodIndex(header[0]) == -1) return METHOD_NOT_SUPPORTED;

    _headers[PATH] = UtilityMethod::remove_dup(header[1]);

    if (header[1].find('/') == std::string::npos) return BAD_REQUEST;

    _headers[VERSION] = header[2];
    
    if (header[2] != HTTP_VERSION) return BAD_REQUEST;

    setMetod(HttpServer::getHttpMethod(header[0]));

    server -> setInstance((HttpServer *)RequestChecker::serverOrLocation(*server, (*this)));

    const HttpServer& instance = *(server -> getInstance());

    std::string full_path = instance.getRootDir() + _headers[PATH];

    int directory = UtilityMethod::is_a_directory(full_path.c_str());

    std::string path(_headers[PATH]);
    size_t i = path.find('?');

    if (i != std::string::npos) path = path.substr(0, i);
    const std::map<std::string, std::string>& cgi_map = instance.getCgiMap();
    const std::map<std::string, std::string>::const_iterator& it = cgi_map.find(UtilityMethod::getFileExtension(full_path, 1));

    if (it != cgi_map.end()) setOptions(HttpRequest::HTTP_REQUEST_CGI_, SET);

    if (getMethod() == HttpServer::HTTP_SERVER_GET)
    {
        if ((directory && instance.getIndex().size()) && (_headers[PATH] == instance.getIndexPath()))
            full_path += '/' + instance.getIndex();
        else if (directory && instance.getIndex().size() == 0)
            full_path = "";
    }
    else if (getMethod() == HttpServer::HTTP_SERVER_POST && checkBits(HttpRequest::HTTP_REQUEST_CGI_) == 0)
    {
        if (instance.getUploadsFilesFolder().size())
        {
            if (directory)
                full_path = instance.getUploadsFilesFolder();
            else
                full_path = instance.getUploadsFilesFolder() + "/" + full_path.substr(full_path.rfind('/') + 1);
        }
    }

    _headers[FULLPATH] = full_path.size() > 0 ? UtilityMethod::remove_dup(full_path) : "";

    size_t lenq;

    if (server -> getInstance() -> getRedirect().size() > 0)
    {
        client.getReponse().setOptions(HttpResponse::HTTP_RESPONSE_REDIRECT_SET, SET);
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
        setOptions(HttpRequest::HTTP_REQUEST_TRANSFER_ENCODING, SET);

    if (_it_content != _headers.end())
    {
        setOptions(HttpRequest::HTTP_REQUEST_CONTENT_LENGTH, SET);
        setBodySize(_it_content -> second);
    }

    int _req = RequestChecker::checkAll(client, (*this));

    std::cout << "REQ VALUE: " << _req << std::endl;
    
    if (_req != 0)  return _req;
    
    if (_it_transfert != _headers.end() || _it_content != _headers.end())
    {
        HttpResponse& res = client.getReponse();

        res.setMethodObj(Method::_tab[getMethod()]());

        if (!checkBits(HttpRequest::HTTP_REQUEST_CGI_) && checkBits(HttpRequest::HTTP_REQUEST_NO_ENCODING)) open_file(client);
    }

    return IO::IO_SUCCESS;
}

void HttpRequest::clear(void)
{
    _headers.clear();
    s_buffer.clear();
    resetOptions();
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
