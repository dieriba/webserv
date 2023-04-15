#include "../includes/Parser.hpp"
#include "../includes/Server.hpp"
#include "../includes/ExceptionThrower.hpp"
#include "../includes/Location.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Parser::Parser(){};
Parser::Parser(const Parser& rhs):StringUtils(rhs){(void)rhs;};
Parser::~Parser(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

void    Parser::setCommonDirectives(std::vector<std::string>& vec, std::map<std::string, std::string>& _map)
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

         if (_map.find(vec[0]) != _map.end())
            throw ExceptionThrower("Redeclaration of the same directive");
        //std::cout << vec[0] << " " << vec[1] << std::endl;
        _map[vec[0]] = vec[1];
}

bool    Parser::validIpFormat(const std::string& ip)
{
    short int bits, count = 0;
    size_t index = 0;
    size_t  i = 0;
    std::stringstream ss;

    while (1)
    {
        index = ip.find(".", index);

        ss << ip.substr(i);
        ss >> bits;
        if ((i == index) || (ss.fail() || bits > 255))
            return false ;
        count++;
        if (index == std::string::npos) break ;
        ss.clear();
        ss.str("");
        index++;
        i = index;
    }
    
    if (count != 4) return false ;

    return true;
}

void    Parser::feedingUpLocation(std::map<std::string,std::string>& _map, Location& location)
{
    unsigned int val;
    std::vector<std::string> vec;
    std::stringstream ss;

    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator end = _map.end();

    it = _map.find(ROOT);

    if (it != end) location.setRootDir(it -> second);
    
    it = _map.find(INDEX);
    
    if (it != end) location.setIndex(it -> second);

    it = _map.find(CLIENT_BODY);

    if (it != end)
    {
        ss << it -> second;
        ss >> val;
        location.setBodySize(val);
    }

    it = _map.find(REDIRECT);

    if (it != end) location.setRedirect(it -> second);
}

void    Parser::feedingUpServer(std::map<std::string, std::string>& _serv_conf, Server& server)
{
    unsigned int val;
    std::vector<std::string> vec;
    std::stringstream ss;
    std::map<std::string, std::string>::iterator it = _serv_conf.find(LISTEN);
    std::map<std::string, std::string>::iterator end = _serv_conf.end();
    
    if (it == end)
        throw ExceptionThrower("Missing \"listen\" Directive");
    
    if ((it -> second.find(':') != std::string::npos) && StringUtils::count(it -> second, ':') != 2)
        throw ExceptionThrower(IP_FORMAT);
    
    vec = StringUtils::stringSpliter(it -> second, ":");
    
    if (vec.size() == 2 && ((vec[0].find_first_not_of(BASE_10".") != std::string::npos) || (vec[1].find_first_not_of(BASE_10";") != std::string::npos)))
        throw ExceptionThrower(LISTEN_FORMAT);

    if (vec.size() > 2) throw ExceptionThrower(LISTEN_FORMAT);
    
    if (vec.size() == 1 && (vec[0].find_first_not_of(BASE_10) != std::string::npos))
        throw ExceptionThrower(LISTEN_FORMAT);

    if (vec.size() == 2)
    {
        if (!validIpFormat(vec[0]))
            throw ExceptionThrower(IP_FORMAT);
        server.setIp(vec[0]);
    }

    ss << (vec.size() == 2 ? vec[1] : vec[0]);
    ss >> val;

    if (ss.fail()) throw ExceptionThrower("PORT VALUE IS TOO HIGH");

    server.setPort(val);
    it = _serv_conf.find(ROOT);

    if (it != end) server.setRootDir(it -> second);
    
    it = _serv_conf.find(INDEX);
    
    if (it != end) server.setIndex(it -> second);

    it = _serv_conf.find(CLIENT_BODY);

    if (it != end)
    {
        ss.clear();
        ss.str("");
        ss << it -> second;
        ss >> val;
        server.setBodySize(val);
    }

    it = _serv_conf.find(REDIRECT);

    if (it != end) server.setRedirect(it -> second);
}

void    Parser::checkOpeningLine(std::ifstream& file, std::string& line)
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

Location Parser::fillUpLocation(Server *server, std::ifstream& file, std::string& line, bool bracket)
{
    std::vector<std::string> vec;
    std::map<std::string, std::string> _map;
    Location _location;
    
    vec = StringUtils::stringSpliter(line, WHITESPACES);
    
    if (vec.size() != 2 && vec.size() != 3)
        throw ExceptionThrower("Too Many/Not Enough Arguments");

    if (bracket && (vec.size() == 2 && vec[1].size() == 1))
        throw ExceptionThrower("Missing Location Path");
    
    if (bracket && vec.size() == 2)
        vec[1].erase(vec[1].length() - 1);

    _location.setSubPath(vec[1]);
    if (!bracket)
    {
        /*CHECKS IF FIRST LINE MEET PARSING REQUIREMENTS*/
        checkOpeningLine(file, line);
        if (line.size() > 1) fillMap(line, _location, _map);
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
                fillMap(line, _location, _map);
                line += "}";
                break ;
            }
            else
                fillMap(line, _location, _map);
        }

        if (file.eof()) break ;
    }
    feedingUpLocation(_map, _location);
    _location.setServer(server);
    return _location;
}

void    Parser::fillMap(const std::string& line, Location& location, std::map<std::string, std::string>& _serv_conf)
{
    std::vector<std::string> vec;
    size_t len;
    
    vec = StringUtils::stringSpliter(line, WHITESPACES);
    
    /*
        CHECK IF DIRECTIVES EXIST
        IF NOT THROW AN ERROR
    */
    if (!TcpServer::isKnownLocationDirectives(vec[0]))
        throw ExceptionThrower("Directive " + vec[0] + " is unknown");

    if (vec.size() == 1)
        throw ExceptionThrower(MISSING_TOO_MANY_KEY_VALUE);

    if (vec.size() > 3 && vec[0] != ALLOWED_METHOD)
        throw ExceptionThrower("Directives " + vec[0] + " Has Too Many Arguments");

    len = vec.size() - 1;

    if (vec[0] == ALLOWED_METHOD)
    {
        int method;

        if (vec.size() > 4)
            throw ExceptionThrower("Directives " + vec[0] + " Has Too Many Arguments");
        
        for (size_t i = 1; i < vec.size(); i++)
        {
            SemicolonCheck(vec[i], i, len);
            if (vec[i].size() == 0 && i == len) break ;
            method = TcpServer::getHttpMethod(vec[i]);
            if (method < 0)
                throw ExceptionThrower("Unknown HTTP Method");
            location.setOptions(method, SET);
        }
    }
    else
        setCommonDirectives(vec, _serv_conf);
}

void    Parser::fillMap(const std::string& line, Server& server, std::map<std::string, std::string>& _serv_conf)
{
    std::vector<std::string> vec;
    size_t len;
    
    vec = StringUtils::stringSpliter(line, WHITESPACES);
    
    /*
        CHECK IF DIRECTIVES EXIST
        IF NOT THROW AN ERROR
    */

    if (!TcpServer::isKnownDirective(vec[0]))
        throw ExceptionThrower("Directive " + vec[0] + " is unknown");

    if (vec.size() == 1)
        throw ExceptionThrower(MISSING_TOO_MANY_KEY_VALUE);

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
        std::string path;
        
        if (server.getCgiPath(vec[1], path))
            throw ExceptionThrower("CGI " + vec[1] + " already exists with path: " + vec[2]);

        if (vec[1].find(';') != std::string::npos || (StringUtils::count(vec[2], ';') > 1))
            throw ExceptionThrower("Bad Syntax");
        
        if (*vec.rbegin() -> rbegin() != ';')
            throw ExceptionThrower("Missing Semicolon At the End");
        vec[2].erase(vec[2].size() - 1);
        server.pushNewCGI(vec[1], vec[2]);
    }
    else if (vec[0] == ALLOWED_METHOD)
    {
        int method;

        if (vec.size() > 4)
            throw ExceptionThrower("Directives " + vec[0] + " Has Too Many Arguments");
        
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
        setCommonDirectives(vec, _serv_conf);
}

Server Parser::fillServer(std::ifstream& file, std::string& line, bool bracket)
{
    std::map<std::string, std::string> _serv_conf;
    Server  server;
    
    if (!bracket)
    {
        /*CHECKS IF FIRST LINE MEET PARSING REQUIREMENTS*/
        checkOpeningLine(file, line);
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
                line += "}";
                break ;
            }
            if (line.find(LOCATION, 0, 8) != std::string::npos)
            {
                size_t pos = line.find('{');
                
                if (StringUtils::count(line, '{') > 1)
                    throw ExceptionThrower("Too Many Opening Bracket");
                
                if ((pos != std::string::npos) && *(line.rbegin()) != '{')
                    throw ExceptionThrower("Opening Bracket Must Be At The End Of The Line");
                
                server.pushNewLocation(fillUpLocation(&server, file, line, pos != std::string::npos));
                if (line.find("}") == std::string::npos)
                    throw ExceptionThrower("Missing End Bracket");
                else
                {
                    std::getline(file, line);
                    break ;
                }
            }
            else
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

                server.push_back(fillServer(file, line, (line.find('{') != std::string::npos)));
                if (line.find("}") == std::string::npos)
                    throw ExceptionThrower("Missing End Bracket");
                else
                    break ;
            }
            else
                throw ExceptionThrower("Wrong Format of config file: " + line);
        }
        if (file.eof()) break ;
    }
    return server;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/