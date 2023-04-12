#include "../includes/ExceptionThrower.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
ExceptionThrower::~ExceptionThrower() throw() {};
ExceptionThrower::ExceptionThrower(const std::string& message):message(message){};
ExceptionThrower::ExceptionThrower(const ExceptionThrower& rhs):message(rhs.message){};
ExceptionThrower& ExceptionThrower::operator=(const ExceptionThrower& rhs)
{
    message = rhs.message;
    return *this;
}
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/
const char *ExceptionThrower::what(void)const throw(){return message.c_str(); };
/*----------------------------------------VIRTUAL FUNCTION----------------------------------------*/