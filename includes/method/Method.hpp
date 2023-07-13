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

        /*SETTERS*/
        void setMessage(const std::string&);

        /*MEMBER FUNCTION*/
        std::string getAllowedMethod(const HttpServer& instance, const std::map<std::string, short int>& _httpMethods);
        int handleFileRessource(ClientSocketStream&, HttpResponse&);
        int sendRedirect(const ClientSocketStream&, HttpResponse&, const char *);
        void makeStatusLine(IO&, const int&);
        void addEndHeaderCRLF(void);
        void setCookieHeader(IO&);
        void appendToResponse(const std::string&, const std::string&);
        void addCustomHeader(const HttpServer&);

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
    protected:
        std::string _response;
        std::string _message;
};

# endif