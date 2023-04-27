# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/StringUtils.hpp"

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

void HttpRequest::parseRequest()
{
    std::vector<std::string> headers = StringUtils::stringSpliter(s_buffer, "\n");
    std::vector<std::string> header;
    std::map<std::string, std::string>::const_iterator it;
    size_t len = headers.size() - 1;
    
    for (size_t i = 0; i < len - 1; i++)
    {
        header = StringUtils::stringSpliter(headers[i], ":");

        if (header.size())
        {
            if (i != 0)
                for (size_t i = 2; i < header.size(); i++)
                    header[1] += ":" + header[i];

            _headers[header[0]] = header.size() >= 2 ? header[1].erase(0, 1) : "NO VALUE";
        }
    }

    it = _headers.find(CONTENT_LEN);
    setBodySize(it == _headers.end() ? "0" : it -> second);
}

int HttpRequest::checkValidHeader(int _ws, struct epoll_event event) const
{
    (void)_ws;
    (void)event;
    
    return 0;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
