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
        HttpServer(const HttpServer& rhs);
        HttpServer& operator=(const HttpServer& rhs);
        ~HttpServer();

        enum
        {
            GET,
            POST,
            DELETE,
            ERROR,
            HEADER,
            LOCATION_BLOCK,
            ERROR_PAGE_SET,
            AUTO_INDEX_ = 8,
            FILE_UPLOAD_
        };

        /*GETTERS*/
        bool getCgiPath(const std::string& key, std::string& path);
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
        std::map<short int, std::string>& getErrorMaps();
        const int& getEpollWs(void) const ;

        /*SETTERS*/
        int addToErrorMap(const short int& error, std::string& file, const std::string& directory);
        void pushNewServer(const Server& server);
        void setBodySize(const size_t& body);
        void setIndex(const std::string& index);
        void setRootDir(const std::string& root_dir);
        void setRedirect(const std::string& redirect);
        void setIndexPath(const std::string& path);
        void setFullIndexPath(const std::string& full_index_path);
        void pushNewCGI(const std::string& key, const std::string& value);
        void setUploadsFilesFolder(const std::string& uploads_files_folders);

        /*MEMBER FUNCTION*/
        void settingUpServer(const char *filename);
        void runningUpServer(void);
        void makeServerServe(void);

        /*STATIC MEMBER FUNCTION*/
        static void initMimeTypes(void);
        static void initHttpResponses(void);
        static void initHttpMethods(void);
        static void initKnownDirectives(void);
        static void initknownLocationsDirectives(void);
        static const std::string& getMimeType(const std::string& key);
        static bool isKnownDirective(const std::string& directive);
        static bool isKnownLocationDirectives(const std::string& directive);
        static const vec_it getHttpResponse(const short int& code);
        static int getHttpMethod(const std::string& method);
        static int makeNonBlockingFd(const int& fd);
        static int getMethodIndex(const std::string& method);

        static void switch_off_signal(int signal);

        /*static var*/
        static short int g_signal;
    
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

    private:
        int _epoll_ws;
        std::vector<Server> _servers;
        static std::map<short int, std::string> _httpResponses;
        static std::map<std::string, bool> _knownDirectives;
        static std::map<std::string, bool> _knownLocationsDirectives;
        static std::map<std::string, short int> _httpMethods;
        static std::map<std::string, std::string> _mimeTypes;
};
# endif