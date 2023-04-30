# ifndef __REQUEST_CHECKER_HPP__
# define __REQUEST_CHECKER_HPP__

class IO;
class HttpRequest;

class RequestChecker
{
    public:
        typedef int (*Checker)(IO&, HttpRequest);
        RequestChecker();
        RequestChecker(const RequestChecker& rhs);
        RequestChecker& operator=(const RequestChecker& rhs);
        ~RequestChecker();

        /*STATIC FUNCTION*/
        static int checkAllowedMethod(IO& serv, HttpRequest req);
        static int checkValidPath(IO& serv, HttpRequest req);

        static Checker tab[10];
};
# endif