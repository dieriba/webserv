# ifndef __HTTP_RESPONSE_HPP__
# define __HTTP_RESPONSE_HPP__

# include "HttpMessage.hpp"
# include "../utils/BitsManipulation.hpp"
# include "../method/Method.hpp"
# include "../method/Error.hpp"

class IO;
class HttpRequest;
class ClientSocketStream;

class HttpResponse: public HttpMessage, public BitsManipulation
{
    public:
        HttpResponse();
        HttpResponse(const HttpResponse&);
        HttpResponse& operator=(const HttpResponse&);
        ~HttpResponse();

        /*ENUM OPTIONS*/
        enum options
        {
            HTTP_RESPONSE_READ_END = -1,
            HTTP_RESPONSE_WRITE_END = -1,
            HTTP_RESPONSE_STARTED,
            HTTP_RESPONSE_FILE,
            HTTP_RESPONSE_DIRECTORY,
            HTTP_RESPONSE_FINISHED_RESPONSE,
            HTTP_RESPONSE_APPLICATION_FORM_URLENCODED,
            HTTP_RESPONSE_NO_ENCODING,
            HTTP_RESPONSE_REDIRECT_SET,
            HTTP_RESPONSE_RESSOURCE_EXIST
        };
        
        /*GETTER*/
        const std::string& getResponse(void) const;
        std::string& getResponse(void);
        const short int& getStatus(void) const;
        const int& getReadEnd(void) const;
        const int& getWriteEnd(void) const;
        int *getPipes(void);
        Error& getErrorMethod(void);
        Method *getHttpMethod(void) const;
        Method *getHttpMethod(void);
        const DIR *getDirectory(void) const;
        std::ifstream& getFile(void);
        const std::string& getPath(void) const;

        /*SETTER*/
        HttpResponse& appendToResponse(const std::string& message);
        HttpResponse& appendToResponse(const char* buffer, const size_t& size);
        HttpResponse& setStatus(const short int& status);
        HttpResponse& setHeader(const std::string&, const std::string&);
        HttpResponse& setCookieHeader(IO&);
        HttpResponse& addCustomHeader(ClientSocketStream&, const short int&);
        HttpResponse& addCustomHeader(const HttpServer&);
        HttpResponse& addEndHeaderCRLF(void);

        void setFd(const int& fd);
        void setPath(const std::string&);
        void setMethodObj(Method*);
    
        /*MEMBER FUNCTION*/
        int sendResponse(void);
        int sendResponse(const char *);
        int sendResponse(const char *, const size_t&);
        int sendJsonResponse(ClientSocketStream&, std::vector<std::string>&);
        int sendRedirect(const ClientSocketStream&, HttpResponse&, const char *);
        int serveResponse(ClientSocketStream&, HttpRequest&);
        int setErrorObjectResponse(const short int&);
        void clearReadEnd();
        void clearWriteEnd();
        void clearBothEnd();
        void clear(void);
        void setDirectory(DIR*);
        HttpResponse& makeStatusLine(IO&, const int&);
        HttpResponse& makeStatusLine(IO&);

    private:
        int     _fd;
        int     _pipes[2];
        short int _status;
        std::string _response;
        std::string _path_req;
        Method  *_method;
        Error   _error;
        DIR     *_directory;
        std::ifstream _file;
};

# endif