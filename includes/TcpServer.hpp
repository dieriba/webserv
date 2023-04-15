#ifndef __TcpServer_HPP_
#define __TcpServer_HPP_

#include "CommonLib.hpp"
#include "Parser.hpp"
class Server;
typedef std::map<short int, std::string>::iterator vec_it;
typedef std::map<std::string, short int>::iterator rev_it;
class TcpServer: public Parser
{
    public:
        /*CONSTRUCTOR/DESTRUCTOR/COPY/ASSIGNEMENT*/
        TcpServer();
        TcpServer(const TcpServer& rhs);
        TcpServer& operator=(const TcpServer& rhs);
        ~TcpServer();

        /*GETTERS*/
        std::vector<Server> getServers(void) const;
        const unsigned int& getBodySize(void) const;
        const std::string& getIndex(void) const;
        const std::string& getRootDir(void) const;
        const std::string& getRedirect(void) const;

        /*SETTERS*/
        void pushNewServer(const Server& server);
        void setBodySize(const unsigned int& body);
        void setIndex(const std::string& index);
        void setRootDir(const std::string& root_dir);
        void setRedirect(const std::string& redirect);

        /*MEMBER FUNCTION*/
        void settingUpServer(const char *filename);

        /*STATIC MEMBER FUNCTION*/
        static void initHttpResponses(void);
        static void initHttpMethods(void);
        static void initKnownDirectives(void);
        static void initknownLocationsDirectives(void);
        static bool isKnownDirective(const std::string& directive);
        static bool isKnownLocationDirectives(const std::string& directive);
        static const vec_it getHttpResponse(const short int& code);
        static int getHttpMethod(const std::string& method);
    protected:
        unsigned int _body_size;
        std::string _index;
        std::string _root_dir;
        std::string _redirect;

    private:
        std::vector<Server> _servers;
        static std::map<short int, std::string> httpResponses;
        static std::map<std::string, bool> knownDirectives;
        static std::map<std::string, bool> knownLocationsDirectives;
        static std::map<std::string, short int> httpMethods;
};
#endif