# ifndef __GET_HPP__
# define __GET_HPP__

# include "Method.hpp"

class IO;
class HttpRequest;
class TcpServer;

class Get: public Method
{
    public:
        Get();
        Get(const Get& rhs);
        Get& operator=(const Get& rhs);
        virtual ~Get();

        /*MEMBER FUNCTION*/
<<<<<<< HEAD
        void sendResponse(IO&, const HttpRequest&, HttpResponse&);
        int firstStep(IO& event, const HttpRequest& req, HttpResponse& res);
        int handleChunkedRequest(HttpResponse& res, std::string& s_buffer);
    private:
        int handleFileRessource(IO& event, const HttpRequest& req, HttpResponse& res);
=======
        void sendResponse(IO&, const HttpRequest&);
    private:
        int handleFileRessource(TcpServer& instance, IO& event, const HttpRequest& req, std::string& ressource);
>>>>>>> origin/main
        int handleDirectoryRessource(IO& event, DIR *directory);
};


# endif