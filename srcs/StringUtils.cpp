#include "../includes/StringUtils.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
StringUtils::StringUtils(){};
StringUtils::StringUtils(const StringUtils& rhs){(void)rhs;};
StringUtils::~StringUtils(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

std::vector<std::string> StringUtils::stringSpliter(const std::string& line, const std::string& delim) 
{
    size_t pos_start = 0, pos_end;
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = line.find_first_of(delim, pos_start)) != std::string::npos)
    {
        token = line.substr(pos_start, pos_end - pos_start);
        pos_end = line.find_first_not_of(WHITESPACES, pos_end);
        pos_start = pos_end;
        res.push_back(token);
    }
    res.push_back(line.substr(pos_start));
    return res;
}

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

size_t  StringUtils::count(const std::string& line, char to_find)
{
    size_t  i = 0, count = 0, len = line.size();
    for (; i < len; i++)
    {
        if (line[i] == to_find)
            count++;
    }
    return count;
}
/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/