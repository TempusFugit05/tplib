#ifndef MATH_H
#define MATH_H
#define MIN_SIGNED(TYPE) ((TYPE(1) << ((sizeof(TYPE) * 8) - 1)))

int absolute(int value);
long reverse(int number, const int base = 10);
int power(int base, int power);

template <typename T>
T reverse_us(T number, const int base = 10)
{
    T reversed = 0;
    T remainder = 0;

    while(number != 0)
    {
        remainder = number % base;
        reversed *= base;
        reversed += remainder;
        number /= base;
    }
    return reversed;
}

#endif //MATH_H
