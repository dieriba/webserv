# ifndef __HTTP_MESSAGE__
# define __HTTP_MESSAGE__

class HttpMessage
{
    public:
        HttpMessage();
        HttpMessage(const HttpMessage& rhs);
        HttpMessage& operator=(const HttpMessage& rhs);
        ~HttpMessage();
    
    protected:
        /* data */
};

# endif