#ifndef __HEADER__HPP_
#define __HEADER__HPP_

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <fstream>


# define BASE_VEC_ARR 10
# define WHITESPACES " \n\t\r\f\v"
# define BASE_10 "0123456789"

/*


*/
# define LISTEN "listen"
# define SERVER_NAMES "server_names"
# define ROOT "root"
# define ALLOWED_METHOD "allowed_methods"
# define INDEX "index"
# define CLIENT_BODY "client_max_body_size"
# define LOCATION "location"
# define ERROR_FILE "error_file"
# define REDIRECT "redirect"
# define CGI "cgi"
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
# define GET 0
# define POST 1
# define DELETE 2

/*

*/
# define SET -1
# define CLEAR -2
# define ALL_METHODS 7

/*

*/
#define bitset(byte,nbit)   (byte |= (1 << nbit))
#define bitclear(byte,nbit) (byte &= ~(1 << nbit))
#define bitcheck(byte,nbit) (byte & (1 << nbit))

#endif