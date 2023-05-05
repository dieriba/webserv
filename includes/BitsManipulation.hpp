#ifndef __BITS_MANIUPULATION__
#define __BITS_MANIUPULATION__
#include "CommonLib.hpp"
class BitsManipulation
{
    public:
        BitsManipulation();
        BitsManipulation(const BitsManipulation& rhs);
        BitsManipulation& operator=(const BitsManipulation& rhs);
        ~BitsManipulation();

        /*GETTER*/
        unsigned int  getOptions(void) const ;
        /*SETTER*/
        void    setOptions(const unsigned int& nbit, char actions);
        /*MEMBER FUNCTION*/
        bool    checkBits(const unsigned int& nbit) const;
<<<<<<< HEAD
        void    resetOptions(void);

    protected:
        unsigned int _options;

=======

    protected:
        unsigned int _options;
        
    private:
>>>>>>> origin/main
};
#endif