# include "../../includes/http/HttpMessage.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpMessage::HttpMessage():_body(0),_method(-1), s_buffer(""){};
HttpMessage::HttpMessage(const HttpMessage& rhs):_body(rhs._body),_method(rhs._method),s_buffer(rhs.s_buffer),_headers(rhs._headers){};
HttpMessage& HttpMessage::operator=(const HttpMessage& rhs)
{
    if (this == &rhs) return *this;
    _body = rhs._body;
    _method = rhs._method;
    s_buffer = rhs.s_buffer;
    _headers = rhs._headers;
    return *this;
};
HttpMessage::~HttpMessage(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const std::string& HttpMessage::getBuffer(void) const {return s_buffer;}
std::string& HttpMessage::getBuffer(void) {return s_buffer;}
const std::map<std::string, std::string>& HttpMessage::getHeaders(void) const {return _headers;}
std::map<std::string, std::string>& HttpMessage::getHeaders(void) {return _headers;}
const size_t& HttpMessage::getBodySize(void) const {return _body;};
int HttpMessage::getMethod(void) const {return _method;}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void HttpMessage::appendToBuffer(const char *toAppend, const ssize_t& size) 
{
    s_buffer.append(toAppend, size);
}

void HttpMessage::setBodySize(const std::string& body) 
{
    std::stringstream ss(body);
    size_t b;

    ss >> b;
    _body = ss.fail() ? std::string::npos : b;
}

void HttpMessage::setBodySize(const size_t& size)
{
    _body = size;
}

void HttpMessage::setMetod(const int& method) {_method = method;}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
