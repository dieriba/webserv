# include "../../includes/server/HttpServer.hpp"
# include "../../includes/server/Server.hpp"
# include "../../includes/utils/ExceptionThrower.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/IO/CgiStream.hpp"
# include "../../includes/IO/ServerStream.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
HttpServer::HttpServer():BitsManipulation(),_body_size(0),
            _index(""),_root_dir(""),_redirect(""),_all_available_method_allowed(0),_epoll_ws(-1){};

HttpServer::HttpServer(const HttpServer& rhs)
    :Parser(rhs),BitsManipulation(rhs),_body_size(rhs._body_size),_index(rhs._index),
    _root_dir(rhs._root_dir),_redirect(rhs._redirect),_index_path(rhs._index_path),_upload_file_folders(rhs._upload_file_folders)
    ,_error_pages(rhs._error_pages),_cgi(rhs._cgi),_headers(rhs._headers),_all_available_method_allowed(rhs._all_available_method_allowed),_epoll_ws(rhs._epoll_ws),_servers(rhs._servers){};

HttpServer& HttpServer::operator=(const HttpServer& rhs)
{
    if (this == &rhs) return *this;
    _upload_file_folders = rhs._upload_file_folders;
    _error_pages = rhs._error_pages;
    _body_size = rhs._body_size;
    _options = rhs._options;
    _index = rhs._index;
    _headers = rhs._headers;
    _cgi = rhs._cgi;
    _all_available_method_allowed = rhs._all_available_method_allowed;
    _root_dir = rhs._root_dir;
    _redirect = rhs._redirect;
    _epoll_ws = rhs._epoll_ws;
    _servers = rhs._servers;
    return *this;
}

HttpServer::~HttpServer()
{
    if (_epoll_ws != -1) close(_epoll_ws);
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
std::map<short int, std::string>& HttpServer::getErrorMaps() {return _error_pages;}
std::vector<Server> HttpServer::getServers(void) const {return _servers;};
std::map<std::string, std::string>& HttpServer::getCgiMap() {return _cgi;}
const int& HttpServer::getEpollWs(void) const {return _epoll_ws;}
const std::map<std::string, std::string>& HttpServer::getCgiMap() const {return _cgi;}
const std::map<std::string, std::string>& HttpServer::getHeadersMap() const {return _headers;}
std::map<std::string, std::string>& HttpServer::getHeadersMap() {return _headers;}
const std::map<short int, std::string>& HttpServer::getErrorMaps() const {return _error_pages;}
const size_t& HttpServer::getBodySize(void) const {return _body_size;};
const std::string& HttpServer::getRootDir(void) const {return _root_dir;};
const std::string& HttpServer::getIndex(void) const {return _index;};
const std::string& HttpServer::getIndexPath(void) const {return _index_path;};
const std::string& HttpServer::getFullIndexPath(void) const {return _full_index_path;};
const std::string& HttpServer::getRedirect(void) const {return _redirect;};
const std::string& HttpServer::getUploadsFilesFolder(void) const {return _upload_file_folders;}
const uint32_t& HttpServer::getAllAvailableMethod(void) const { return _all_available_method_allowed ;}
bool HttpServer::getCgiPath(const std::string& key, std::string& path)
{
    it_map it = _cgi.find(key);
    path = (it != _cgi.end()) ? it -> second : "";
    return it != _cgi.end();
};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

int HttpServer::addToErrorMap(const short int& error, std::string& file, const std::string& directory)
{
    if (_error_pages.find(error) != _error_pages.end())
        return -1;
    
    file = directory + (file[0] != '/' ? "/" + file : file);

    _error_pages[error] = file;
    return 0;
}

void HttpServer::setWs(const int& ws)
{
    _epoll_ws = ws;
}

void HttpServer::pushNewHeaderDirective(const std::string& key, const std::string& value)
{
    _headers[key] = value;
}

void HttpServer::setUploadsFilesFolder(const std::string& uploads_files_foler)
{
    _upload_file_folders = uploads_files_foler;
}

void HttpServer::pushNewServer(const Server& server)
{
    _servers.push_back(server);
};

void    HttpServer::setBodySize(const size_t& body)
{
    _body_size = body;
};

void    HttpServer::setRootDir(const std::string& root_dir)
{
    _root_dir = root_dir;
};

void    HttpServer::setIndex(const std::string& index)
{
    _index = index;
};

void    HttpServer::setRedirect(const std::string& redirect)
{
    _redirect = redirect;    
}

void    HttpServer::setIndexPath(const std::string& path)
{
    _index_path = path;    
}

void    HttpServer::setFullIndexPath(const std::string& path)
{
    _full_index_path = path;
}

void    HttpServer::pushNewCGI(const std::string& key, const std::string& value)
{
    _cgi[key] = value;
}

void    HttpServer::setAllAvailableMethod(const size_t& option)
{
    bitset(_all_available_method_allowed, option);
}

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void HttpServer::settingUpServer(const char *filename)
{
    std::ifstream file;   
    _servers.reserve(BASE_VEC_ARR);
    file.exceptions(std::ifstream::badbit);
    
    filename != NULL ? file.open(filename) : file.open(DEFAULT_CONF_FILE);

    if (file.is_open() == false) throw ExceptionThrower("Failled to open file");
    
    if (file.peek() == std::ifstream::traits_type::eof()) throw ExceptionThrower("File is empty");

    _servers = Parser::getServerConfig(file, this);
    
    HttpServer::setUpServerNameToServerMap(_servers);
    
}

void HttpServer::setUpServerNameToServerMap(std::vector<Server>& servers)
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        const std::vector<std::string>& server_names = servers[i].getServerNames();
        
        if (server_names.size() == 0) continue;

        const unsigned int& port = servers[i].getPort();
        
        for (size_t j = 0; j < server_names.size(); j++)
            HttpServer::_serverNameToServer[port].insert(std::make_pair(server_names[j], &servers[i]));
    }
    
    std::map<unsigned int, std::map<std::string, Server*> >::iterator it = HttpServer::_serverNameToServer.begin();
    
    for (; it != HttpServer::_serverNameToServer.end(); it++)
    {
        std::map<std::string, Server*>::iterator server_it = it -> second.begin();

        Server *server = server_it -> second;

        for (; server_it != it -> second.end(); server_it++)
        {
            if (server_it -> first == DEFAULT_SERVER || server_it -> first.size() == 0)
            {
                server = server_it -> second;
                break ;
            }
        }

        HttpServer::_serverNameToServer[it -> first].insert(std::make_pair(DEFAULT_SERVER, server));
    }
}

void HttpServer::runningUpServer(void)
{
    std::string message;
    struct epoll_event event;

    if ((_epoll_ws = epoll_create1(0)) == -1)
        throw ExceptionThrower("Failled to create an epoll instance");

    for (size_t i = 0; i < _servers.size(); i++)
    {
        message = _servers[i].launchServer();

        event.events = EPOLLIN;
        
        _servers[i].setWs(_epoll_ws);

        if (message.size()) throw ExceptionThrower(message);
        
        event.data.ptr = new ServerStream(_servers[i].getServSocket(), &_servers[i]);

        _servers[i].addToEventsMap(static_cast<IO*>(event.data.ptr));
        
        if (epoll_ctl(_epoll_ws, EPOLL_CTL_ADD, _servers[i].getServSocket(), &event) == -1)
            throw ExceptionThrower("Failled To Add Socket To EPOLL WATCHERS FD");
        
        std::cout << "Server: " << i + 1 << " is listening on PORT: " << _servers[i].getPort() << std::endl; 
    }

}

void HttpServer::makeServerServe(void)
{
    int to_proceed;
    int res;

    struct epoll_event _events[MAXEVENTS];

    IO  *client;

    while (HttpServer::g_signal == 1)
    {
        to_proceed = epoll_wait(_epoll_ws, _events, MAXEVENTS, EPOLL_TIMEOUT);

        if (HttpServer::g_signal == 1 && to_proceed == IO::IO_ERROR) throw ExceptionThrower("Epoll_wait failled");

        for (int i = 0; i < to_proceed; i++)
        {
            client = static_cast<IO *>(_events[i].data.ptr);

            if (client -> checkBits(IO::IO_KILL_CGI))
            {
                std::cout << "Killing myself Remember me as fd: " << client -> getFd() << " from interests list" << std::endl;
                CgiStream* cgi = static_cast<CgiStream*>(client -> getIO());
                kill(cgi -> getPid(), SIGTERM);
                client -> getBaseServer() -> deleteFromEventsMap(*client);
                continue ;
            }

            res = client -> handleIoOperation(_epoll_ws, _events[i]);

            if (res == IO::IO_ERROR)
            {
                std::cout << "Deletting client with fd: " << client -> getFd() << " from interests list" << std::endl;
                client -> getBaseServer() -> deleteFromEventsMap(*client);
            }
        }
    }
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------STATIC FUNCTION----------------------------------------*/

int HttpServer::getHttpMethod(const std::string& method)
{
    rev_it it = _httpMethods.find(method);
    if (it == _httpMethods.end())
        return (-1);
    return (it -> second);
}

const std::string& HttpServer::getMimeType(const std::string& key)
{
    std::map<std::string, std::string>::iterator it = _mimeTypes.find(key);

    if (it == _mimeTypes.end())
        return _mimeTypes.find(MIME_PLAIN) -> second;
    return it -> second;
}

const vec_it HttpServer::getHttpResponse(const short int& response)
{
    return _httpResponses.find(response);
}

Server* HttpServer::getHostnameServerMap(const unsigned int& port, const std::string& server_name)
{
    std::map<unsigned int, std::map<std::string, Server*> >::iterator it = _serverNameToServer.find(port); 

    if (it == _serverNameToServer.end()) return NULL;

    std::map<std::string, Server*>::iterator server = it -> second.find(server_name);
        
    return server == it -> second.end() ? it -> second.find(DEFAULT_SERVER) -> second : server -> second;
}

bool HttpServer::isKnownDirective(const std::string& directive)
{
    return (_knownDirectives.find(directive) != _knownDirectives.end());
}

bool HttpServer::isKnownLocationDirectives(const std::string& directive)
{
    return (_knownLocationsDirectives.find(directive) != _knownDirectives.end());
}

void HttpServer::initknownLocationsDirectives(void)
{
    _knownLocationsDirectives[AUTO_INDEX];
    _knownLocationsDirectives[ADD_HEADER];
    _knownLocationsDirectives[ALIAS];
    _knownLocationsDirectives[ROOT];
    _knownLocationsDirectives[ALLOWED_METHOD];
    _knownLocationsDirectives[INDEX];
    _knownLocationsDirectives[REDIRECT];
    _knownLocationsDirectives[ERROR_PAGE];
    _knownLocationsDirectives[CLIENT_BODY];
    _knownLocationsDirectives[LOCATION];
    _knownLocationsDirectives[ROOT_ERROR_PAGE];
    _knownDirectives[UPLOAD_FILE_FOLDERS];
    _knownDirectives[FILE_UPLOAD];
}   

void HttpServer::initKnownDirectives(void)
{
    _knownDirectives[AUTO_INDEX];
    _knownDirectives[ALIAS];
    _knownDirectives[ADD_HEADER];
    _knownDirectives[ROOT_ERROR_PAGE];
    _knownDirectives[LISTEN];
    _knownDirectives[SERVER_NAMES];
    _knownDirectives[ROOT];
    _knownDirectives[ALLOWED_METHOD];
    _knownDirectives[INDEX];
    _knownDirectives[CLIENT_BODY];
    _knownDirectives[LOCATION];
    _knownDirectives[ERROR_PAGE];
    _knownDirectives[REDIRECT];
    _knownDirectives[CGI];
    _knownDirectives[ROOT_ERROR_PAGE];
    _knownDirectives[UPLOAD_FILE_FOLDERS];
    _knownDirectives[FILE_UPLOAD];
}

void HttpServer::initHttpResponses(void)
{
    _httpResponses[100] = "Continue";
    _httpResponses[101] = "Switching Protocols";
    _httpResponses[102] = "Processing";
    _httpResponses[103] = "Checkpoint";
    _httpResponses[122] = "Request-URI too long ";

    _httpResponses[200] = "OK";
    _httpResponses[201] = "Created";
    _httpResponses[202] = "Accepted";
    _httpResponses[203] = "Non-Authoritative Information";
    _httpResponses[204] = "No Content";
    _httpResponses[205] = "Reset Content";
    _httpResponses[206] = "Partial Content";
    _httpResponses[207] = "Multi-Status";
    _httpResponses[208] = "Already Reported";
    _httpResponses[226] = "IM Used";
    
    _httpResponses[300] = "Multiple Choices";
    _httpResponses[301] = "Moved Permanently";
    _httpResponses[302] = "Found";
    _httpResponses[303] = "See Other";
    _httpResponses[304] = "Not Modified";
    _httpResponses[305] = "Use Proxy";
    _httpResponses[306] = "Switch Proxy";
    _httpResponses[307] = "Temporary Redirect";
    _httpResponses[308] = "Permanenet Redirect";
    
    _httpResponses[400] = "Bad Request";
    _httpResponses[401] = "Unauthorized";
    _httpResponses[402] = "Payment Required";
    _httpResponses[403] = "Forbidden";
    _httpResponses[404] = "Not Found";
    _httpResponses[405] = "Method Not Allowed";
    _httpResponses[406] = "Not Acceptable";
    _httpResponses[407] = "Proxy Authentification Required";
    _httpResponses[408] = "Request Timeout";
    _httpResponses[409] = "Conflict";
    _httpResponses[410] = "Gone";
    _httpResponses[411] = "Length Required";
    _httpResponses[412] = "Precondition Failed";
    _httpResponses[413] = "Request Entity Too Large";
    _httpResponses[414] = "Request-URI Too Long";
    _httpResponses[429] = "Too Many Requests";
    _httpResponses[431] = "Request Header Fields Too Large";
    _httpResponses[444] = "No Response";
    _httpResponses[499] = "Client Closed Request";

    _httpResponses[500] = "Internal Server Error";
    _httpResponses[501] = "Not Implemented";
    _httpResponses[502] = "Bad Gateway";
    _httpResponses[503] = "Service Unavailable";
    _httpResponses[504] = "Gateway Timeout";
    _httpResponses[505] = "HTTP Version Not Supported";
    _httpResponses[507] = "Insufficient Storage";
    _httpResponses[508] = "Loop Detected";
    _httpResponses[509] = "Bandwidth Limit Exceeded";
    _httpResponses[510] = "Not Extended";
    _httpResponses[511] = "Network Authentification Required";
}

void HttpServer::initMimeTypes(void)
{
    std::ifstream file(MIME_FILENAME);

    if (!file)
        ExceptionThrower("Could not open the file");

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::string> vec = UtilityMethod::stringSpliter(line, WHITESPACES);

        if (vec.size() == 2)
        {
            if ((vec[0][0] == '.' && UtilityMethod::count(vec[0], '.') == 1) && (vec[1][0] != '/' && UtilityMethod::count(vec[1], '/') == 1))
            {
                _mimeTypes[vec[0]] = vec[1];
                _mimeTypes[vec[1]] = vec[0];
            }
        }

        if (file.eof()) break;
    }

    _mimeTypes[BIN] = MIME_BIN;
    _mimeTypes[MIME_BIN] = BIN; 
    _mimeTypes[DEFAULT] = MIME_PLAIN;
    _mimeTypes[HTM] = MIME_HTM;
    _mimeTypes[HTML] = MIME_HTML;
    _mimeTypes[SLASH] = MIME_HTML;
    _mimeTypes[CSS] = MIME_CSS;
    _mimeTypes[CSV] = MIME_CSV;
    _mimeTypes[JPEG] = MIME_JPEG;
    _mimeTypes[JPG] = MIME_JPG;
    _mimeTypes[JS] = MIME_JS;
    _mimeTypes[JSON] = MIME_JSON;
    _mimeTypes[PNG] = MIME_PNG;
    _mimeTypes[PDF] = MIME_PDF;
    _mimeTypes[PHP] = MIME_PHP;
    _mimeTypes[SH] = MIME_SH;
    _mimeTypes[MP4] = MIME_MP4;
    _mimeTypes[MIME_PLAIN] = "";
}

void HttpServer::initHttpMethods(void)
{
    _httpMethods["HEAD"] = HTTP_SERVER_HEAD;
    bitset(HttpServer::_all_methods, HTTP_SERVER_HEAD);

    _httpMethods["GET"] = HTTP_SERVER_GET;
    bitset(HttpServer::_all_methods, HTTP_SERVER_GET);

    _httpMethods["POST"] = HTTP_SERVER_POST;
    bitset(HttpServer::_all_methods, HTTP_SERVER_POST);

    _httpMethods["PUT"] = HTTP_SERVER_PUT;
    bitset(HttpServer::_all_methods, HTTP_SERVER_PUT);

    _httpMethods["OPTIONS"] = HTTP_SERVER_OPTIONS;
    bitset(HttpServer::_all_methods, HTTP_SERVER_OPTIONS);

    _httpMethods["DELETE"] = HTTP_SERVER_DELETE;
    bitset(HttpServer::_all_methods, HTTP_SERVER_DELETE);

}

int HttpServer::getMethodIndex(const std::string& method)
{
    std::map<std::string, short int>::iterator it = _httpMethods.find(method);
    if (it == _httpMethods.end()) return -1;
    return it -> second;
}

/*
    @brief 
*/

int HttpServer::makeNonBlockingFd(const int& fd)
{
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == -1)
        return -1;
    return 0;
}

void HttpServer::switch_off_signal(int)
{
    HttpServer::g_signal = 0;
}

/*----------------------------------------STATIC FUNCTION----------------------------------------*/

/*----------------------------------------STATIC VARIABLES----------------------------------------*/
short int HttpServer::g_signal = 1;
short int HttpServer::number_of_methods = 0;
unsigned int HttpServer::_all_methods = 0;
std::map<short int, std::string> HttpServer::_httpResponses;
std::map<std::string, bool> HttpServer::_knownDirectives;
std::map<std::string, bool> HttpServer::_knownLocationsDirectives;
std::map<std::string, short int> HttpServer::_httpMethods;
std::map<std::string, std::string> HttpServer::_mimeTypes;
std::map<unsigned int, std::map<std::string, Server*> > HttpServer::_serverNameToServer;
/*----------------------------------------STATIC VARIABLES----------------------------------------*/