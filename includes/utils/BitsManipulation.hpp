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
        uint64_t&  getOption(void);
        const uint64_t&  getOption(void) const;
        /*SETTER*/
        void    setOptions(const uint64_t&, char);
        /*MEMBER FUNCTION*/
        void    resetAndPreserverSomeFlags(const uint64_t& flags);
        void    resetOptions(void);
        uint64_t    checkBits(const short int&) const;

    protected:
        uint64_t _options;

};
#endif