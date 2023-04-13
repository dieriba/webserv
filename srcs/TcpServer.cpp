#include "../includes/TcpServer.hpp"
#include "../includes/Server.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
TcpServer::TcpServer(const char *filename)
{
    std::ifstream file;
    _servers.reserve(BASE_VEC_ARR);
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit );
    filename != NULL ? file.open(filename) : file.open(DEFAULT_CONF_FILE);
    getServerConfig(file);
}
TcpServer::~TcpServer(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
std::vector<Server> TcpServer::getServers(void) const {return _servers; };
const vec_it TcpServer::getHttpResponse(const short int& response)
{
    return TcpServer::httpResponses.find(response);
}
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void TcpServer::pushNewServer(const Server& server)
{
    _servers.push_back(server);
};
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/
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

void TcpServer::getServerConfig(std::ifstream& file)
{
    std::string myline;
    while ( std::getline (file, myline) )
    {
        std::cout << myline << '\n';
        if (file.eof()) return ;
    }
}
/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/
std::map<short int, std::string> TcpServer::httpResponses;