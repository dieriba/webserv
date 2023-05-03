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

        void sendResponse(const IO&, const HttpRequest&);
    private:
        int handleFileRessource(const IO& event, const HttpRequest& req, std::string& ressource);
        int handleDirectoryRessource(const IO& event, DIR *directory);
};


# endif