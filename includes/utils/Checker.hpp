# ifndef __CHECKER_HPP88
# define __CHECKER_HPP88
# include "CommonLib.hpp"
# include "UtilityMethod.hpp"
class Server;
class Location;
class HttpServer;

class Checker
{    
    public:
        Checker();
        Checker(const Checker& rhs);
        ~Checker();

        /*MEMBER FUNCTION*/
        static int checkConfigFile(const char* filename);
    private:

        /*MEMBER FUNC*/
        Checker& operator=(const Checker&);
        static Location checkLocationContext(Server *, std::ifstream&, std::string&, bool);
        static void checkServerContext(std::ifstream&, std::string&, bool);
        static bool    validIpFormat(const std::string&);
        static void    notifyError(const std::string& error);
        static int    handleErrorPages(HttpServer&, std::vector<std::string>&);
        static int     setHeaderMap(HttpServer&, std::vector<std::string>&);
        static int    setAllowedMethods(std::vector<std::string>&, std::map<std::string, std::string>&);
        static void    SemicolonCheck(std::string&, size_t, size_t);
        static int     setCgiMap(HttpServer& instance, std::vector<std::string>& vec);
        static void    fillMap(const std::string&, Server&, std::map<std::string, std::string>&);
        static void    fillMap(const std::string&, Location&, std::map<std::string, std::string>&);
        static int     fillInstance(HttpServer&, std::vector<std::string>&, std::map<std::string, std::string>&);
        static void    checkOpeningLine(std::ifstream&, std::string&);
        static void    checkEndSemicolons(std::vector<std::string>&);
        static void    feedingUpServer(std::map<std::string, std::string>&, Server&);
        static void    feedingUpInstance(std::map<std::string, std::string>&, HttpServer&);
        static void    setCommonDirectives(std::vector<std::string>&, std::map<std::string, std::string>&);
        static void    throwException(const std::string&);

        /*STATIC VARIABLES*/
        static std::map<std::string, bool> _loc;
        static bool _error_spotted;
        static size_t _line_number;
};

#endif