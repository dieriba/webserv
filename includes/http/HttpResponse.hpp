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
        HttpResponse(const HttpResponse& rhs);
        HttpResponse& operator=(const HttpResponse& rhs);
        ~HttpResponse();

        /*ENUM OPTIONS*/
        enum options
        {
            READ_END = -1,
            WRITE_END = -1,
            STARTED,
            FILE,
            DIRECTORY,
            FINISHED_RESPONSE,
            APPLICATION_FORM_URLENCODED,
            NO_ENCODING,
            REDIRECT_SET
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
        void setPath(const std::string& ressource);
        void setMethodObj(Method *method);
    
        /*MEMBER FUNCTION*/
        void clearReadEnd();
        void clearWriteEnd();
        void clearBothEnd();
        void clear(void);
        void setDirectory(DIR* directory);
        int serveResponse(IO&, HttpRequest&);
        int switchMethod(IO& event, const short int& method, const short int& status);

    private:
        int     _pipes[2];
        std::string _path_req;
        Method  *_method;
        Error   _error;
        DIR     *_directory;
        std::ifstream _file;
};

# endif