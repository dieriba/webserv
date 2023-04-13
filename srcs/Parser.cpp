#include "../includes/Parser.hpp"
#include "../includes/Server.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Parser::Parser(){};
Parser::~Parser(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

Server Parser::fillServer(std::ifstream& file)
{
    Server  server;
    
    (void) file;
    server.setPort(2000);
    server.setServOption(GET, SET);
    return server;
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
std::vector<Server> Parser::getServerConfig(std::ifstream& file)
{
    std::string line;
    std::vector<Server> server;
    server.reserve(10);
    std::map<std::string, std::string> _serv_conf;
    while (std::getline(file, line))
    {
        std::cout << StringUtils::trimBothEnd(line, WHITESPACES) << '\n';
        /*if (line.find(SERVER_CONTEXT) != std::string::npos)
        {
            std::cout << "ENNTEREDDDDDDDDDDDDDDDD----\n";
            server.push_back(fillServer(file));
        }*/
        if (file.eof()) break ;
    }
    return server;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/