#include "../includes/ExceptionThrower.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
ExceptionThrower::ExceptionThrower(const std::string& exception):exception(exception.c_str()){};
ExceptionThrower::ExceptionThrower(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/
const char *ExceptionThrower::what(void){return exception; };
/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/