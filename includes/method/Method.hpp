# ifndef __METHOD_HPP__
# define __METHOD_HPP__

# include "../utils/CommonLib.hpp"

class IO;
class HttpRequest;
class HttpResponse;

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
        int sendBuffer(int client_socket, const char *buffer, int bytes);
        int handleFileRessource(IO& event, HttpResponse& res);
        int sendRedirect(const IO& event, HttpResponse& res, const char *status_line);
        void makeStatusLine(IO& event, const int& status);
        void setCookieHeader(IO& event);
        void appendToResponse(const std::string& key, const std::string& value);

        /*VIRTUAL FUNCTION*/
        virtual Method* clone(void) const = 0;
        virtual int sendResponse(IO&, HttpRequest&, HttpResponse&) = 0;

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