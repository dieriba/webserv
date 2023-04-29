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
        int getBodySize(void) const;
        int getMethod(void) const;
        const std::string& getBuffer(void) const;
        const std::map<std::string, std::string>& getHeaders() const;

        /*SETTER*/
        void setBodySize(const std::string& body) ;
        void appendToBuffer(const std::string& toAppend);
        void setMetod(const int& _method);
    protected:
        int  _body;
        short int _method;
        std::string s_buffer;
        std::map <std::string, std::string> _headers;
};

# endif