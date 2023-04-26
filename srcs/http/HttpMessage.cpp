# include "../../includes/http/HttpMessage.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpMessage::HttpMessage(){};
HttpMessage::HttpMessage(const HttpMessage& rhs):s_buffer(rhs.s_buffer){};
HttpMessage& HttpMessage::operator=(const HttpMessage& rhs)
{
    if (this == &rhs) return *this;
    s_buffer = rhs.s_buffer;
    return *this;
};
HttpMessage::~HttpMessage(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const std::string& HttpMessage::getBuffer(void) const {return s_buffer;}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void HttpMessage::appendToBuffer(const std::string& toAppend)
{
    s_buffer.append(toAppend);
}
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
