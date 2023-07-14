# ifndef __METHOD_HPP__
# define __METHOD_HPP__

# include "../utils/CommonLib.hpp"

class IO;
class ClientSocketStream;
class HttpRequest;
class HttpResponse;
class HttpServer;

class Method
{
    public:
        Method();
        Method(const Method&);
        Method& operator=(const Method&);
        virtual ~Method();

        typedef Method* (*Factory)();

        /*MEMBER FUNCTION*/
        int handleFileRessource(HttpResponse&);
        

        /*PURE VIRTUAL MEMBER FUNCTION*/
        virtual int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&) = 0;
        virtual Method* clone(void) const = 0;

        /*STATIC MEMBER FUNCTION*/
        static Method* createHead(void);
        static Method* createGet(void);
        static Method* createPost(void);
        static Method* createPut(void);
        static Method* createDelete(void);
        static Method* createOptions(void);
        static std::vector<Factory> _tab;

        /*STATIC VARIABLES*/
        static const short int INSERT;
};

# endif