# ifndef __PARSER_HPP__
# define __PARSER_HPP__
# include "CommonLib.hpp"
# include "UtilityMethod.hpp"
class Server;
class Location;
class TcpServer;

class Parser
{    
    public:
        Parser();
        Parser(const Parser& rhs);
        ~Parser();

        /*MEMBER FUNCTION*/
        std::vector<Server> getServerConfig(std::ifstream& file, TcpServer *tcp_server);
    private:

        /*MEMBER FUNC*/

        bool    validIpFormat(const std::string& ip);
        Server fillServer(std::ifstream& file, std::string& line, bool bracket);
        Location fillUpLocation(Server *server, std::ifstream& file, std::string& line, bool bracket);
        Parser& operator=(const Parser& rhs);
        void    setAllowedMethods(TcpServer& instance, std::vector<std::string>& vec, std::map<std::string, std::string>& _serv_conf);
        void    handleErrorPages(TcpServer& instance, std::vector<std::string>& vec);
        void    SemicolonCheck(std::string& line, size_t i, size_t len);
        void    fillMap(const std::string& line, Server& server, std::map<std::string, std::string>& _serv_conf);
        void    fillMap(const std::string& line, Location& server, std::map<std::string, std::string>& _serv_conf);
        void    checkOpeningLine(std::ifstream& file, std::string& line);
        void    feedingUpLocation(std::map<std::string, std::string>& _map, Location& location);
        void    feedingUpServer(std::map<std::string, std::string>& _serv_conf, Server& server);
        void    setCommonDirectives(std::vector<std::string>& vec, std::map<std::string, std::string>& _map);
};

#endif