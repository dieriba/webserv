# include "../../includes/utils/BitsManipulation.hpp"

/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/
BitsManipulation::BitsManipulation():_options(0){};
BitsManipulation::BitsManipulation(const BitsManipulation& rhs):_options(rhs._options){};
BitsManipulation& BitsManipulation::operator=(const BitsManipulation& rhs)
{
    if (this == &rhs) return *this;
    _options = rhs._options;
    return *this;
};
BitsManipulation::~BitsManipulation(){};
/*----------------------------------------CONSTRUCTOR/DESTRUCTOR----------------------------------------*/

/*----------------------------------------GETTER----------------------------------------*/
unsigned int& BitsManipulation::getOption(void) {return _options;};
const unsigned int& BitsManipulation::getOption(void) const { return _options ;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void    BitsManipulation::setOptions(const unsigned int& nbit, char actions)
{
    if (actions == SET)
        bitset(_options, nbit);
    else if (actions == CLEAR)
        bitclear(_options, nbit);
};
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/
/*

*/
bool    BitsManipulation::checkBits(const unsigned int& nbit) const
{
    return (bitcheck(_options, nbit) > 0);
}

void    BitsManipulation::resetOptions(void)
{
    _options = 0;
}
/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/
