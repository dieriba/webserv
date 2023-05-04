# include "../includes/utilityMethod.hpp"
# include "../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
utilityMethod::utilityMethod(){};
utilityMethod::utilityMethod(const utilityMethod& rhs){(void)rhs;};
utilityMethod::~utilityMethod(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

std::vector<std::string> utilityMethod::stringSpliter(const std::string& line, const std::string& delim) 
{
    size_t pos_start = 0, pos_end;
    std::string token;
    std::vector<std::string> res;

    if ((pos_start = (line.find_first_not_of(delim))) != std::string::npos)
    { 
        while ((pos_end = line.find_first_of(delim, pos_start)) != std::string::npos)
        {
            token = line.substr(pos_start, pos_end - pos_start);
            pos_end = line.find_first_not_of(delim, pos_end);
            pos_start = pos_end;
            res.push_back(token);
        }
    }
    
    if (pos_start < line.size())
        res.push_back(line.substr(pos_start));
    return res;
}

std::string utilityMethod::ltrim(const std::string &line, const char *toTrim)
{
    size_t  start = line.find_first_not_of(toTrim);
    return (start == std::string::npos) ? "" : line.substr(start);
}

std::string utilityMethod::rtrim(const std::string &line, const char *toTrim)
{
    size_t  end = line.find_last_not_of(toTrim);
    return (end == std::string::npos) ? "" : line.substr(0, end + 1);
}

std::string utilityMethod::trimBothEnd(std::string& line, const char *toTrim)
{
    return rtrim(ltrim(line, toTrim), toTrim);
}

size_t  utilityMethod::count(const std::string& line, char to_find)
{
    size_t  i = 0, count = 0, len = line.size();
    for (; i < len; i++)
    {
        if (line[i] == to_find)
            count++;
    }
    return count;
}


void utilityMethod::switchEvents(const int& _ws, uint32_t mode, struct epoll_event& event, const IO& ev)
{
    event.events = mode;
    event.data.ptr = event.data.ptr;
    if (epoll_ctl(_ws, EPOLL_CTL_MOD, ev.getFd(), &event) == -1)
        close(ev.getFd());
}

std::string utilityMethod::numberToString(const int& number)
{
    std::ostringstream ss;

    ss << number;
    
    return ss.str();
}

const std::string& utilityMethod::getMimeType(std::string& path, const std::string& index_path, const std::string& index)
{
    if (path == index_path) path = index;
    
    int  i = path.size() - 1;


    for (; i >= 0; i--)
    {
        if (path[i] == '/' || path[i] == '.')
            break ;
    }
    
    return path[i] == '/' ? TcpServer::getMimeType(DEFAULT) : TcpServer::getMimeType(&path[i]);
}

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/