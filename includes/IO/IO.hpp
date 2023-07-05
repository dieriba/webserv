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
        IO(const int&, Server *);
        IO(const int&, const int&, Server *);
        IO(const IO&);
        IO& operator=(const IO&);
        virtual ~IO();

    
        /*GETTERS*/
        int getType(void) const;
        const uint32_t& getEvents() const;
        const int& getFd(void) const;
        const HttpRequest& getRequest(void) const;
        const HttpResponse& getReponse(void) const;
        const int& getErrStatus(void) const;
        HttpRequest& getRequest(void);
        HttpResponse& getReponse(void);
        Server* getServer(void) const;
        const int& getWs(void) const;
        long getTimeStamp() const;

        /*GETTERS*/
        IO* getIO(void) const;

        /*SETTERS*/
        void setWs(const int&);
        void setIO(IO*);
        void setEvents(const uint32_t&);
        void setFD(const int&);
        void setErrorStatus(const int&);
        void updateTimeStamp();

        /*MEMBER FUNCTION*/
        long getTimestampInMillisecond(const long& _cgi_timesamp) const;
        /*PURE VIRTUAL FUNCTION*/
        virtual int handleIoOperation(const int&, struct epoll_event&) = 0;
        /*MEMBER FUNCTION*/
        int deleteAndResetIO(HttpResponse&);
        void clear(void);

        enum
        {
            IO_ERROR = -1,
            IO_SUCCESS,
            CGI_ON,
            COOKIE,
            KILL_MYSELF
        };

        enum type
        {
            VIRTUAL_SERV,
            CLIENT_SOCKET,
            CGI_STREAM,
        };
        
        protected:
            int _ws;
            int _fd;
            int _err;
            Server *_server;
            uint32_t     _event;
            IO  *_io;
            HttpRequest _request;
            HttpResponse _response;
            enum type _type;
            std::clock_t _timestamp;
            std::clock_t _end;
};

# endif