# ifndef __POST_HPP__
# define __POST_HPP__

# include "Method.hpp"

class ClientSocketStream;
class HttpRequest;

class Post: public Method
{
    public:
        Post();
        Post(const Post&);
        Post& operator=(const Post&);
        virtual ~Post();

        /*GETTERS*/
        const size_t& getRequestBodySize(void) const;

        /*SETTERS*/
        void updateSize(const size_t&);

        /*MEMBER FUNCTION*/
        void clearRequestBodySize(void);
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        int handleCgiPost(ClientSocketStream&, HttpRequest&, HttpResponse&);
        int postCgiHandler(HttpRequest&, HttpResponse&);
        int writeToFile(HttpRequest&);
        int writeToFile(HttpRequest&, const size_t&, const size_t&);
        int writeToFileMutltipartData(HttpRequest&, const size_t&);

        /*VIRTUAL MEMBER FUNCTION*/
        virtual Method* clone(void) const;

    private:
        int handleMultipartData(ClientSocketStream&, HttpRequest&);
        size_t _request_body_size;
};

# endif