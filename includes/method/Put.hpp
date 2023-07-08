# ifndef __PUT_HPP__
# define __PUT_HPP__

# include "Method.hpp"

class ClientSocketStream;
class HttpRequest;

class Put: public Method
{
    public:
        Put();
        Put(const Put&);
        Put& operator=(const Put&);
        virtual ~Put();

        /*GETTERS*/
        const size_t& getRequestBodySize(void) const;

        /*SETTERS*/
        void updateSize(const size_t&);

        /*MEMBER FUNCTION*/
        void clearRequestBodySize(void);
        int sendResponse(ClientSocketStream&, HttpRequest&, HttpResponse&);
        int writeToFile(HttpRequest&);
        int writeToFile(HttpRequest&, const size_t&, const size_t&);
        int writeToFileMutltipartData(HttpRequest&, const size_t&);

        /*VIRTUAL MEMBER FUNCTION*/
        virtual Method* clone(void) const;

    private:
        int handleMultipartData(ClientSocketStream&, HttpRequest&);
        size_t _request_body_size;
};

# endif