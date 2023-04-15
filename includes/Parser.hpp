#ifndef __PARSER_HPP__
#define __PARSER_HPP__
#include "CommonLib.hpp"
#include "StringUtils.hpp"
class Server;
class Location;
class Parser: public StringUtils
{    
    public:
        Parser();
        Parser(const Parser& rhs);
        ~Parser();

        /*MEMBER FUNCTION*/
        std::vector<Server> getServerConfig(std::ifstream& file);
    private:

        /*MEMBER FUNC*/
        Server fillServer(std::ifstream& file, bool bracket);
        Location fillUpLocation(Server *server, std::ifstream& file, std::string& line, bool bracket);
        void    SemicolonCheck(std::string& line, size_t i, size_t len);
        void    fillMap(const std::string& line, Server& server, std::map<std::string, std::string>& _serv_conf);
        void    fillMap(const std::string& line, Location& server, std::map<std::string, std::string>& _serv_conf);
        void    checkOpeningLine(std::ifstream& file, std::string& line);
        void    feedingUpLocation(std::map<std::string, std::string>& _map, Location& location);
        void    feedingUpServer(std::map<std::string, std::string>& _serv_conf, Server& server);
        bool    validIpFormat(const std::string& ip);
        void    setCommonDirectives(std::vector<std::string>& vec, std::map<std::string, std::string>& _map);
        Parser& operator=(const Parser& rhs);
};
#endif