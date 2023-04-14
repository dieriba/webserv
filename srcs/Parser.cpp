#include "../includes/Parser.hpp"
#include "../includes/Server.hpp"
#include "../includes/ExceptionThrower.hpp"
#include "../includes/Location.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Parser::Parser(){};
Parser::Parser(const Parser& rhs):StringUtils(rhs){(void)rhs;};
Parser::~Parser(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

Location Parser::fillUpLocation(std::ifstream& file, bool bracket)
{
    Location _location;
    std::string line;

    (void)bracket;
    while (std::getline(file, line))
    {

    }
    return _location;
}

void    Parser::fillMap(const std::string& line, Server& server, std::map<std::string, std::string>& _serv_conf)
{
    std::vector<std::string> vec;

    vec = StringUtils::stringSpliter(line, WHITESPACES);
    
    if (!TcpServer::isKnownDirective(vec[0]))
        throw ExceptionThrower("Directive " + vec[0] + " is unknown");

    if (vec.size() == 1)
        throw ExceptionThrower(MISSING_TOO_MANY_KEY_VALUE);

    /*
        CHECK IF DIRECTIVES EXIST
        IF NOT THROW AN ERROR
    */
    if (vec.size() > 3 && vec[0] != SERVER_NAMES)
        throw ExceptionThrower("Directives " + vec[0] + " Has Too Many Arguments");
    
    if (vec[0] == SERVER_NAMES)
    {
        for (size_t i = 1; i < vec.size(); i++)
        {
            server.pushNewServerName(vec[i]);
           // std::cout << vec[i] << std::endl;
        }
    }
    else
    {
        /*
            CHECK IF DIRECTIVES FOLLOWS THE RIGHT FORMAT
            IF NOT THROW AN ERROR
        */
        if (vec.size() == 3)
        {
            if (vec.rbegin() -> length() != 1 || vec.rbegin() -> at(0) != ';')
                throw ExceptionThrower(LAST_ARGS);
            if (vec[1].find(';') != std::string::npos)
                throw ExceptionThrower(BAD_SYNTAX);
        }
        else
        {
            if (Parser::count(vec[1], ';') > 1 || vec[1].at(vec[1].size() - 1) != ';')
                throw ExceptionThrower(MISSING_SEMICOLONS);
            std::cout << "-------- " << vec[1] << std::endl;
            vec[1].erase(vec[1].length() - 1);
        }

        /*
            IF NO ERROR HAS BEEN THROWN
            ADD THE DIRECTIVE TO THE MAP
        */

         if (_serv_conf.find(vec[0]) != _serv_conf.end())
            throw ExceptionThrower("Redeclaration of the same directive");
        //std::cout << vec[0] << " " << vec[1] << std::endl;
        _serv_conf[vec[0]] = vec[1];
    }
}

Server Parser::fillServer(std::ifstream& file, bool bracket)
{
    std::string line;
    Server  server;
    std::map<std::string, std::string> _serv_conf;

    if (!bracket)
    {
        while (std::getline(file, line))
        {
            line = StringUtils::trimBothEnd(line, WHITESPACES);
            
            if (line.size())
            {
                if (line.size() == 1 && line.at(0) == '{')
                    break ;
                
                if (line.size() == 1)
                    throw ExceptionThrower("Missing Opening Bracket");
                
                if (StringUtils::count(line, '{') > 1)
                    throw ExceptionThrower("Too Many Opening Bracket");
                
                if (line.at(0) != '{')
                    throw ExceptionThrower("Opening Bracket Should Start The Line");
                
                line.erase(line.begin());
                fillMap(line, server, _serv_conf);
                break ;
            }

            if (file.eof()) throw ExceptionThrower(WRONG_FILE_FORMAT);
        }
    }

    if (file.eof()) throw ExceptionThrower(WRONG_FILE_FORMAT); 
    
    while (std::getline(file, line))
    {
        line = StringUtils::trimBothEnd(line, WHITESPACES);

        if (line.size())
        {
            if (line.find('}') != std::string::npos)
            {
                if (line.size() == 1 && line.at(0) == '}')
                    break ;

                if (StringUtils::count(line, '}') > 1)
                    throw ExceptionThrower("Too Many Closing Bracket");
                
                if (line.at(line.size() - 1) != '}')
                    throw ExceptionThrower("Missing Closing Bracket");
                
                line.erase(--line.end());
                fillMap(line, server, _serv_conf);
                break ;
            }
            if (line.find(LOCATION) != std::string::npos)
                server.pushNewLocation(fillUpLocation(file, line.find('}') != std::string::npos));
            else
                fillMap(line, server, _serv_conf);
        }
        
        if (file.eof()) break ;
    }

    return server;
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
std::vector<Server> Parser::getServerConfig(std::ifstream& file)
{
    std::string line;
    std::vector<Server> server;
    server.reserve(10);
    while (std::getline(file, line))
    {
        line = StringUtils::trimBothEnd(line, WHITESPACES);

        if (line.size())
        {
            if (line.find('}') != std::string::npos)
                    throw ExceptionThrower("BAD_SYNTAX");
            
            if (line.find(SERVER_CONTEXT) != std::string::npos)
            {
                std::cout << "get serv:"  << line <<  std::endl;
                server.push_back(fillServer(file, (line.find('{') != std::string::npos)));
                break ;
            }
            
            throw ExceptionThrower("Wrong Format of config file: " + line);
        }
        if (file.eof()) break ;
    }
    return server;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/