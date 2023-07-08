# ifndef __FILE_WRITER_HPP__
# define __FILE_WRITER_HPP__

# include "CommonLib.hpp"

class ClientSocketStream;
class HttpRequest;
class HttpServer;

class FileWriter
{
    public:
        /*GETTERS*/
        const size_t& getRequestBodySize(void) const;
        std::ofstream& getOutfile(void);
        
        /*Virtual Member Function*/
        virtual int open_file(HttpServer& instance, std::map<std::string, std::string>&);
        virtual int open_file(HttpServer& instance, std::map<std::string, std::string>&, std::string& filepath);

    protected:
        FileWriter();
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

    private:
        std::ofstream _outfile;
};

# endif