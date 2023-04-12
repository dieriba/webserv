#ifndef __HEADER__HPP_
#define __HEADER__HPP_

#include <string>
#include <iostream>
#include <list>

# define GET 1
# define POST 2
# define DELETE 3
# define ALL_METHODS 7

#define bitset(byte,nbit)   (byte |= (1 << nbit))
#define bitclear(byte,nbit) (byte &= ~(1 << nbit))
#define bitcheck(byte,nbit) (byte & (1 << nbit))

#endif