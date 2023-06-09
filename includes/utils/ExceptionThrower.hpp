#ifndef __EXECEPTION_THROWER_HPP__
#define __EXECEPTION_THROWER_HPP__
#include "CommonLib.hpp"

class ExceptionThrower : public std::exception
{
    public:
        ExceptionThrower(const std::string&);
        ExceptionThrower(const ExceptionThrower&);
        ExceptionThrower& operator=(const ExceptionThrower&);
        ~ExceptionThrower() throw();
        
        /*virutal method inhirited from std::exception*/
        virtual const char* what(void) const throw();
    private:
        ExceptionThrower();
        std::string message;
};

#endif