# ifndef __PARSER_HPP__
# define __PARSER_HPP__
# include "CommonLib.hpp"
# include "UtilityMethod.hpp"
class Server;
class Location;
class HttpServer;

class Parser
{    
    public:
        Parser();
        Parser(const Parser& rhs);
        ~Parser();

        /*MEMBER FUNCTION*/
        std::vector<Server> getServerConfig(std::ifstream&, HttpServer *);
    private:

        /*MEMBER FUNC*/

        Parser& operator=(const Parser&);
        Location fillUpLocation(Server *, std::ifstream&, std::string&, bool);
        Server fillServer(std::ifstream&, std::string&, bool);
        bool    validIpFormat(const std::string&);
        int    handleErrorPages(HttpServer&, std::vector<std::string>&);
        int    setAllowedMethods(HttpServer&, std::vector<std::string>&, std::map<std::string, std::string>&);
        void    SemicolonCheck(std::string&, size_t, size_t);
        void    fillMap(const std::string&, Server&, std::map<std::string, std::string>&);
        void    fillMap(const std::string&, Location&, std::map<std::string, std::string>&);
        int     fillInstance(HttpServer&, std::vector<std::string>&, std::map<std::string, std::string>&);
        void    checkOpeningLine(std::ifstream&, std::string&);
        void    checkEndSemicolons(std::vector<std::string>&);
        void    feedingUpLocation(std::map<std::string, std::string>&, Location&);
        void    feedingUpServer(std::map<std::string, std::string>&, Server&);
        void    feedingUpInstance(std::map<std::string, std::string>&, HttpServer&);
        void    setCommonDirectives(std::vector<std::string>&, std::map<std::string, std::string>&);
};

#endif