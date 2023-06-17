# ifndef __GET_HPP__
# define __GET_HPP__

# include "Method.hpp"

class IO;
class HttpRequest;
class HttpServer;

class Get: public Method
{
    public:
        Get();
        Get(const Get& rhs);
        Get& operator=(const Get& rhs);
        virtual ~Get();

        /*MEMBER FUNCTION*/
        int basicCgiSetup(IO& event, HttpResponse& res);
        int sendResponse(IO&, HttpRequest&, HttpResponse&);
        int getCgiHandler(IO& event, const HttpRequest& req, HttpResponse& res);
        int firstStep(IO& event, const HttpRequest& req, HttpResponse& res);
        int handleChunkedRequest(HttpResponse& res, std::string& s_buffer);
        int directoryCgi(IO& event, const HttpRequest& req, HttpResponse& res);

        /*VIRTUAL FUNCTION*/
        virtual Method* clone(void) const;
    private:
        int handleDirectoryRessource(IO& event, const HttpRequest& req, HttpResponse& res);
};


# endif