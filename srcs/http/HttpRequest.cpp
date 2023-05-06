# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/utilityMethod.hpp"
# include "../../includes/TcpServer.hpp"
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpRequest::HttpRequest():HttpMessage(){};
HttpRequest::HttpRequest(const HttpRequest& rhs):HttpMessage(rhs){};
HttpRequest& HttpRequest::operator=(const HttpRequest& rhs)
{
    if (this == &rhs) return *this;
    s_buffer = rhs.s_buffer;
    return *this;
};
HttpRequest::~HttpRequest(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

int HttpRequest::parseRequest(IO& object)
{
    if (object.checkBits(TcpServer::CONTENT_LENGTH))
    {
        if (s_buffer.size() >= _body)
            object.setOptions(TcpServer::FINISH_BODY, SET);
        return 0;
    }
    else if (object.checkBits(TcpServer::TRANSFER_ENCODING))
    {
        return 0;
    }
    
    std::vector<std::string> headers = utilityMethod::stringSpliter(s_buffer, CRLF);
    std::vector<std::string> header;
    std::map<std::string, std::string>::const_iterator _it_content;
    std::map<std::string, std::string>::const_iterator _it_transfert;
    size_t len = headers.size() - 1;

    
    header = utilityMethod::stringSpliter(headers[0], " ");
    
    _headers[METHOD] =  header.size() > 0 ? header[0] : NO_VALUE;
    _headers[PATH] =  header.size() > 1 ? header[1] : NO_VALUE;
    _headers[VERSION] = header.size() > 2 ? header[2] : NO_VALUE;

    setMetod(TcpServer::getHttpMethod(header[0]));

    for (size_t i = 1; i < len - 1; i++)
    {
        header = utilityMethod::stringSpliter(headers[i], ":");

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

    s_buffer.erase(0, s_buffer.find(CRLF CRLF) + 4);

    if (_it_transfert != _headers.end())
    {
        object.setOptions(TcpServer::TRANSFER_ENCODING, SET);
        
    }
    
    if (_it_content != _headers.end())
    {
        object.setOptions(TcpServer::CONTENT_LENGTH, SET);
        setBodySize(_it_content -> second);
        if ((s_buffer.size()) == _body)
            object.setOptions(TcpServer::FINISH_BODY, SET);
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
