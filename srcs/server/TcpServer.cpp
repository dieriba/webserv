# include "../../includes/server/TcpServer.hpp"
# include "../../includes/server/Server.hpp"
# include "../../includes/utils/ExceptionThrower.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/IO/ServerStream.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"
# include "../../includes/IO/CgiStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
TcpServer::TcpServer():BitsManipulation(),_body_size(0),
            _index(""),_root_dir(""),_redirect(""),_epoll_ws(-1){};

TcpServer::TcpServer(const TcpServer& rhs)
    :Parser(rhs),BitsManipulation(rhs),_body_size(rhs._body_size),_index(rhs._index),
    _root_dir(rhs._root_dir),_redirect(rhs._redirect),_index_path(rhs._index_path),_error_pages(rhs._error_pages)
    ,_epoll_ws(rhs._epoll_ws),_servers(rhs._servers){};

TcpServer& TcpServer::operator=(const TcpServer& rhs)
{
    if (this == &rhs) return *this;
    _error_pages = rhs._error_pages;
    _body_size = rhs._body_size;
    _options = rhs._options;
    _index = rhs._index;
    _root_dir = rhs._root_dir;
    _redirect = rhs._redirect;
    _epoll_ws = rhs._epoll_ws;
    _servers = rhs._servers;
    return *this;
}

TcpServer::~TcpServer()
{
    if (_epoll_ws != -1) close(_epoll_ws);
};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
const int& TcpServer::getEpollWs(void) const {return _epoll_ws;}
const std::map<short int, std::string>& TcpServer::getErrorMaps() const {return _error_pages;}
std::map<short int, std::string>& TcpServer::getErrorMaps() {return _error_pages;}
std::vector<Server> TcpServer::getServers(void) const {return _servers;};
const size_t& TcpServer::getBodySize(void) const {return _body_size;};
const std::string& TcpServer::getRootDir(void) const {return _root_dir;};
const std::string& TcpServer::getIndex(void) const {return _index;};
const std::string& TcpServer::getIndexPath(void) const {return _index_path;};
const std::string& TcpServer::getFullIndexPath(void) const {return _full_index_path;};
const std::string& TcpServer::getRedirect(void) const {return _redirect;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

int TcpServer::addToErrorMap(const short int& error, std::string& file, const std::string& directory)
{
    if (_error_pages.find(error) != _error_pages.end())
        return -1;
    
    file = directory + (file[0] != '/' ? "/" + file : file);

    _error_pages[error] = file;
    return 0;
}

void TcpServer::pushNewServer(const Server& server)
{
    _servers.push_back(server);
};

void    TcpServer::setBodySize(const size_t& body)
{
    _body_size = body;
};

void    TcpServer::setRootDir(const std::string& root_dir)
{
    _root_dir = root_dir;
};

void    TcpServer::setIndex(const std::string& index)
{
    _index = index;
};

void    TcpServer::setRedirect(const std::string& redirect)
{
    _redirect = redirect;    
}

void    TcpServer::setIndexPath(const std::string& path)
{
    _index_path = path;    
}

void    TcpServer::setFullIndexPath(const std::string& path)
{
    _full_index_path = path;
}


/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void TcpServer::settingUpServer(const char *filename)
{
    std::ifstream file;   
    _servers.reserve(BASE_VEC_ARR);
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    filename != NULL ? file.open(filename) : file.open(DEFAULT_CONF_FILE);
    _servers = Parser::getServerConfig(file, this);
}

void TcpServer::runningUpServer(void)
{
    std::string message;
    struct epoll_event event;

    if ((_epoll_ws = epoll_create1(0)) == -1)
        throw ExceptionThrower("Failled to create an epoll instance");
    
    for (size_t i = 0; i < _servers.size(); i++)
    {
        message = _servers[i].launchServer();

        event.events = EPOLLIN;
        
        if (message.size()) throw ExceptionThrower(message);
        
        event.data.ptr = new ServerStream(_servers[i].getServSocket(), &_servers[i]);

        _servers[i].addToEventsMap((const IO *)event.data.ptr);
        
        if (epoll_ctl(_epoll_ws, EPOLL_CTL_ADD, _servers[i].getServSocket(), &event) == -1)
            throw ExceptionThrower("Failled To Add Socket To EPOLL WATCHERS FD");
        
        std::cout << "Server: " << i + 1 << " is listening on PORT: " << _servers[i].getPort() << std::endl; 
    }

}

void TcpServer::makeServerServe(void)
{
    int to_proceed;
    int res;

    struct epoll_event _events[MAXEVENTS];

    IO  *events;

    while (TcpServer::g_signal == 1)
    {
        to_proceed = epoll_wait(_epoll_ws, _events, MAXEVENTS, -1);
        for (int i = 0; i < to_proceed; i++)
        {
            events = static_cast<IO *>(_events[i].data.ptr);
            res = events -> handleIoOperation(_epoll_ws, _events[i]) == IO::IO_ERROR;
            if (res == IO::IO_ERROR)
            {
                std::cout << "ERROR" << std::endl;
                Server *server = events -> getServer();
                server -> deleteFromEventsMap(events);
            }
        }
    }
}


/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------STATIC FUNCTION----------------------------------------*/

int TcpServer::getHttpMethod(const std::string& method)
{
    rev_it it = _httpMethods.find(method);
    if (it == _httpMethods.end())
        return (-1);
    return (it -> second);
}

const std::string& TcpServer::getMimeType(const std::string& key)
{
    std::map<std::string, std::string>::iterator it = _mimeTypes.find(key);

    if (it == _mimeTypes.end())
        return _mimeTypes.find(DEFAULT) -> second;
    return it -> second;
}

const vec_it TcpServer::getHttpResponse(const short int& response)
{
    return _httpResponses.find(response);
}

bool TcpServer::isKnownDirective(const std::string& directive)
{
    return (_knownDirectives.find(directive) != _knownDirectives.end());
}

bool TcpServer::isKnownLocationDirectives(const std::string& directive)
{
    return (_knownLocationsDirectives.find(directive) != _knownDirectives.end());
}

void TcpServer::initknownLocationsDirectives(void)
{
    _knownLocationsDirectives[ROOT] = true;
    _knownLocationsDirectives[ALLOWED_METHOD] = true;
    _knownLocationsDirectives[INDEX] = true;
    _knownLocationsDirectives[REDIRECT] = true;
    _knownLocationsDirectives[ERROR_PAGE] = true;
    _knownLocationsDirectives[CLIENT_BODY] = true;
    _knownLocationsDirectives[LOCATION] = true;
    _knownLocationsDirectives[ROOT_ERROR_PAGE] = true;
}   

void TcpServer::initKnownDirectives(void)
{
    _knownDirectives[ROOT_ERROR_PAGE] = true;
    _knownDirectives[LISTEN] = true;
    _knownDirectives[SERVER_NAMES] = true;
    _knownDirectives[ROOT] = true;
    _knownDirectives[ALLOWED_METHOD] = true;
    _knownDirectives[INDEX] = true;
    _knownDirectives[CLIENT_BODY] = true;
    _knownDirectives[LOCATION] = true;
    _knownDirectives[ERROR_PAGE] = true;
    _knownDirectives[REDIRECT] = true;
    _knownDirectives[CGI] = true;
    _knownLocationsDirectives[ROOT_ERROR_PAGE] = true;
}

void TcpServer::initHttpResponses(void)
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

void TcpServer::initMimeTypes(void)
{
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
}

void TcpServer::initHttpMethods(void)
{
    _httpMethods["GET"] = GET;
    _httpMethods["POST"] = POST;
    _httpMethods["DELETE"] = DELETE;
}

int TcpServer::getMethodIndex(const std::string& method)
{
    std::map<std::string, short int>::iterator it = _httpMethods.find(method);
    if (it == _httpMethods.end()) return -1;
    return it -> second;
}

/*
    @brief 
*/

int TcpServer::makeNonBlockingFd(const int& fd)
{
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == -1)
        return -1;
    return 0;
}

void TcpServer::switch_off_signal(int)
{
    TcpServer::g_signal = 0;
}

/*----------------------------------------STATIC FUNCTION----------------------------------------*/
short int TcpServer::g_signal = 1;
std::map<short int, std::string> TcpServer::_httpResponses;
std::map<std::string, bool> TcpServer::_knownDirectives;
std::map<std::string, bool> TcpServer::_knownLocationsDirectives;
std::map<std::string, short int> TcpServer::_httpMethods;
std::map<std::string, std::string> TcpServer::_mimeTypes;