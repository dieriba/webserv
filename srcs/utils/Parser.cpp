# include "../../includes/utils/Parser.hpp"
# include "../../includes/utils/ExceptionThrower.hpp"
# include "../../includes/server/HttpServer.hpp"
# include "../../includes/server/Server.hpp"
# include "../../includes/server/Location.hpp"
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Parser::Parser(){};
Parser::Parser(const Parser& rhs){(void)rhs;};
Parser::~Parser(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

void    Parser::checkEndSemicolons(std::vector<std::string>& vec)
{
    if (vec.size() == 3)
    {
        if (vec.rbegin() -> length() != 1 || vec.rbegin() -> at(0) != ';')
            throw ExceptionThrower(LAST_ARGS);
        if (vec[1].find(';') != std::string::npos)
            throw ExceptionThrower(BAD_SYNTAX);
        return ;
    }
    if (UtilityMethod::count(vec[1], ';') > 1 || vec[1].at(vec[1].size() - 1) != ';')
        throw ExceptionThrower(MISSING_SEMICOLONS);
    vec[1].erase(vec[1].length() - 1);
}

void    Parser::setCommonDirectives(std::vector<std::string>& vec, std::map<std::string, std::string>& _map)
{
    checkEndSemicolons(vec);
    if (_map.find(vec[0]) != _map.end())
        throw ExceptionThrower("Redeclaration of the same directive");

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

void    Parser::feedingUpInstance(std::map<std::string, std::string>& _map, HttpServer& instance)
{
    std::stringstream ss;

    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator end = _map.end();


    it = _map.find(INDEX);
    
    if (it != end) instance.setIndex(it -> second);
    
    it = _map.find(ROOT);

    if (it != end)
    {
        instance.setRootDir(UtilityMethod::rtrim("." + it -> second, "/"));
        instance.setFullIndexPath(instance.getRootDir() + instance.getIndexPath());
    }

    it = _map.find(ALLOWED_METHOD);

    if (it == end) instance.getOption() |= HttpServer::_all_methods;

    it = _map.find(CLIENT_BODY);

    if (it != end)
    {
        size_t val;
        ss << it -> second;
        ss >> val;

        if (val == std::string::npos) ExceptionThrower("Body Value insanely huge");

        instance.setBodySize(val);
    }
    else
        instance.setBodySize(std::string::npos);

    it = _map.find(REDIRECT);

    if (it != end) instance.setRedirect(it -> second);

    it = _map.find(AUTO_INDEX);

    if (it != end && it -> second ==  "on") instance.setOptions(HttpServer::HTTP_SERVER_AUTO_INDEX_, SET);

    it = _map.find(FILE_UPLOAD);

    instance.setOptions(HttpServer::HTTP_SERVER_FILE_UPLOAD_, SET);

    if (it != end && it -> second ==  "off") instance.setOptions(HttpServer::HTTP_SERVER_FILE_UPLOAD_, CLEAR);

    it = _map.find(UPLOAD_FILE_FOLDERS);

    if (it != end) instance.setUploadsFilesFolder(UtilityMethod::rtrim("." + it -> second, "/"));
}

void    Parser::feedingUpServer(std::map<std::string, std::string>& _serv_conf, Server& server)
{
    size_t val;
    std::vector<std::string> vec;
    std::stringstream ss;
    std::map<std::string, std::string>::iterator it = _serv_conf.find(LISTEN);
    std::map<std::string, std::string>::iterator end = _serv_conf.end();
    
    if (it == end)
        throw ExceptionThrower("Missing \"listen\" Directive");
    
    if ((it -> second.find(':') != std::string::npos) && UtilityMethod::count(it -> second, ':') != 2)
        throw ExceptionThrower(IP_FORMAT);
    
    vec = UtilityMethod::stringSpliter(it -> second, ":");
    
    if (vec.size() == 2 && ((vec[0].find_first_not_of(BASE_10".") != std::string::npos) || (vec[1].find_first_not_of(BASE_10";") != std::string::npos)))
        throw ExceptionThrower(LISTEN_FORMAT);

    if (vec.size() > 2 || vec.size() == 0) throw ExceptionThrower(LISTEN_FORMAT);
    
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

    feedingUpInstance(_serv_conf, static_cast<HttpServer&>(server));
}

int    Parser::setAllowedMethods(HttpServer& instance, std::vector<std::string>& vec, std::map<std::string, std::string>& _serv_conf)
{
    size_t  len = vec.size() - 1;
        
    for (size_t i = 1; i < vec.size(); i++)
    {
        SemicolonCheck(vec[i], i, len);
        if (vec[i].size() == 0 && i == len) break ;
        int method = HttpServer::getHttpMethod(vec[i]);
        if (method < 0)
            throw ExceptionThrower("Unknown HTTP Method");
        instance.setOptions(method, SET);
    }
    _serv_conf[vec[0]] = "HTTP METHODS";

    return 1;
}

void    Parser::checkOpeningLine(std::ifstream& file, std::string& line)
{
    while (std::getline(file, line))
    {
        line = UtilityMethod::trimBothEnd(line, WHITESPACES);
            
        if (line.size())
        {
            if (line.size() == 1 && line.at(0) == '{')
                break ;
                
            if (line.size() == 1)
                throw ExceptionThrower("Missing Opening Bracket");
                
            if (UtilityMethod::count(line, '{') > 1)
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
        if (UtilityMethod::count(line, ';') > 1)
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
    
    vec = UtilityMethod::stringSpliter(line, WHITESPACES);
    
    if (vec.size() != 2 && vec.size() != 3)
        throw ExceptionThrower("Too Many/Not Enough Arguments");

    if (bracket && (vec.size() == 2 && vec[1].size() == 1))
        throw ExceptionThrower("Missing Location Path");
    
    if (bracket && vec.size() == 2)
        vec[1].erase(vec[1].length() - 1);

    _location.setIndexPath(UtilityMethod::rtrim(vec[1], "/"));

    if (!bracket)
    {
        /*CHECKS IF FIRST LINE MEET PARSING REQUIREMENTS*/
        checkOpeningLine(file, line);
        if (line.size() > 1) fillMap(line, _location, _map);
    }

    if (file.eof()) throw ExceptionThrower(WRONG_FILE_FORMAT);
    
    while (std::getline(file, line))
    {
        line = UtilityMethod::trimBothEnd(line, WHITESPACES);

        if (line.size())
        {
            if (line.find('}') != std::string::npos)
            {
                if (line.size() == 1)
                    break ;

                if (UtilityMethod::count(line, '}') > 1)
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

    feedingUpInstance(_map, static_cast<HttpServer&>(_location));

    _location.setServer(server);
    return _location;
}

int Parser::setHeaderMap(HttpServer& instance, std::vector<std::string>& vec)
{
    if (vec.size() < 3) throw ExceptionThrower("Missing Key And/Or value");

    const std::map<std::string, std::string>& headers = instance.getHeadersMap();
    
    if (headers.find(vec[1]) != headers.end())
        throw ExceptionThrower("Redeclaration of same header");

    std::string& last = *(vec.rbegin());

    if (*(last.rbegin()) != ';')
        throw ExceptionThrower("Missing Semicolon At the End");
    
    last.erase(last.size() - 1);

    if (vec[2].size() == 0) throw ExceptionThrower("Missing Key Value");

    instance.pushNewHeaderDirective(vec[1], vec[2]);

    if (instance.checkBits(HttpServer::HTTP_SERVER_CUSTOM_HEADER) == 0)
        instance.setOptions(HttpServer::HTTP_SERVER_CUSTOM_HEADER, SET);
    
    return 1;
}

int Parser::setCgiMap(HttpServer& instance, std::vector<std::string>& vec)
{
    std::string path;
        
    if (instance.getCgiPath(vec[1], path))
        throw ExceptionThrower("CGI " + vec[1] + " already exists with path: " + vec[2]);

    if (vec[1].find(';') != std::string::npos || (UtilityMethod::count(vec[2], ';') > 1))
        throw ExceptionThrower("Bad Syntax");
        
    if (*(vec.rbegin() -> rbegin()) != ';')
        throw ExceptionThrower("Missing Semicolon At the End");
    vec[2].erase(vec[2].size() - 1);
    instance.pushNewCGI(vec[1], vec[2]);

    return 1;
}

int    Parser::fillInstance(HttpServer& instance, std::vector<std::string>& vec, std::map<std::string, std::string>& _map)
{
    if (vec[0] == CGI)
        return setCgiMap(instance, vec);
    else if (vec[0] == ALLOWED_METHOD)
        return setAllowedMethods(instance, vec, _map);
    else if (vec[0] == ADD_HEADER)
        return setHeaderMap(instance, vec);
    else if (vec[0] == ERROR_PAGE)
        return handleErrorPages(instance, vec);
    else if (vec[0] == AUTO_INDEX || vec[0] == FILE_UPLOAD)
    {
        checkEndSemicolons(vec);
        if (vec[1].compare("on") != 0 && vec[1].compare("off") != 0)
            throw ExceptionThrower("auto_index possible value are either 'on' or 'off'");
        _map[vec[0]] = vec[1];
        return 1;
    }

    return 0;
}

void    Parser::fillMap(const std::string& line, Location& location, std::map<std::string, std::string>& _map)
{
    std::vector<std::string> vec;
    
    vec = UtilityMethod::stringSpliter(line, WHITESPACES);
    
    if (!HttpServer::isKnownLocationDirectives(vec[0]))
        throw ExceptionThrower("Directive " + vec[0] + " is unknown");

    if (vec.size() == 1)
        throw ExceptionThrower(MISSING_TOO_MANY_KEY_VALUE);

    if (vec.size() > 3 && (vec[0] != ALLOWED_METHOD && vec[0] != ERROR_PAGE && vec[0] != ADD_HEADER))
        throw ExceptionThrower("Directives " + vec[0] + " Has Too Many Arguments");
    if (fillInstance(static_cast<HttpServer&>(location), vec, _map))
        return ;
    else
        setCommonDirectives(vec, _map);
}

int    Parser::handleErrorPages(HttpServer& instance, std::vector<std::string>& vec)
{
    size_t len = vec.size() - 1;

    for (size_t i = 0; i <= len; i++)
        SemicolonCheck(vec[i], i, len);
    
    if ((vec.size() % 2 != 0) || vec.size() < 4)
        throw ExceptionThrower("Missing root_error_page directory");

    std::string directory("." + vec[len--]);

    for (size_t i = 1; i < len; i+=2)
    {
        std::stringstream number(vec[i]);
            
        short int err;
            
        number >> err;

        if (number.fail())
            throw ExceptionThrower("Error value must is too high");

        if (vec[i].find_first_not_of(BASE_10) != std::string::npos)
            throw ExceptionThrower("Error status code must only contains number between 0-9");
            
        if (instance.addToErrorMap(err, vec[i + 1], directory) == -1)
            throw ExceptionThrower("Cannot rewrite the same status code");
    }

    instance.setOptions(HttpServer::HTTP_SERVER_ERROR_PAGE_SET, SET);
    
    return 1;
}

void    Parser::fillMap(const std::string& line, Server& server, std::map<std::string, std::string>& _serv_conf)
{
    std::vector<std::string> vec;
    size_t len;
    
    vec = UtilityMethod::stringSpliter(line, WHITESPACES);
    
    /*
        CHECK IF DIRECTIVES EXIST
        IF NOT THROW AN ERROR
    */

    if (!HttpServer::isKnownDirective(vec[0]))
        throw ExceptionThrower("Directive " + vec[0] + " is unknown");

    if (vec.size() == 1)
        throw ExceptionThrower(MISSING_TOO_MANY_KEY_VALUE);

    if (vec.size() > 3 && (vec[0] != SERVER_NAMES && vec[0] != ALLOWED_METHOD && vec[0] != ADD_HEADER && vec[0] != ERROR_PAGE))
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
        }
    }
    else if (fillInstance(static_cast<HttpServer&>(server), vec, _serv_conf) == 0)
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
        line = UtilityMethod::trimBothEnd(line, WHITESPACES);

        if (line.size())
        {
            if (line.find('}') != std::string::npos)
            {
                if (line.size() == 1)
                    break ;

                if (UtilityMethod::count(line, '}') > 1)
                    throw ExceptionThrower("Too Many Closing Bracket");
                
                if (*(line.rbegin()) != '}')
                    throw ExceptionThrower("Missing Closing Bracket");
                
                line.erase(--line.end());
                fillMap(line, server, _serv_conf);
                line += "}";
                break ;
            }

            size_t pos;
            
            if ((pos = line.find(LOCATION)) != std::string::npos)
            {
                if (pos != 0) throw ExceptionThrower("Unkown Context");
                    
                pos = line.find('{');
                
                if (UtilityMethod::count(line, '{') > 1)
                    throw ExceptionThrower("Too Many Opening Bracket");
                
                if ((pos != std::string::npos) && *(line.rbegin()) != '{')
                    throw ExceptionThrower("Opening Bracket Must Be At The End Of The Line");
                
                server.pushNewLocation(fillUpLocation(&server, file, line, pos != std::string::npos));

                if (line.find("}") == std::string::npos)
                    throw ExceptionThrower("Missing End Bracket");
            }
            else
                fillMap(line, server, _serv_conf);
        }
        
        if (file.eof()) break ;
    }

    feedingUpServer(_serv_conf, server);
    
    std::vector<Location>& serv_locations(server.getLocations());

    bool header = server.checkBits(HttpServer::HTTP_SERVER_CUSTOM_HEADER);
    std::map<std::string, std::string>& _map = server.getHeadersMap();
    for (size_t i = 0; i < serv_locations.size(); i++)
    {
        if (serv_locations[i].getRootDir().size() == 0)
            serv_locations[i].setRootDir(server.getRootDir());
        
        if (header == true && serv_locations[i].checkBits(HttpServer::HTTP_SERVER_CUSTOM_HEADER) == 0)
        {
            serv_locations[i].setOptions(HttpServer::HTTP_SERVER_CUSTOM_HEADER, SET);
            std::map<std::string, std::string>::const_iterator it = _map.begin();
            std::map<std::string, std::string>& _location_map = serv_locations[i].getHeadersMap();
            for (; it != _map.end(); it++)
            {
                if (_location_map.find(it -> first) == _location_map.end())
                    _location_map[it -> first] = it -> second;
            }
        }
    }
    
    return server;
}

/*----------------------------------------MEMBER FUNCTION-----line.find('{') != std::string::npos-----------------------------------*/
std::vector<Server> Parser::getServerConfig(std::ifstream& file, HttpServer *http_server)
{
    std::string line;
    Server serv;
    std::vector<Server> server;
    size_t  pos;

    server.reserve(BASE_VEC_ARR);

    while (std::getline(file, line))
    {
        line = UtilityMethod::trimBothEnd(line, WHITESPACES);

        if (line.size())
        {
            if (line.find('}') != std::string::npos)
                    throw ExceptionThrower("BAD_SYNTAX");
            
            if (line.find(SERVER_CONTEXT) == 0)
            {
                pos = line.find_first_not_of(SERVER_CONTEXT);
                
                if ((pos == std::string::npos && line.size() != UtilityMethod::myStrlen(SERVER_CONTEXT))
                    || (pos != std::string::npos && (UtilityMethod::myStrlen(SERVER_CONTEXT) != pos)))
                    throw ExceptionThrower("Unknow Context: " + line + " please refer to dieriba for the known context");

                if (UtilityMethod::count(line, '{') > 1)
                    throw ExceptionThrower("Too Many Opening Bracket");

                pos = line.find('{');

                if ((pos != std::string::npos) && *line.rbegin() != '{')
                    throw ExceptionThrower("Opening Bracket Must Be At The End Of The Line");
                
                serv = fillServer(file, line, (pos != std::string::npos));
                
                serv.setIndexPath("/");

                serv.setHttpServer(http_server);
                
                server.push_back(serv);

                if (line.find("}") == std::string::npos)
                    throw ExceptionThrower("Missing End Bracket Here");
            }
            else
                throw ExceptionThrower("Unknow Context: " + line + " please refer to dieriba for the known context");
        }

        if (file.eof()) break ;
        
    }

    return server;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/