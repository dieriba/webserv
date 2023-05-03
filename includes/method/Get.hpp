# ifndef __GET_HPP__
# define __GET_HPP__

# include "Method.hpp"

class IO;
class HttpRequest;

class Get: public Method
{
    public:
        Get();
        Get(const Get& rhs);
        Get& operator=(const Get& rhs);
        virtual ~Get();

        /*MEMBER FUNCTION*/
        void sendResponse(IO&, const HttpRequest&);
    private:
        int handleFileRessource(IO& event, const HttpRequest& req, std::string& ressource);
        int handleDirectoryRessource(IO& event, DIR *directory);
};


# endif