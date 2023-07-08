# ifndef __POST_HPP__
# define __POST_HPP__

# include "Method.hpp"
# include "../utils/FileWriter.hpp"

class ClientSocketStream;
class HttpRequest;

class Post: public Method, public FileWriter
{
    public:
        Post();
        Post(const Post&);
        Post& operator=(const Post&);
        virtual ~Post();

        /*MEMBER FUNCTION*/
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        int handleCgiPost(ClientSocketStream&, HttpRequest&, HttpResponse&);
        int postCgiHandler(HttpRequest&, HttpResponse&);

        /*VIRTUAL MEMBER FUNCTION*/
        virtual Method* clone(void) const;
};

# endif