# ifndef __HTTP_RESPONSE_HPP__
# define __HTTP_RESPONSE_HPP__

# include "HttpMessage.hpp"
# include "../BitsManipulation.hpp"
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
            STARTED,
            FILE,
            DIRECTORY,
            FINISHED_RESPONSE
        };
        
        /*GETTER*/
        Error& getErrorMethod(void);
        Method *getHttpMethod(void) const;
        const DIR *getDirectory(void) const;
        std::ifstream& getFile(void);
        const std::string& getPath(void) const;

        /*SETTER*/
        void setPath(const std::string& ressource);
        void setMethodObj(Method *method);
    
        /*MEMBER FUNCTION*/
        void setDirectory(DIR* directory);
        void serveResponse(IO&, const HttpRequest&);

    private:
        std::string _path_req;
        Method  *_method;
        Error   _error;
        DIR     *_directory;
        std::ifstream _file;
};

# endif