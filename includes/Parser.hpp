#ifndef __PARSER_HPP__
#define __PARSER_HPP__
#include "CommonLib.hpp"
#include "StringUtils.hpp"
class Server;
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
        Server fillServer(std::ifstream& file);
        Parser& operator=(const Parser& rhs);
};
#endif