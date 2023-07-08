# ifndef __HTTP_RESPONSE_HPP__
# define __HTTP_RESPONSE_HPP__

# include "HttpMessage.hpp"
# include "../utils/BitsManipulation.hpp"
# include "../method/Method.hpp"
# include "../method/Error.hpp"


class HttpRequest;
class IO;

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
        void setPath(const std::string&);
        void setMethodObj(Method*);
    
        /*MEMBER FUNCTION*/
        void clearReadEnd();
        void clearWriteEnd();
        void clearBothEnd();
        void clear(void);
        void setDirectory(DIR*);
        int serveResponse(ClientSocketStream&, HttpRequest&);
        int setErrorObjectResponse(IO& client, const short int& status);

    private:
        int     _pipes[2];
        std::string _path_req;
        Method  *_method;
        Error   _error;
        DIR     *_directory;
        std::ifstream _file;
};

# endif