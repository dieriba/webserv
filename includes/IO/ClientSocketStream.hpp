# ifndef __CLIENTSOCKETSTREAM_HPP__
# define __CLIENTSOCKETSTREAM_HPP__

# include "IO.hpp"
# include "../server/Server.hpp"

class ClientSocketStream: public IO
{
    public:
        ClientSocketStream();
        ClientSocketStream(const int&, const int&, Server*, Server*);
        ClientSocketStream(const ClientSocketStream&);
        ClientSocketStream& operator=(const ClientSocketStream&);
        ~ClientSocketStream();

        /*GETTERS*/
        const unsigned int& getPort(void) const;
        const unsigned int& getPrevContentLength(void) const;
        const unsigned int& getPrevBodySize(void) const;
        std::string& getFilename(void);
        std::string getAllowedMethod(const HttpServer& instance, const std::map<std::string, short int>& _httpMethods);
        
        /*SETTERS*/
        void setPrevContentLength(const unsigned int&);
        void updatePrevBodySize(const unsigned int&);
        void setFilename(const std::string&);

        /*PURE VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual int handleIoOperation(const int&, struct epoll_event&);
 
        /*VIRTUAL MEMBER FUNCTION OVERRIDEN*/
        virtual void clear(void);
    private:
        int readFromSocket(const int&, struct epoll_event&);
        int writeToSocket(const int&, struct epoll_event&);
        unsigned int _port;
        unsigned int _prev_body_size;
        unsigned int _prev_content_len;
        std::string _filename;
        std::string cookie;
};

# endif