# include "../../includes/method/Method.hpp"
# include "../../includes/server/HttpServer.hpp"
# include "../../includes/method/Head.hpp"
# include "../../includes/method/Get.hpp"
# include "../../includes/method/Post.hpp"
# include "../../includes/method/Put.hpp"
# include "../../includes/method/Delete.hpp"
# include "../../includes/method/Options.hpp"
# include "../../includes/IO/IO.hpp"
# include "../../includes/IO/ClientSocketStream.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
Method::Method(){};
Method::Method(const Method& rhs){(void)rhs;};
Method& Method::operator=(const Method& rhs)
{
    if (this == &rhs) return *this;
    
    return *this;
};
Method::~Method(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
/*----------------------------------------GETTER----------------------------------------*/


/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER FUNCTION----------------------------------------*/

int Method::handleFileRessource(HttpResponse& res)
{
    try
    {
        char buffer[REQUEST_SIZE];

        std::ifstream& file = res.getFile();
        
        file.read(buffer, REQUEST_SIZE);

        if (res.sendResponse(buffer, file.gcount()) == IO::IO_ERROR) return (IO::IO_ERROR);

        if (file.fail() || file.eof()) res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    }
    catch(const std::exception& e)
    {
        res.setOptions(HttpResponse::HTTP_RESPONSE_FINISHED_RESPONSE, SET);
    }
    
    return IO::IO_SUCCESS;
}



/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/
Method* Method::createHead() {return new Head; }
Method* Method::createGet() {return new Get; }
Method* Method::createPost() {return new Post; }
Method* Method::createPut() {return new Put; }
Method* Method::createOptions() {return new Options; }
Method* Method::createDelete() {return new Delete; }
/*----------------------------------------MEMBER FUNCTION----------------------------------------*/


/*----------------------------------------STATIC VARIABLE----------------------------------------*/
std::vector<Method::Factory> Method::_tab;
const short int Method::INSERT = 14;
/*----------------------------------------STATIC VARIABLE----------------------------------------*/