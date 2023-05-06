# ifndef __IO_HPP__
# define __IO_HPP__

# include "../CommonLib.hpp"
# include "../TcpServer.hpp"
# include "../Server.hpp"
# include "../http/HttpRequest.hpp"
# include "../http/HttpResponse.hpp"
# include "../BitsManipulation.hpp"

class IO: public BitsManipulation
{
    public:
        IO();
        IO(const int& fd, Server *server);
        IO(const IO& rhs);
        IO& operator=(const IO& rhs);
        virtual ~IO();

        virtual int handleIoOperation(const int& _ws, struct epoll_event& event) = 0;

        
    
        /*GETTERS*/
        const int& getFd(void) const;
        const HttpRequest& getRequest(void) const;
        const HttpResponse& getReponse(void) const;
        HttpResponse& getReponse(void);
        const int& getErrStatus(void) const;
        Server* getServer(void) const;

        /*SETTERS*/
        void setFD(const int& fd);
        void setErrorStatus(const int& err);

        /*MEMBER FUNCTION*/
        bool validSocketClient(int _fd, struct epoll_event event);

        enum
        {
            IO_ERROR = -1,
            IO_SUCCESS
        };

        enum type
        {
            VIRTUAL_SERV,
            CLIENT_SOCKET,
            CGI_PIPE
        };
        
        protected:
            int _fd;
            int _err;
            Server *_server;
            HttpRequest _request;
            HttpResponse _response;
            enum type _type;
            std::clock_t _begin;
            std::clock_t _end;
};

# endif