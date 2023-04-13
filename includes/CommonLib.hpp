#ifndef __HEADER__HPP_
#define __HEADER__HPP_

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>


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