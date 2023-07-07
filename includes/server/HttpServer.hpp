#ifndef __HttpServer_HPP_
#define __HttpServer_HPP_

# include "../utils/CommonLib.hpp"
# include "../utils/Parser.hpp"
# include "../utils/BitsManipulation.hpp"

class Server;
class IO;
typedef std::map<short int, std::string>::iterator vec_it;
typedef std::map<std::string, short int>::iterator rev_it;
class HttpServer: public Parser, public BitsManipulation
{
    public:
        /*CONSTRUCTOR/DESTRUCTOR/COPY/ASSIGNEMENT*/
        HttpServer();
        HttpServer(const HttpServer&);
        HttpServer& operator=(const HttpServer&);
        ~HttpServer();

        enum
        {
            HTTP_SERVER_HEAD,
            HTTP_SERVER_GET,
            HTTP_SERVER_POST,
            HTTP_SERVER_DELETE,
            HTTP_SERVER_HEADER,
            HTTP_SERVER_LOCATION_BLOCK,
            HTTP_SERVER_ERROR_PAGE_SET,
            HTTP_SERVER_AUTO_INDEX_ = 8,
            HTTP_SERVER_FILE_UPLOAD_,
        };

        /*GETTERS*/
        bool getCgiPath(const std::string&, std::string&);
        std::vector<Server> getServers(void) const;
        const size_t& getBodySize(void) const;
        const std::string& getIndex(void) const;
        const std::string& getRootDir(void) const;
        const std::string& getRedirect(void) const;
        const std::string& getIndexPath(void) const;
        const std::string& getFullIndexPath(void) const;
        const std::string& getUploadsFilesFolder(void) const;
        const std::map<short int, std::string>& getErrorMaps() const;
        const std::map<std::string, std::string>& getCgiMap() const;
        const std::map<std::string, std::string>& getHeadersMap() const;
        std::map<short int, std::string>& getErrorMaps();
        std::map<std::string, std::string>& getCgiMap();
        const int& getEpollWs(void) const ;

        /*SETTERS*/
        int addToErrorMap(const short int&, std::string&, const std::string&);
        void pushNewServer(const Server&);
        void setBodySize(const size_t&);
        void setIndex(const std::string&);
        void setRootDir(const std::string&);
        void setRedirect(const std::string&);
        void setIndexPath(const std::string&);
        void setFullIndexPath(const std::string&);
        void pushNewCGI(const std::string&, const std::string&);
        void setUploadsFilesFolder(const std::string&);

        /*MEMBER FUNCTION*/
        void settingUpServer(const char *);
        void runningUpServer(void);
        void makeServerServe(void);

        /*STATIC MEMBER FUNCTION*/
        void setWs(const int&);
        static void initMimeTypes(void);
        static void initHttpResponses(void);
        static void initHttpMethods(void);
        static void initKnownDirectives(void);
        static void initknownLocationsDirectives(void);
        static const std::string& getMimeType(const std::string&);
        static bool isKnownDirective(const std::string&);
        static bool isKnownLocationDirectives(const std::string&);
        static const vec_it getHttpResponse(const short int&);
        static int getHttpMethod(const std::string&);
        static int makeNonBlockingFd(const int&);
        static int getMethodIndex(const std::string&);
        static Server* getHostnameServerMap(const unsigned int&, const std::string&);
        static void setUpServerNameToServerMap(std::vector<Server>&);

        static void switch_off_signal(int);

        /*static var*/
        static short int number_of_methods;
        static unsigned int _all_methods;
        static short int g_signal;
        static std::map<std::string, short int> _httpMethods;
    
    protected:
        size_t _body_size;
        std::string _index;
        std::string _root_dir;
        std::string _redirect;
        std::string _index_path;
        std::string _full_index_path;
        std::string _upload_file_folders;
        std::map<short int, std::string> _error_pages;
        std::map<std::string, std::string> _cgi;
        std::map<std::string, std::string> _headers;

    private:
        int _epoll_ws;
        std::vector<Server> _servers;
        static std::map<short int, std::string> _httpResponses;
        static std::map<std::string, bool> _knownDirectives;
        static std::map<std::string, bool> _knownLocationsDirectives;
        static std::map<std::string, std::string> _mimeTypes;
        static std::map<unsigned int, std::map<std::string, Server*> > _serverNameToServer;
};
# endif