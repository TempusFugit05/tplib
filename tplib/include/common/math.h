#ifndef MATH_H
#define MATH_H

#define MAX_SIGNED(TYPE) (~(TYPE(1) << ((sizeof(TYPE) * 8) - 1)))
#define MIN_SIGNED(TYPE) ((TYPE(1) << ((sizeof(TYPE) * 8) - 1)))
#define MAX_UNSIGNED(TYPE) (~TYPE(0))
#define MIN_UNSIGNED() (0)

int absolute(int value);
long reverse(int number, const int base = 10);
int power(int base, int power);

#endif //MATH_H
