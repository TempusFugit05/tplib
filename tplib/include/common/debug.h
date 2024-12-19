#ifndef DEBUG_H
#define DEBUG_H
#include "io.h"
#include "system.h"

#ifndef NDEBUG
    #define ASSERT(CONDITION, MESSAGE) if(!(CONDITION)){print("\nAssertion failed at %s line %i: %s\n", __FILE__, __LINE__, MESSAGE); abort();}
#else
    #define ASSERT(CONDITION, MESSAGE)
#endif

#endif // DEBUG_H
