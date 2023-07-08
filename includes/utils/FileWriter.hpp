# ifndef __FILE_WRITER_HPP__
# define __FILE_WRITER_HPP__

# include "CommonLib.hpp"

class ClientSocketStream;
class HttpRequest;
class HttpServer;

class FileWriter
{
    public:

        /*SETTERS*/
        void setFileExist(const bool&);

        /*GETTERS*/
        const size_t& getRequestBodySize(void) const;
        std::ofstream& getOutfile(void);

        /*Virtual Member Function*/
        virtual int open_file(ClientSocketStream&) = 0;
        virtual int open_file(ClientSocketStream&, std::string& filepath) = 0;

    protected:
        FileWriter();
        FileWriter(const short int&);
        FileWriter(const FileWriter&);
        FileWriter& operator=(const FileWriter&);
        virtual ~FileWriter();

        /*SETTERS*/
        void updateSize(const size_t&);

        /*MEMBER FUNCTION*/
        int fillChunkBody(HttpRequest&);
        int handleMultipartData(ClientSocketStream&, HttpRequest&);
        int writeToFile(HttpRequest&);
        int writeToFile(HttpRequest&, const size_t&, const size_t&);
        int writeToFileMutltipartData(HttpRequest&, const size_t&);
        void clearRequestBodySize(void);
        size_t _request_body_size;
        std::ofstream _outfile;
        static unsigned int _nb;

    private:
        short int _method;
        bool _file_exists;
};

# endif