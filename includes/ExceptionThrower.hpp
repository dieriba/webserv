#ifndef __EXECEPTION_THROWER_HPP__
#define __EXECEPTION_THROWER_HPP__
#include "CommonLib.hpp"
#include <exception>

class ExceptionThrower : public std::exception
{
    public:
        ExceptionThrower(const std::string& exception);

        //virutal method inhirited from std::exception
        const char *what(void);
    private:
        ExceptionThrower(const ExceptionThrower& rhs);
        ExceptionThrower& operator=(const ExceptionThrower& rhs);
        ExceptionThrower();
        const char *exception;
};

#endif