# ifndef __IO_HPP__
# define __IO_HPP__

# include "../utils/CommonLib.hpp"
# include "../server/HttpServer.hpp"
# include "../server/Server.hpp"
# include "../http/HttpRequest.hpp"
# include "../http/HttpResponse.hpp"
# include "../utils/BitsManipulation.hpp"

class IO: public BitsManipulation
{
    public:
        IO();
        IO(const int& fd, Server *server);
        IO(const IO& rhs);
        IO& operator=(const IO& rhs);
        virtual ~IO();

    
        /*GETTERS*/
        const uint32_t& getEvents() const;
        const int& getFd(void) const;
        const HttpRequest& getRequest(void) const;
        const HttpResponse& getReponse(void) const;
        const int& getErrStatus(void) const;
        HttpRequest& getRequest(void);
        HttpResponse& getReponse(void);
        Server* getServer(void) const;

        /*GETTERS*/
        IO* getIO(void) const;

        /*SETTERS*/
        void setIO(IO* io);
        void setEvents(const uint32_t& events);
        void setFD(const int& fd);
        void setErrorStatus(const int& err);

        /*MEMBER FUNCTION*/
        bool validSocketClient(int _fd, struct epoll_event event);

        /*PURE VIRTUAL FUNCTION*/
        virtual int handleIoOperation(const int& _ws, struct epoll_event& event) = 0;
        void clear(void);

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
            uint32_t     _event;
            IO  *_io;
            HttpRequest _request;
            HttpResponse _response;
            enum type _type;
            std::clock_t _begin;
            std::clock_t _end;
};

# endif