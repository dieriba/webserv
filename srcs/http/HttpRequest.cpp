# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/server/TcpServer.hpp"
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpRequest::HttpRequest():HttpMessage(),_header_size(0),_request_body_size(0),_start(true){};
HttpRequest::HttpRequest(const HttpRequest& rhs):HttpMessage(rhs)
{
    _request_body_size = rhs._request_body_size;
    _header_size = rhs._header_size;
    _start = rhs._start;
};
HttpRequest& HttpRequest::operator=(const HttpRequest& rhs)
{
    if (this == &rhs) return *this;
    _request_body_size = rhs._request_body_size;
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
const size_t& HttpRequest::getHeaderSize(void) const {return _header_size;}
const size_t& HttpRequest::getRequestBodySize(void) const {return _request_body_size;}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void HttpRequest::updateSize(void)
{
    _request_body_size += s_buffer.size();
};

void HttpRequest::clearRequestBodySize(void)
{
    _request_body_size = 0;
}
/*----------------------------------------SETTER----------------------------------------*/


/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void HttpRequest::appendToBuffer(const char *toAppend, ssize_t size)
{
    size_t  len;
    s_buffer.append(toAppend, size);
    if (_header_size == 0 && ((len = s_buffer.find(CRLF CRLF)) != std::string::npos))
        _header_size = len + 1;
}

int HttpRequest::handlePostMethod(IO& object)
{
    if (_start == true)
    {
        outfile.open("video.mp4");
        if (outfile.fail())
            return FORBIDEN;
        _start = false;
    }
    try
    {
        outfile.write(s_buffer.data(), s_buffer.size());
        updateSize();
        if (getRequestBodySize() >= _body)
        {
            std::cout << getRequestBodySize() << std::endl;
            object.setOptions(TcpServer::FINISH_BODY, SET);
            clearRequestBodySize();
            outfile.close();
            _start = true;
        }
        s_buffer.clear();
    }
    catch(const std::exception& e)
    {
        outfile.close();
        _start = true;
        clearRequestBodySize();
        return INTERNAL_SERVER_ERROR;
    }
    return 0;
}

int HttpRequest::parseRequest(IO& object)
{
    if (object.checkBits(TcpServer::CONTENT_LENGTH))
    {
        handlePostMethod(object);
        return 0;
    }
    else if (object.checkBits(TcpServer::TRANSFER_ENCODING))
    {
        return 0;
    }
    
    std::vector<std::string> headers = UtilityMethod::stringSpliter(s_buffer, CRLF);
    std::vector<std::string> header;
    std::map<std::string, std::string>::const_iterator _it_content;
    std::map<std::string, std::string>::const_iterator _it_transfert;
    size_t len = headers.size() - 1;

    
    header = UtilityMethod::stringSpliter(headers[0], " ");
    
    _headers[METHOD] =  header.size() > 0 ? header[0] : NO_VALUE;
    _headers[PATH] =  header.size() > 1 ? header[1] : NO_VALUE;
    _headers[VERSION] = header.size() > 2 ? header[2] : NO_VALUE;

    setMetod(TcpServer::getHttpMethod(header[0]));

    for (size_t i = 1; i < len - 1; i++)
    {
        header = UtilityMethod::stringSpliter(headers[i], ":");

        if (header.size())
        {
            if (i != 0)
                for (size_t i = 2; i < header.size(); i++)
                    header[1] += ":" + header[i];

            _headers[header[0]] = header.size() >= 2 ? header[1].erase(0, 1) : NO_VALUE;
        }
    }

    _it_content = _headers.find(CONTENT_LEN);
    _it_transfert = _headers.find(TRANSFERT_ENCODING);

    size_t lenq;
    if ((lenq = s_buffer.find(CRLF CRLF)) != std::string::npos)
    {
        lenq += 4;
        std::cout << s_buffer.substr(0, lenq);
        s_buffer.erase(0, lenq);
    }
    
    if (_it_transfert != _headers.end())
    {
        object.setOptions(TcpServer::TRANSFER_ENCODING, SET);

        if (s_buffer.find(END_CHUNK) != std::string::npos)
            object.setOptions(TcpServer::FINISH_BODY, SET);
    }
    
    if (_it_content != _headers.end())
    {
        object.setOptions(TcpServer::CONTENT_LENGTH, SET);
        
        setBodySize(_it_content -> second);
        
        if ((s_buffer.size()) >= _body)
            object.setOptions(TcpServer::FINISH_BODY, SET);
        return handlePostMethod(object);
    }
    return 0;
}

int HttpRequest::checkValidHeader(int _ws, struct epoll_event event) const
{
    (void)_ws;
    (void)event;
    
    return 0;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
