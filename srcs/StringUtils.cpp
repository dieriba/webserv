#include "../includes/StringUtils.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
StringUtils::StringUtils(){};
StringUtils::~StringUtils(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

std::string StringUtils::ltrim(const std::string &line, const char *toTrim)
{
    size_t  start = line.find_first_not_of(toTrim);
    return (start == std::string::npos) ? "" : line.substr(start);
}

std::string StringUtils::rtrim(const std::string &line, const char *toTrim)
{
    size_t  end = line.find_last_not_of(toTrim);
    return (end == std::string::npos) ? "" : line.substr(0, end + 1);
}

std::string StringUtils::trimBothEnd(std::string& line, const char *toTrim)
{
    return rtrim(ltrim(line, toTrim), toTrim);
}
/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/