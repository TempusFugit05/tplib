#ifndef MATH_H
#define MATH_H

#define MAX_SIGNED(TYPE) (~(TYPE(1) << ((sizeof(TYPE) * 8) - 1)))
#define MIN_SIGNED(TYPE) ((TYPE(1) << ((sizeof(TYPE) * 8) - 1)))
#define MAX_UNSIGNED(TYPE) (~TYPE(0))
#define MIN_UNSIGNED() (0)

int absolute(int value);
long reverse(int number, const int base = 10);
int power(int base, int power);

template <typename T> T power(T base, int power)
{
    T result = base;
    if(power == 0)
    {
        return 0;
    }
    for(int i = 1; i < power; i++)
    {
        result *= base;
    }
    return result;
}

#endif //MATH_H
