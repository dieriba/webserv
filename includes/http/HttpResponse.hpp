# ifndef __HTTP_RESPONSE_HPP__
# define __HTTP_RESPONSE_HPP__

# include "HttpMessage.hpp"
<<<<<<< HEAD
# include "../BitsManipulation.hpp"
# include "../method/Method.hpp"
# include "../method/Error.hpp"


class HttpRequest;
class IO;

class HttpResponse: public HttpMessage, public BitsManipulation
=======
# include "../method/Method.hpp"
# include "../method/Error.hpp"

class HttpRequest;
class IO;

class HttpResponse: public HttpMessage
>>>>>>> origin/main
{
    public:
        HttpResponse();
        HttpResponse(const HttpResponse& rhs);
        HttpResponse& operator=(const HttpResponse& rhs);
        ~HttpResponse();

<<<<<<< HEAD
        /*ENUM OPTIONS*/
        enum options
        {
            STARTED,
            FIRST_READ,
            FILE,
            DIRECTORY,
            TRANSFERT_ENC,
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
=======
        /*GETTER*/
        Error& getErrorMethod(void);
        Method *getHttpMethod(void) const;

        /*SETTER*/
        void setMethodObj(Method *method);
    
        /*MEMBER FUNCTION*/
        void serveResponse(IO&, const HttpRequest&);

    private:
        Method  *_method;
        Error   _error;
>>>>>>> origin/main
};

# endif