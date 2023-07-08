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

        /*GETTERS*/
        std::string& getResponse(void);

        /*MEMBER FUNCTION*/
        int handleFileRessource(ClientSocketStream&, HttpResponse&);
        int sendRedirect(const ClientSocketStream&, HttpResponse&, const char *);
        void makeStatusLine(IO&, const int&);
        void addEndHeaderCRLF(void);
        void setCookieHeader(IO&);
        void appendToResponse(const std::string&, const std::string&);
        void addCustomHeader(const HttpServer&);

        /*VIRTUAL FUNCTION*/
        virtual Method* clone(void) const = 0;
        virtual int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&) = 0;

        /*STATIC MEMBER FUNCTION*/
        static Method* createHead(void);
        static Method* createGet(void);
        static Method* createPost(void);
        static Method* createPut(void);
        static Method* createDelete(void);
        static Method* createOptions(void);
        static std::vector<Factory> _tab;

    protected:
        std::string _response;
};

# endif