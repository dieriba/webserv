# include "../../includes/utils/UtilityMethod.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/IO/CgiStream.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/method/Method.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
UtilityMethod::UtilityMethod(){};
UtilityMethod::UtilityMethod(const UtilityMethod& rhs){(void)rhs;};
UtilityMethod::~UtilityMethod(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

int UtilityMethod::basicCgiSetup(IO& event, HttpResponse& res)
{
    CgiStream* cgi = static_cast<CgiStream *>(event.getIO());
        
    cgi -> setPipes(res.getPipes());
    cgi -> setFD(res.getReadEnd());

    epoll_event ev;

    ev.events = EPOLLIN;
    cgi -> setEvents(EPOLLIN);
    ev.data.ptr = cgi;
    if (epoll_ctl(event.getWs(), EPOLL_CTL_ADD, res.getReadEnd(), &ev) == -1) return IO::IO_ERROR;

    return IO::IO_SUCCESS;
}

int UtilityMethod::sendBuffer(int client_socket, const char *buffer, int bytes)
{
    while (bytes > 0)
    {
        int bytesSent = send(client_socket, buffer, bytes, 0);
        if (bytesSent <= 0) return IO::IO_ERROR;

        bytes -= bytesSent;

        buffer += bytesSent;
    }
    return IO::IO_SUCCESS;
}

size_t UtilityMethod::hexToDecimal(const std::string& hex)
{
    std::stringstream ss;

    ss << std::hex << hex;
    
    size_t  number;

    ss >> number;

    if (ss.fail()) return std::string::npos;
    return number;
}

std::string UtilityMethod::getDateAndTime(void)
{
    std::time_t currentTime = std::time(NULL);
    std::tm* currentTm = std::gmtime(&currentTime);
    char formattedDate[50] = {0};

    std::strftime(formattedDate, sizeof(formattedDate), "Date: %a, %d %b %Y %H:%M:%S GMT", currentTm);

    std::string date(formattedDate);

    return date;
}

std::vector<std::string> UtilityMethod::stringSpliter(const std::string& line, const std::string& delim) 
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

std::string UtilityMethod::ltrim(const std::string &line, const char *toTrim)
{
    size_t  start = line.find_first_not_of(toTrim);
    return (start == std::string::npos) ? "" : line.substr(start);
}

std::string UtilityMethod::rtrim(const std::string &line, const char *toTrim)
{
    size_t  end = line.find_last_not_of(toTrim);
    return (end == std::string::npos) ? "" : line.substr(0, end + 1);
}

std::string UtilityMethod::trimBothEnd(std::string& line, const char *toTrim)
{
    return rtrim(ltrim(line, toTrim), toTrim);
}

size_t  UtilityMethod::count(const std::string& line, char to_find)
{
    size_t  i = 0, count = 0, len = line.size();
    for (; i < len; i++)
    {
        if (line[i] == to_find)
            count++;
    }
    return count;
}

void UtilityMethod::deleteEventFromEpollInstance(const int& _ws, const int& _fd)
{
    if (_fd > 0) epoll_ctl(_ws, EPOLL_CTL_DEL, _fd, NULL);
}

int UtilityMethod::switchEvents(const int& _ws, uint32_t mode, struct epoll_event& event, IO& ev)
{
    event.events = mode;
    event.data.ptr = event.data.ptr;
    
    if (epoll_ctl(_ws, EPOLL_CTL_MOD, ev.getFd(), &event) == -1)
    {
        close(ev.getFd());
        return IO::IO_ERROR;
    }

    ev.setEvents(mode);
    return IO::IO_SUCCESS;
}

std::string UtilityMethod::numberToString(const size_t& number)
{
    std::ostringstream ss;

    ss << number;
    
    return ss.str();
}

std::string UtilityMethod::decimalToHex(const int& number)
{
    std::stringstream ss;
    ss << std::hex << number; 
    return ss.str();
}

const std::string& UtilityMethod::getMimeType(std::string& path, const std::string& index_path, const std::string& index, const bool& to_check)
{
    if (to_check && (path == index_path)) path = index;
    
    int  i = path.size() - 1;

    for (; i >= 0; i--)
    {
        if (path[i] == '/' || path[i] == '.')
            break ;
    }
    
    return HttpServer::getMimeType(&path[i]);
}

std::string UtilityMethod::getFileExtension(const std::string& content_type, const bool& option)
{
    if (option == false) 
        return HttpServer::getMimeType(content_type);
    
    int i = content_type.size() - 1;

    for (; i >= 0; i--)
        if (content_type[i] == '.') break ;

    if (i == 0) return "";
    
    return &content_type[i];
}

size_t  UtilityMethod::myStrlen(const char *buffer)
{
    size_t  i = -1;
    while (buffer[++i]);
    return i;
}

std::string UtilityMethod::remove_dup(std::string& ressource)
{
    std::vector<std::string> vec = stringSpliter(ressource, "/");
    if (vec.size() == 0) return "/";
    
    std::string path;
    
    size_t i = 0;
    
    if (vec[0] == ".")
    {
        path = ".";
        i = 1;
    }

    for ( ; i < vec.size(); i++)
        path += "/" + vec[i];

    return path;
}

int UtilityMethod::is_a_directory(const char *path)
{
   struct stat statbuf;
   
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}
/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/