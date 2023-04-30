# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/StringUtils.hpp"
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
    Server *server = object.getServer();

    if (server -> checkBits(C_LEN))
    {
        if (s_buffer.size() == _body)
            server -> setOptions(FINISH_BODY, SET);
        return 0;
    }
    else if (server -> checkBits(T_ENC))
    {
        return 0;
    }
    
    std::vector<std::string> headers = StringUtils::stringSpliter(s_buffer, "\n");
    std::vector<std::string> header;
    std::map<std::string, std::string>::const_iterator _it_content;
    std::map<std::string, std::string>::const_iterator _it_transfert;
    size_t len = headers.size() - 1;

    
    header = StringUtils::stringSpliter(headers[0], " ");
    
    _headers["METHOD"] =  header.size() > 0 ? header[0] : NO_VALUE;
    _headers["PATH"] =  header.size() > 1 ? header[1] : NO_VALUE;
    _headers["VERSION"] = header.size() > 2 ? header[2] : NO_VALUE;

    setMetod(TcpServer::getHttpMethod(header[0]));

    for (size_t i = 1; i < len - 1; i++)
    {
        header = StringUtils::stringSpliter(headers[i], ":");

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

    s_buffer.erase(0, s_buffer.find(CRLF) + 4);

    if (_it_transfert != _headers.end())
    {
        server -> setOptions(T_ENC, SET);
        
    }
    
    if (_it_content != _headers.end())
    {
        server -> setOptions(C_LEN, SET);
        setBodySize(_it_content -> second);
        if ((s_buffer.size()) == _body)
            server -> setOptions(FINISH_BODY, SET);
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
