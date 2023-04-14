#include "../includes/Parser.hpp"
#include "../includes/Server.hpp"
#include "../includes/ExceptionThrower.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Parser::Parser(){};
Parser::Parser(const Parser& rhs):StringUtils(rhs){(void)rhs;};
Parser::~Parser(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

void    Parser::fillMap(const std::string& line, std::map<std::string, std::string>& _serv_conf)
{
    std::vector<std::string> vec;

    vec = StringUtils::stringSpliter(line, WHITESPACES);
    
    if (vec.size() == 1 || vec.size() > 3)
        throw ExceptionThrower(MISSING_TOO_MANY_KEY_VALUE);
    
    /*
        CHECK IF DIRECTIVES EXIST
        IF NOT THROW AN ERROR
    */
    if (!TcpServer::isKnownDirective(vec[0]))
        throw ExceptionThrower("Directive" + vec[0] + "is unknown");
    if (vec[0] == LOCATION)
    {
        /**/
    }
    else
    {
        /*
            CHECK IF DIRECTIVES FOLLOWS THE RIGHT FORMAT
            IF NOT THROW AN ERROR
        */
        if (vec.size() == 3)
        {
            if (vec[2].length() != 1 || vec[2].at(0) != ';')
                throw ExceptionThrower(LAST_ARGS);
            if (vec[1].find(';') != std::string::npos)
                throw ExceptionThrower(BAD_SYNTAX);
        }
        else
        {
            if (Parser::count(vec[1], ';') > 1 || vec[1].at(vec[1].size() - 1) != ';')
                throw ExceptionThrower(MISSING_SEMICOLONS);
            vec[1].erase(vec[1].length() - 1);
        }
        /*
            IF NO ERROR HAS BEEN THROWN
            ADD THE DIRECTIVE TO THE MAP
        */
         if (_serv_conf.find(vec[0]) != _serv_conf.end())
            throw ExceptionThrower("Redeclaration of the same directive");
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
                fillMap(line, _serv_conf);
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
                fillMap(line, _serv_conf);
                break ;
            }

            fillMap(line, _serv_conf);
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