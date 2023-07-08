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
uint32_t& BitsManipulation::getOption(void) {return _options;};
const uint32_t& BitsManipulation::getOption(void) const { return _options ;};
/*----------------------------------------GETTER----------------------------------------*/

/*----------------------------------------SETTER----------------------------------------*/
void    BitsManipulation::setOptions(const uint32_t& nbit, char actions)
{
    if (actions == SET)
        bitset(_options, nbit);
    else if (actions == CLEAR)
        bitclear(_options, nbit);
};
/*----------------------------------------SETTER----------------------------------------*/

/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/

void BitsManipulation::resetAndPreserverSomeFlags(const uint32_t &flags)
{
    _options = 0 | flags;
}

uint32_t    BitsManipulation::checkBits(const short int& nbit) const
{
    if (bitcheck(_options, nbit) > 0)
        return ((unsigned int)1 << nbit);
    return 0;
}

void    BitsManipulation::resetOptions(void)
{
    _options = 0;
}
/*----------------------------------------MEMBER/FUNCTION----------------------------------------*/
