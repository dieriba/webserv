# ifndef __POST_HPP__
# define __POST_HPP__

# include "Method.hpp"

class IO;
class HttpRequest;

class Post: public Method
{
    public:
        Post();
        Post(const Post& rhs);
        Post& operator=(const Post& rhs);
        virtual ~Post();

        /*GETTERS*/
        const size_t& getRequestBodySize(void) const;

        /*SETTERS*/
        void updateSize(const size_t& size);

        /*MEMBER FUNCTION*/
        void clearRequestBodySize(void);
        int sendResponse(IO&, HttpRequest&, HttpResponse&);
        int postCgiHandler();
        int writeToFile(HttpRequest& req);
        int writeToFile(HttpRequest& req, const size_t& pos, const size_t& bytes);
        int writeToFileMutltipartData(HttpRequest& req, const size_t& bytes);

    private:
        int handleMultipartData(IO& event, HttpRequest& req);
        size_t _request_body_size;
};

# endif