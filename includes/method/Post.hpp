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
        int handleCgiPost(ClientSocketStream&, HttpRequest&, HttpResponse&);
        int postCgiHandler(HttpRequest&, HttpResponse&);
        void create_file(std::string& filepath);

        /*VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int open_file(ClientSocketStream&);
        virtual int open_file(ClientSocketStream&, std::string& filepath);
 
        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        virtual Method* clone(void) const;
};

# endif