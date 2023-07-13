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
        const HttpResponse& getResponse(void) const;
        const int& getErrStatus(void) const;
        HttpRequest& getRequest(void);
        HttpResponse& getResponse(void);
        Server* getServer(void) const;
        const int& getWs(void) const;
        long getTimeStamp(void) const;
        IO* getIO(void) const;
        Server *getBaseServer(void);

        /*SETTERS*/
        void updateTimeStamp(void);
        void updateTimeStamp(const long&);
        void setServer(Server *);
        void setWs(const int&);
        void setIO(IO*);
        void setEvents(const uint32_t&);
        void setFD(const int&);
        void setErrorStatus(const int&);

        /*MEMBER FUNCTION*/
        long getTimestampInMillisecond(const long& _cgi_timesamp) const;
        long getCurrentTimestampMs(void) const;
        
        /*MEMBER FUNCTION*/
        int deleteAndResetIO(HttpResponse&);
        
        /*PURE VIRTUAL MEMBER FUNCTION*/
        virtual int handleIoOperation(const int&, struct epoll_event&) = 0;

        /*VIRTUAL MEMBER FUNCTION*/
        virtual void clear(void);
        
        enum
        {
            IO_ERROR = -1,
            IO_SUCCESS,
            IO_CGI_ON,
            IO_COOKIE,
            IO_KILL_CGI,
            IO_KILL_MYSELF,
            IO_SOCKET_NOT_FINISH,
            IO_CONTENT_LEN,
        };

        enum type
        {
            IO_VIRTUAL_SERV,
            IO_CLIENT_SOCKET,
            IO_CGI_STREAM,
        };
        
        protected:
            int _ws;
            int _fd;
            int _err;
            long _timestamp;
            Server *_server;
            uint32_t     _event;
            IO  *_io;
            HttpRequest _request;
            HttpResponse _response;
            enum type _type;
            Server *_base_server;
};

# endif