#ifndef DEV_DEBUG_H
#define DEV_DEBUG_H

#include "system.h"
#include "io.h"

#ifdef DEV_DEBUG
#define DEV_ASSERT(CONDITION, MESSAGE) if(!(CONDITION)){print("\nDev assertion failed at %s line %i: %s\n", __FILE__, __LINE__, MESSAGE); abort();}
//#define DEV_ASSERT(CONDITION, MESSAGE, ...) if(!(CONDITION)){print_formatted("\nDev assertion failed at %s %i: %s\n", __LINE__, __FILE__, MESSAGE, __VA_ARGS__); abort();}
#else
    #define DEV_ASSERT(CONDITION, MESSAGE)
#endif

#endif // DEV_DEBUG_H
