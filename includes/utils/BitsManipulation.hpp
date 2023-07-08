#ifndef __BITS_MANIUPULATION__
#define __BITS_MANIUPULATION__
#include "CommonLib.hpp"
class BitsManipulation
{
    public:
        BitsManipulation();
        BitsManipulation(const BitsManipulation&);
        BitsManipulation& operator=(const BitsManipulation&);
        ~BitsManipulation();

        /*GETTER*/
        uint32_t&  getOption(void);
        const uint32_t&  getOption(void) const;
        /*SETTER*/
        void    setOptions(const uint32_t&, char);
        /*MEMBER FUNCTION*/
        void    resetAndPreserverSomeFlags(const uint32_t& flags);
        void    resetOptions(void);
        uint32_t    checkBits(const short int&) const;

    protected:
        uint32_t _options;

};
#endif