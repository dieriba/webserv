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
        void create_file(std::string& filepath);

        /*VIRTUAL MEMBER FUNCTION*/
        virtual int open_file(ClientSocketStream&);
        virtual int open_file(ClientSocketStream&, std::string& filepath);
        virtual Method* clone(void) const;
};

# endif