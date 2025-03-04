#include "io.h"
#include "math.h"

// Returns the absolute value of a number.
int absolute(int value)
{
    if(value == 0)
    {
        return(value);
    }
    if(value == MIN_SIGNED(int))
    {
        return(value - 1);
    } // Intentional integer underflow to get the maximum value if integer is maximally negative.
    value--;
    value = ~value;
    value = (unsigned int)value << 1;
    value = (unsigned int)value >> 1;
    return value;
}

// Reverse a number (ie 123 -> 321).
// Will return a larger type than the input to prevent an overflow with some numbers.
// For example, an input int 1999999999 will be 9999999991 Which will cause an overflow.
long reverse(int number, const int base)
{
    long reversed = 0;
    int remainder = 0;

    while(number != 0)
    {
        remainder = number % base;
        reversed *= base;
        reversed += remainder;
        number /= base;
    }
    return reversed;
}
