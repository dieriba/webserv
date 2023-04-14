#include "../includes/TcpServer.hpp"
#include "../includes/Server.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
TcpServer::TcpServer():_body_size(0),_index(""),_root_dir(""),_redirect(""){};

TcpServer::TcpServer(const TcpServer& rhs)
    :Parser(rhs),_body_size(rhs._body_size),_index(rhs._index),_root_dir(rhs._root_dir),_redirect(rhs._redirect){};

TcpServer& TcpServer::operator=(const TcpServer& rhs)
{
    if (this == &rhs) return *this;
    _body_size = rhs._body_size;
    _index = rhs._index;
    _root_dir = rhs._root_dir;
    _redirect = rhs._redirect;
    return *this;
}
TcpServer::~TcpServer(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
std::vector<Server> TcpServer::getServers(void) const {return _servers; };
const unsigned int& TcpServer::getBodySize(void) const {return _body_size;};
const std::string& TcpServer::getRootDir(void) const {return _root_dir;};
const std::string& TcpServer::getIndex(void) const {return _index;};
const std::string& TcpServer::getRedirect(void) const {return _redirect;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void TcpServer::pushNewServer(const Server& server)
{
    _servers.push_back(server);
};

void    TcpServer::setBodySize(const unsigned int& body)
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
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

void TcpServer::settingUpServer(const char *filename)
{
    std::ifstream file;   
    _servers.reserve(BASE_VEC_ARR);
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    filename != NULL ? file.open(filename) : file.open(DEFAULT_CONF_FILE);
    _servers = Parser::getServerConfig(file);
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------STATIC FUNCTION----------------------------------------*/

int TcpServer::getHttpMethod(const std::string& method)
{
    rev_it it = httpMethods.find(method);
    if (it == httpMethods.end())
        return (-1);
    return (it -> second);
}

const vec_it TcpServer::getHttpResponse(const short int& response)
{
    return httpResponses.find(response);
}

bool TcpServer::isKnownDirective(const std::string& directive)
{
    return (knownDirectives.find(directive) != knownDirectives.end());
}

void TcpServer::initKnownDirectives(void)
{
    knownDirectives[LISTEN] = true;
    knownDirectives[SERVER_NAMES] = true;
    knownDirectives[ROOT] = true;
    knownDirectives[ALLOWED_METHOD] = true;
    knownDirectives[INDEX] = true;
    knownDirectives[CLIENT_BODY] = true;
    knownDirectives[LOCATION] = true;
    knownDirectives[ERROR_FILE] = true;
    knownDirectives[REDIRECT] = true;
    knownDirectives[CGI] = true;
}

void TcpServer::initHttpResponses(void)
{
    httpResponses[100] = "Continue";
    httpResponses[101] = "Switching Protocols";
    httpResponses[102] = "Processing";
    httpResponses[103] = "Checkpoint";
    httpResponses[122] = "Request-URI too long ";

    httpResponses[200] = "OK";
    httpResponses[201] = "Created";
    httpResponses[202] = "Accepted";
    httpResponses[203] = "Non-Authoritative Information";
    httpResponses[204] = "No Content";
    httpResponses[205] = "Reset Content";
    httpResponses[206] = "Partial Content";
    httpResponses[207] = "Multi-Status";
    httpResponses[208] = "Already Reported";
    httpResponses[226] = "IM Used";
    
    httpResponses[300] = "Multiple Choices";
    httpResponses[301] = "Moved Permanently";
    httpResponses[302] = "Found";
    httpResponses[303] = "See Other";
    httpResponses[304] = "Not Modified";
    httpResponses[305] = "Use Proxy";
    httpResponses[306] = "Switch Proxy";
    httpResponses[307] = "Temporary Redirect";
    httpResponses[308] = "Permanenet Redirect";
    
    httpResponses[400] = "Bad Request";
    httpResponses[401] = "Unauthorized";
    httpResponses[402] = "Payment Required";
    httpResponses[403] = "Forbidden";
    httpResponses[404] = "Not Found";
    httpResponses[405] = "Method Not Allowed";
    httpResponses[406] = "Not Acceptable";
    httpResponses[407] = "Proxy Authentification Required";
    httpResponses[408] = "Request Timeout";
    httpResponses[409] = "Conflict";
    httpResponses[410] = "Gone";
    httpResponses[411] = "Length Required";
    httpResponses[412] = "Precondition Failed";
    httpResponses[413] = "Request Entity Too Large";
    httpResponses[414] = "Request-URI Too Long";
    httpResponses[429] = "Too Many Requests";
    httpResponses[431] = "Request Header Fields Too Large";
    httpResponses[444] = "No Response";
    httpResponses[499] = "Client Closed Request";

    httpResponses[500] = "Internal Server Error";
    httpResponses[501] = "Not Implemented";
    httpResponses[502] = "Bad Gateway";
    httpResponses[503] = "Service Unavailable";
    httpResponses[504] = "Gateway Timeout";
    httpResponses[505] = "HTTP Version Not Supported";
    httpResponses[507] = "Insufficient Storage";
    httpResponses[508] = "Loop Detected";
    httpResponses[509] = "Bandwidth Limit Exceeded";
    httpResponses[510] = "Not Extended";
    httpResponses[511] = "Network Authentification Required";
}

void TcpServer::initHttpMethods(void)
{
    httpMethods["GET"] = GET;
    httpMethods["POST"] = POST;
    httpMethods["DELETE"] = DELETE;
}

/*
    @brief 
*/

/*----------------------------------------STATIC FUNCTION----------------------------------------*/
std::map<short int, std::string> TcpServer::httpResponses;
std::map<std::string, bool> TcpServer::knownDirectives;
std::map<std::string, short int> TcpServer::httpMethods;