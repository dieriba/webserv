#ifndef __COMMONLIB_HPP__
#define __COMMONLIB_HPP__

# include <string>
# include <iostream>
# include <list>
# include <vector>
# include <map>
# include <fstream>
# include <fcntl.h>
# include <netinet/in.h>
# include <cstring>
# include <unistd.h>
# include <algorithm>
# include <ctime>
# include <sys/epoll.h>
# include <sstream>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>


/*

*/

/*

*/

# define DEFAULT_FILE_NAME "/ressource_"
/*


*/
# define OK 200
# define CREATED 201
# define BAD_REQUEST 400
# define FORBIDEN 403
# define NOT_FOUND 404
# define METHOD_NOT_ALLOWED 405
# define METHOD_NOT_SUPPORTED 501
# define TOO_LARGE_CONTENT 413
# define INTERNAL_SERVER_ERROR 500
/*



*/
# define CONTENT_TYP "Content-Type"
# define CONTENT_LEN "Content-Length"
# define TRANSFERT_ENCODING "Transfer-Encoding"
# define CHUNK_REQUEST "-4"
# define VERSION "VERSION"
# define METHOD "METHOD"
# define HTTP_VERSION "HTTP/1.1"
# define PATH "PATH"
# define MIME_FILENAME "mimes_types.txt"
# define MULTIPART_FORM_DATA "multipart/form-data"
# define BOUNDARY "boundary"
# define END_BOUNDARY "end_boundary"
# define FORM_URLENCODED "application/x-www-form-urlencoded"

/*

*/

# define SERVER_SUCCESS_POST_RESPONSE "HTTP/1.1 201 CREATED\r\nContent-Type: application/json\r\nContent-Length: 19\r\n\r\n{\n\t\"sucess\": true\n}"
# define SERVER_ERROR_PAGE_NOT_FOUND "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 493\r\n\r\n<!DOCTYPE html>\n<html lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t\t<title>Webserv Error Page</title>\n\t</head>\n\t<body style=\"display: flex; height: 100vh; align-items: center; font-family: 'Wix Madefor Text', sans-serif;  font-size: 2rem; flex-direction: column;\">\n\t\t<h1>404 Not Found</h1>\n\t\t<p>Unfortunately, the asked ressource do not exist</p>\n\t\t</body>\n</html>"
# define SERVER_ERROR_PAGE_INTERNAL_SERVER_ERROR "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 529\r\n\r\n<!DOCTYPE html>\n<html lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t\t<title>Webserv Error Page</title>\n\t</head>\n\t<body style=\"display: flex; height: 100vh; align-items: center; font-family: 'Wix Madefor Text', sans-serif;  font-size: 2rem; flex-direction: column;\">\n\t\t<h1>500 Internal Server Error</h1>\n\t\t<p>Unfortunately, an error occured from the server, please try again later</p>\n\t</body>\n</html>"
# define SERVER_ERROR_PAGE_BAD_REQUEST "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 437\r\n\r\n<!DOCTYPE html>\n<html lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t\t<title>Webserv Error Page</title>\n\t</head>\n\t<body style=\"display: flex; height: 100vh; align-items: center; font-family: 'Wix Madefor Text', sans-serif;  font-size: 2rem; flex-direction: column;\">\n\t\t<h1>400 Bad Request</h1>\n\t</body>\n</html>"
# define SERVER_ERROR_PAGE_FORBIDDEN "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: 435\r\n\r\n<!DOCTYPE html>\n<html lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t\t<title>Webserv Error Page</title>\n\t</head>\n\t<body style=\"display: flex; height: 100vh; align-items: center; font-family: 'Wix Madefor Text', sans-serif;  font-size: 2rem; flex-direction: column;\">\n\t\t<h1>403 Forbidden</h1>\n\t</body>\n</html>"


/*

*/
# define FOUND_REDIRECT "HTTP/1.1 302 Found\r\nLocation: "
# define FOUND_REDIRECT_POST "HTTP/1.1 303 See Other\r\nLocation: "
# define FOUND_REDIRECT_IND_METHOD "HTTP/1.1 307 Temporary Redirect\r\nLocation: "

/*

*/
# define NO_BODY "-2"
# define BASE_VEC_ARR 10
# define WHITESPACES " \n\t\r\f\v"
# define NEW_LINE "\n"
# define BASE_10 "0123456789"
# define BASE_16 "0123456789ABCDEFabcdef"
# define REQUEST_SIZE 60000
# define MAXEVENTS 64
# define CRLF "\r\n"
# define LEN_CRLF 2
# define END_CHUNK "0\r\n\r\n"
# define LEN_END_CHUNK 5
# define TIMEOUT 1000
# define MAX_HEADER_SIZE 4000
# define NO_VALUE "NO_VALUE"
# define DOUBLE_HIPHEN "--"
/*


*/

# define SLASH "/"
# define DEFAULT "default"
# define HTM ".html"
# define HTML ".html"
# define CSS ".css"
# define CSV ".csv"
# define JPEG ".jpeg"
# define JPG ".jpg"
# define JS ".js"
# define JSON ".json"
# define PNG ".png"
# define PDF ".pdf"
# define PHP ".php"
# define SH ".sh"
# define MPEG ".mpeg"
# define MP3 ".mp3"
# define MP4 ".mp4"

# define MIME_PLAIN "text/plain"
# define MIME_HTM "text/html"
# define MIME_HTML "text/html"
# define MIME_CSS "text/css"
# define MIME_CSV "text/csv"
# define MIME_JPEG "image/jpeg"
# define MIME_JPG MIME_JPEG
# define MIME_JS "text/javascript"
# define MIME_JSON "application/json"
# define MIME_PNG "image/png"
# define MIME_PDF "application/pdf"
# define MIME_PHP "application/x-httpd-php"
# define MIME_SH "application/x-sh"
# define MIME_MPEG "video/mpeg"
# define MIME_MP3 "audio/mpeg"
# define MIME_MP4 "video/mp4"
/*


*/
# define LISTEN "listen"
# define SERVER_NAMES "server_names"
# define ROOT "root"
# define ALLOWED_METHOD "allowed_methods"
# define INDEX "index"
# define CLIENT_BODY "client_max_body_size"
# define LOCATION "location"
# define ERROR_PAGE "error_pages"
# define REDIRECT "redirect"
# define CGI "cgi"
# define ROOT_ERROR_PAGE "root_error_pages"
/*

*/
# define DEFAULT_CONF_FILE "./configs/default.conf"
# define MISSING_OP_BRACKET "Missing Opening Bracket"
# define MISSING_END_BRACKET "Missing End Bracket"
# define WRONG_FILE_FORMAT "Wrong File Format"
# define MISSING_TOO_MANY_KEY_VALUE "Missing Or Too Many Arguments"
# define LAST_ARGS "There should be only 1 ';'"
# define MISSING_SEMICOLONS "Missing Semicolon Or Too Many"
# define BAD_SYNTAX "Some Characters Used Are Not Allowed"
# define LISTEN_FORMAT "Required Format -> listen X.X.X.X::Y OR listen ::Y  OR listen Y"
# define IP_FORMAT "IP Fomart -> X.X.X.X  -> 0 < X < 255"
/*

*/
# define SERVER_CONTEXT "server"
# define LOCATION_CONTEXT "location"


/*

*/
# define SET -1
# define CLEAR -2
# define ALL_METHODS 7

/*

*/
# define bitset(byte,nbit)   (byte |= (1 << nbit))
# define bitclear(byte,nbit) (byte &= ~(1 << nbit))
# define bitcheck(byte,nbit) (byte & (1 << nbit))

#endif