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

        /*MEMBER FUNCTION*/
<<<<<<< HEAD
        void sendResponse(IO&, const HttpRequest&, HttpResponse&);
=======
        void sendResponse(IO&, const HttpRequest&);
>>>>>>> origin/main
};

# endif