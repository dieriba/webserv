#include "CommonLib.hpp"
#ifndef __EXECEPTION_THROWER_HPP__
#define __EXECEPTION_THROWER_HPP__
#include <exception>

class ExceptionThrower : std::exception
{
    public:
        ExceptionThrower(const std::string& exception);
        const char *what();
    private:
        char *exception;
};
#endif