# ifndef __HTTP_MESSAGE__
# define __HTTP_MESSAGE__

# include "../utils/CommonLib.hpp"

class HttpMessage
{
    public:
        HttpMessage();
        HttpMessage(const HttpMessage&);
        HttpMessage& operator=(const HttpMessage&);
        ~HttpMessage();

        /*GETTER*/
        const size_t& getBodySize(void) const;
        int getMethod(void) const;
        const std::string& getBuffer(void) const;
        std::string& getBuffer(void);
        const std::map<std::string, std::string>& getHeaders() const;
        std::map<std::string, std::string>& getHeaders();

        /*SETTER*/
        void setBodySize(const std::string&);
        void setBodySize(const size_t&);
        virtual void appendToBuffer(const char*, const ssize_t&);
        void setMetod(const int&);

        /*MEMBER FUNCTION*/
        virtual void clear(void) = 0;

    protected:
        size_t  _body;
        short int _method;
        std::string s_buffer;
        std::map <std::string, std::string> _headers;
};

# endif