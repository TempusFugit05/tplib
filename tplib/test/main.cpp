#include <random>
#include <sys/time.h>
#include "usr/include/string.h"
#include "/usr/include/c++/12/cstring"

#include "io.h"
#include "format.h"
#include "macro_utils.h"
#include "debug.h"
#include "math.h"

#define PRINT_INT_RUNS 100

class rng
{
    public:
        rng()
        {
            timeval seed;
            gettimeofday(&seed, NULL); // Get current time.
            srand(seed.tv_usec); // Initialize srand with nanoseconds.
        }
        // Returns a random int between min and max (both inclusive)
        int get_random(int min, int max)
        {
            ASSERT(min < max, "The minimum value must be greater than the maximum.");
            return (rand() % max) + min;
        }
};

rng rng_global;

bool test_print_int()
{
    char control_format[64];
    char test_format[64];

    for(int i = 0; i < PRINT_INT_RUNS; i++)
    {
        int random = rng_global.get_random(0, MAX_SIGNED(int));
        snprintf(control_format, ARRAY_LENGTH(control_format), "%i", random);
        str_format(test_format, ARRAY_LENGTH(control_format), "$i", random);
        if(strcmp(control_format, test_format) != 0)
        {
            return false;
        }
    }
    return true;
}

int main()
{
}
