# include "../../includes/method/Post.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/IO.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Post::Post(){};
Post::Post(const Post& rhs):Method(rhs){};
Post& Post::operator=(const Post& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Post::~Post(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/


int Post::handleMultipartData(HttpRequest& req)
{
    static int _nb;
    const std::map<std::string, std::string>& _map = req.getHeaders();
    std::string& s_buffer = req.getBuffer();

    while (1)
    {
        size_t pos = s_buffer.find(_map.find(BOUNDARY CRLF) -> second);
        if (pos != std::string::npos)
        {
            std::cout << s_buffer;
            //std::cout << pos << std::endl;
            std::vector<std::string> form_header = UtilityMethod::stringSpliter(s_buffer.substr(pos, s_buffer.find(CRLF CRLF)), CRLF);
            
            /*for (size_t i = 0; i < form_header.size(); i++)
            {
                std::cout << form_header[i] << std::endl;
            }*/
            
            s_buffer.erase(pos, s_buffer.find(CRLF CRLF) + 4);
        }

        /*pos = s_buffer.find(_map.find(END_BOUNDARY CRLF) -> second);
        if (pos == 0) break ;*/
    }

    _nb++;

    return IO::IO_SUCCESS;
}

int Post::sendResponse(IO& event, HttpRequest& req, HttpResponse& res)
{
    if (event.getEvents() & EPOLLIN)
    {
        (void)req;
        if (res.checkBits(HttpResponse::MULTIPART_DATA))
        {
            std::cout << req.getBuffer().find(req.getHeaders().find(END_BOUNDARY) -> second) << std::endl;
            handleMultipartData(req);
        }
        exit(1);
    }
    else
    {
        if (sendBuffer(event.getFd(), SERVER_SUCCESS_POST_RESPONSE, strlen(SERVER_SUCCESS_POST_RESPONSE)))
        {
            res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
            return IO::IO_ERROR;
        }
        res.setOptions(HttpResponse::FINISHED_RESPONSE, SET);
    }
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
