# include "../../includes/http/HttpMessage.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpMessage::HttpMessage(){};
HttpMessage::HttpMessage(const HttpMessage& rhs):_body(rhs._body),s_buffer(rhs.s_buffer),_headers(rhs._headers){};
HttpMessage& HttpMessage::operator=(const HttpMessage& rhs)
{
    if (this == &rhs) return *this;
    _body = rhs._body;
    s_buffer = rhs.s_buffer;
    _headers = rhs._headers;
    return *this;
};
HttpMessage::~HttpMessage(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const std::string& HttpMessage::getBuffer(void) const {return s_buffer;}
const size_t& HttpMessage::getBodySize(void) const {return _body;};
const std::map<std::string, std::string>& HttpMessage::getHeaders(void) const {return _headers;}
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
void HttpMessage::appendToBuffer(const std::string& toAppend) {s_buffer.append(toAppend);}
void HttpMessage::setBodySize(const std::string& body) 
{
    std::stringstream ss(body);
    int b;

    ss >> b;
    if (ss.fail())
        _body = -1;
    else
        _body = b;
}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
