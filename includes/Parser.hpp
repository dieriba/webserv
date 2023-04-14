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
        Location fillUpLocation(std::ifstream& file, bool bracket);
        void   fillMap(const std::string& line, Server& server, std::map<std::string, std::string>& _serv_conf);
        Parser& operator=(const Parser& rhs);
};
#endif