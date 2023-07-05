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
        unsigned int  getOptions(void) const ;
        /*SETTER*/
        void    setOptions(const unsigned int&, char);
        /*MEMBER FUNCTION*/
        bool    checkBits(const unsigned int&) const;
        void    resetOptions(void);

    protected:
        unsigned int _options;

};
#endif