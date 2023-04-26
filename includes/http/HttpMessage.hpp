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

        void appendToBuffer(const std::string& toAppend);
        const std::string& getBuffer(void) const;
    protected:
        std::string s_buffer;
};

# endif