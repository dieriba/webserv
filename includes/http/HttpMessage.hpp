# ifndef __HTTP_MESSAGE__
# define __HTTP_MESSAGE__

# include "../CommonLib.hpp"

class HttpMessage
{
    public:
        HttpMessage();
        HttpMessage(const HttpMessage& rhs);
        HttpMessage& operator=(const HttpMessage& rhs);
        ~HttpMessage();

        /*GETTER*/
        const std::string& getBuffer(void) const;
        const size_t& getBodySize(void) const;

        /*SETTER*/
        void setBodySize(const std::string& body) ;
        void appendToBuffer(const std::string& toAppend);
    protected:
        int  _body;
        std::string s_buffer;
        std::map <std::string, std::string> _headers;
};

# endif