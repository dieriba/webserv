# ifndef __METHOD_HPP__
# define __METHOD_HPP__

# include "../CommonLib.hpp"

class IO;
class HttpRequest;
<<<<<<< HEAD
class HttpResponse;
=======
>>>>>>> origin/main

class Method
{
    public:
        Method();
        Method(const Method& rhs);
        Method& operator=(const Method& rhs);
        virtual ~Method();

        typedef Method* (*Factory)();

        /*GETTERS*/
        std::string& getResponse(void);

        /*MEMBER FUNCTION*/
        void makeStatusLine(const int& status);
        void appendToResponse(const std::string& key, const std::string& value);

        /*VIRTUAL FUNCTION*/
<<<<<<< HEAD
        virtual void sendResponse(IO&, const HttpRequest&, HttpResponse&) = 0;
=======
        virtual void sendResponse(IO&, const HttpRequest&) = 0;
>>>>>>> origin/main

        /*STATIC MEMBER FUNCTION*/
        static Method* createGet();
        static Method* createPost();
        static Method* createDelete();
        static Method* createError();
        static Factory _tab[4];

    protected:
        std::string _response;

};

# endif