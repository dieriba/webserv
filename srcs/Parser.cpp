#include "../includes/Parser.hpp"
#include "../includes/Server.hpp"
#include "../includes/ExceptionThrower.hpp"
#include "../includes/Location.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Parser::Parser(){};
Parser::Parser(const Parser& rhs):StringUtils(rhs){(void)rhs;};
Parser::~Parser(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

void    Parser::feedingUpServer(std::map<std::string, std::string>& _serv_conf, Server& server)
{
    std::stringstream ss_port;
    std::stringstream ss_body;
    unsigned int port;
    unsigned int body;
    std::map<std::string, std::string>::iterator it = _serv_conf.find(LISTEN);
    std::map<std::string, std::string>::iterator end = _serv_conf.end();
    
    if (it == end)
        throw ExceptionThrower("Missing Port value");
    
    ss_port << it -> second;
    ss_port >> port;
    server.setPort(port);
    it = _serv_conf.find(ROOT);

    if (it != end) server.setRootDir(it -> second);
    
    it = _serv_conf.find(INDEX);
    
    if (it != end) server.setIndex(it -> second);

    it = _serv_conf.find(CLIENT_BODY);

    if (it != end)
    {
        ss_body << it -> second;
        ss_body >> body;
        server.setBodySize(body);
    }

    it = _serv_conf.find(REDIRECT);

    if (it != end) server.setRedirect(it -> second);
}

void    Parser::checkOpeningLine(std::ifstream& file)
{
    std::string line;
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
            break ;
        }
        if (file.eof()) throw ExceptionThrower(WRONG_FILE_FORMAT);
    }
}

void    Parser::SemicolonCheck(std::string& line, size_t i, size_t len)
{
    size_t  pos = line.find(';');

    if (i != len && pos != std::string::npos)
        throw ExceptionThrower("BAD_SYNTAX");
            
    if (i == len)
    {
        if (StringUtils::count(line, ';') > 1)
            throw ExceptionThrower("Too Many Semicolons");
                
        if (*(line.rbegin()) != ';')
            throw ExceptionThrower("Missing Semicolons At end");

        line.erase(line.size() - 1);
    }
}

Location Parser::fillUpLocation(std::ifstream& file, bool bracket)
{
    Location _location;
    std::vector<std::string> vec;
    std::string line;
    
    vec = StringUtils::stringSpliter(line, WHITESPACES);
    
    if (vec.size() != 2 && vec.size() != 3)
        throw ExceptionThrower("Too Many/Not Enough Arguments");

    if (bracket && (vec.size() == 2 && vec[1].size() == 1))
        throw ExceptionThrower("Missing Location Path");
    
    if (bracket && vec.size() == 2)
        vec[1].erase(vec[1].length() - 1);

    _location.setRootDir(vec[1]);

    if (!bracket)
    {
        /*CHECKS IF FIRST LINE MEET PARSING REQUIREMENTS*/
        checkOpeningLine(file);
        if (line.size() > 1)
            line = "";
    }

    if (file.eof()) throw ExceptionThrower(WRONG_FILE_FORMAT);
    
    while (std::getline(file, line))
    {
        vec = StringUtils::stringSpliter(line, WHITESPACES);
        if (file.eof()) break ;
    }

    return _location;
}

void    Parser::fillMap(const std::string& line, Server& server, std::map<std::string, std::string>& _serv_conf)
{
    std::vector<std::string> vec;
    size_t len;
    
    vec = StringUtils::stringSpliter(line, WHITESPACES);
    
    if (!TcpServer::isKnownDirective(vec[0]))
        throw ExceptionThrower("Directive " + vec[0] + " is unknown");

    if (vec.size() == 1)
        throw ExceptionThrower(MISSING_TOO_MANY_KEY_VALUE);

    /*
        CHECK IF DIRECTIVES EXIST
        IF NOT THROW AN ERROR
    */
    if (vec.size() > 3 && (vec[0] != SERVER_NAMES && vec[0] != ALLOWED_METHOD))
        throw ExceptionThrower("Directives " + vec[0] + " Has Too Many Arguments");
        
    if (vec.size() != 3 && vec[0] == CGI)
        throw ExceptionThrower("CGI PATTERN MISSING: cgi 'extensions' 'path to exentions'");

    len = vec.size() - 1;
    
    if (vec[0] == SERVER_NAMES)
    {
        for (size_t i = 1; i < vec.size(); i++)
        {
            SemicolonCheck(vec[i], i, len);
            server.pushNewServerName(vec[i]);
           // std::cout << vec[i] << std::endl;
        }
    }
    else if (vec[0] == CGI)
    {
        for (size_t i = 1; i < vec.size(); i++)
        {
            SemicolonCheck(vec[i], i, len);
            server.pushNewCGI(vec[i]);
        }
    }
    else if (vec[0] == ALLOWED_METHOD)
    {
        if (vec.size() > 4)
            throw ExceptionThrower("Directives " + vec[0] + " Has Too Many Arguments");
        int method;
        for (size_t i = 1; i < vec.size(); i++)
        {
            SemicolonCheck(vec[i], i, len);
            if (vec[i].size() == 0 && i == len) break ;
            method = TcpServer::getHttpMethod(vec[i]);
            if (method < 0)
                throw ExceptionThrower("Unknown HTTP Method");
            server.setOptions(method, SET);
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
            vec[1].erase(vec[1].length() - 1);
        }

        /*
            IF NO ERROR HAS BEEN THROWN
            ADD THE DIRECTIVE TO THE MAP
            CHECK IF DIRECTIVE HAVE ALREADY BEEN ADDED
            IF SO THROW AN ERROR
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
        /*CHECKS IF FIRST LINE MEET PARSING REQUIREMENTS*/
        checkOpeningLine(file);
        if (line.size() > 1)
            fillMap(line, server, _serv_conf);
    }

    if (file.eof()) throw ExceptionThrower(WRONG_FILE_FORMAT); 
    
    while (std::getline(file, line))
    {
        line = StringUtils::trimBothEnd(line, WHITESPACES);

        if (line.size())
        {
            if (line.find('}') != std::string::npos)
            {
                if (line.size() == 1)
                    break ;

                if (StringUtils::count(line, '}') > 1)
                    throw ExceptionThrower("Too Many Closing Bracket");
                
                if (*(line.rbegin()) != '}')
                    throw ExceptionThrower("Missing Closing Bracket");
                
                line.erase(--line.end());
                fillMap(line, server, _serv_conf);
                break ;
            }
            /*if (line.find(LOCATION, 0, 8) != std::string::npos)
            {
                if (StringUtils::count(line, '{') > 1)
                    throw ExceptionThrower("Too Many Opening Bracket");

                pos = line.find('{');

                if ((pos != std::string::npos) && *(line.rbegin()) != '{')
                    throw ExceptionThrower("Opening Bracket Must Be At The End Of The Line");

                server.pushNewLocation(fillUpLocation(file, pos != std::string::npos));
            }
            else*/
            fillMap(line, server, _serv_conf);
        }
        
        if (file.eof()) break ;
    }
    feedingUpServer(_serv_conf, server);
    return server;
}

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
std::vector<Server> Parser::getServerConfig(std::ifstream& file)
{
    std::string line;
    std::vector<Server> server;
    size_t  pos;

    server.reserve(10);
    while (std::getline(file, line))
    {
        line = StringUtils::trimBothEnd(line, WHITESPACES);

        if (line.size())
        {
            if (line.find('}') != std::string::npos)
                    throw ExceptionThrower("BAD_SYNTAX");
            
            if (line.find(SERVER_CONTEXT, 0, 6) != std::string::npos)
            {
                if (StringUtils::count(line, '{') > 1)
                    throw ExceptionThrower("Too Many Opening Bracket");

                pos = line.find('{');

                if ((pos != std::string::npos) && *line.rbegin() != '{')
                    throw ExceptionThrower("Opening Bracket Must Be At The End Of The Line");

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