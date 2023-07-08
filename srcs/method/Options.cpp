# include "../../includes/method/Options.hpp"
# include "../../includes/http/HttpRequest.hpp"
# include "../../includes/http/HttpResponse.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Options::Options(){};
Options::Options(const Options& rhs):Method(rhs){};
Options& Options::operator=(const Options& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Options::~Options(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
int Options::sendResponse(ClientSocketStream& /* client */ , HttpRequest& /* req */ , HttpResponse& /* res */)
{
    
    return IO::IO_SUCCESS;
}
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
Method *Options::clone(void) const {return new Options; };
/*----------------------------------------VIRTUAL MEMBER FUNCTION----------------------------------------*/
