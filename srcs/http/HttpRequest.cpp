# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/RequestChecker.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/method/Post.hpp"
# include "../../includes/method/Put.hpp"
# include "../../includes/server/HttpServer.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/utils/FileWriter.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpRequest::HttpRequest():HttpMessage(),_header_size(0),_chunk_size(0),_current_chunk_size(0){};
HttpRequest::HttpRequest(const HttpRequest& rhs):HttpMessage(rhs),BitsManipulation(rhs)
{
    _header_size = rhs._header_size;
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

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
void HttpRequest::appendToChunkBody(const std::string& chunk, const ssize_t& size)
{
    _chunk_body.append(chunk, size);
}

void HttpRequest::clearCurrentChunkSize(void)
{
    _current_chunk_size = 0;
}

void HttpRequest::appendToBuffer(const char *toAppend, const ssize_t& size)
{
    size_t  len;
    s_buffer.append(toAppend, size);
    if (_header_size == 0 && ((len = s_buffer.find(CRLF CRLF)) != std::string::npos))
        _header_size = len + 1;
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

    if (headers.size() == 0) return BAD_REQUEST;

    header = UtilityMethod::stringSpliter(headers[0], " ");

    if (header.size() != 3) return BAD_REQUEST;

    _headers[METHOD] = header[0];

    setMetod(HttpServer::getMethodIndex(header[0]));
    
    if (_method == -1) return METHOD_NOT_SUPPORTED;

    _headers[PATH] = UtilityMethod::remove_dup(header[1]);

    _headers[VERSION] = header[2];
    
    if (header[2] != HTTP_VERSION) return BAD_REQUEST;

    bool location = false;

    server -> setInstance((HttpServer *)RequestChecker::serverOrLocation(*server, (*this), location));

    HttpServer& instance = *(server -> getInstance());

    std::string full_path;
    
    if (location == true && instance.checkBits(HttpServer::HTTP_SERVER_ALIAS))
        full_path = instance.getRootDir() + &_headers[PATH][instance.getIndexPath().size()];
    else
        full_path = instance.getRootDir() + _headers[PATH];

    std::cout << "Full path: " << full_path << std::endl;

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
    else if (((getMethod() == HttpServer::HTTP_SERVER_POST || getMethod() == HttpServer::HTTP_SERVER_PUT)) 
        && checkBits(HttpRequest::HTTP_REQUEST_CGI_) == 0)
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
        client.getResponse().setOptions(HttpResponse::HTTP_RESPONSE_REDIRECT_SET, SET);
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
        client.setPrevContentLength(getBodySize());
    }

    int _req = RequestChecker::checkAll(client, (*this));

    std::cout << "REQ VALUE: " << _req << std::endl;
    
    if (_req != 0)  return _req;
    
    if (_it_transfert != _headers.end() || _it_content != _headers.end())
    {
        HttpResponse& res = client.getResponse();

        res.setMethodObj(Method::_tab[getMethod()]());

        FileWriter* writer;
        
        if (getMethod() == HttpServer::HTTP_SERVER_POST)
            writer = (static_cast<Post*>(res.getHttpMethod()));
        else
            writer = (static_cast<Put*>(res.getHttpMethod()));

        if (!checkBits(HttpRequest::HTTP_REQUEST_CGI_) && checkBits(HttpRequest::HTTP_REQUEST_NO_ENCODING))
            writer -> open_file(client) ;
    }

    return IO::IO_SUCCESS;
}

void HttpRequest::clear(void)
{
    _headers.clear();
    s_buffer.clear();
    _header_size = 0;
    resetOptions();
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
