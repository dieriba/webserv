# ifndef __METHOD_HPP__
# define __METHOD_HPP__

# include "../CommonLib.hpp"

class IO;
class HttpRequest;

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
        void makeStatusLine(int status);

        /*VIRTUAL FUNCTION*/
        virtual void sendResponse(IO&, const HttpRequest&) = 0;

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