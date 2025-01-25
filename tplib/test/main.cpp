#include <random>
#include <sys/time.h>
#include <cstring>
#include <limits>

#include "io.h"
#include "format.h"
#include "macro_utils.h"
#include "debug.h"
#include "math.h"
#include "macro_helpers.h"

#define ANSI_CYAN "\x1B[36m"
#define ANSI_RESET "\x1B[0m"

#define PASS " - " ANSI_CYAN "PASS" ANSI_RESET
#define MESSAGE(INDENT, TEXT, ...) for(int i = 0; i < INDENT; i++){printf("...");} printf(TEXT "\n", __VA_ARGS__)
#define MESSAGE_NO_VA(INDENT, TEXT) for(int i = 0; i < INDENT; i++){printf("...");} printf(TEXT "\n")
#define TEST_STATUS2(STATUS, TEXT, ...) MESSAGE_NO_VA(1, TEXT STATUS)
#define TEST_STATUS3(STATUS, TEXT, ...) MESSAGE(1, TEXT STATUS, __VA_ARGS__)
#define TEST_STATUS4(STATUS, TEXT, ...) MESSAGE(1, TEXT STATUS, __VA_ARGS__)
#define TEST_STATUS5(STATUS, TEXT, ...) MESSAGE(1, TEXT STATUS, __VA_ARGS__)
#define TEST_STATUS(...) CONCATENATE(TEST_STATUS, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define NUM_RUNS 100
#define TEST_LIMIT(TEST, ...) if(!TEST(__VA_ARGS__)){return false;}

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
        int get_random(int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max())
        {
            ASSERT(min < max, "The minimum value must be greater than the maximum.");
            return (rand() + min) % max;
        }
};

rng rng_global;

class test
{
    private:
        virtual bool run_test() = 0;

    protected:
        char test_name[32] = "\0";
        char fail_reason[128] = "\0";
        void set_name(const char* const name)
        {
            strncpy(test_name, name, ARRAY_LENGTH(test_name));
        }

    public:
        bool run()
        {
            printf("Testing: %s\n", test_name);
            bool test_result = run_test();
            if(test_result)
            {
                TEST_STATUS(PASS, "Test %s!", test_name);
            }
            else
            {
                TEST_STATUS(PASS, "Test %s - FAILED! Reason: %s", test_name, fail_reason);
            }
            printf("\n");
            return test_result;
        }
};

// STRING FORMAT TESTS

// Test formatting a string with integers.
class test_print : public test
{
    private:
        void set_fail_reason(const char* expected, const char* actual) const
        {

            snprintf((char*)fail_reason, ARRAY_LENGTH(fail_reason), "tp version did not match std version! Expected: %s, got %s.", expected, actual);
        }
        
        bool compare_std2tp(const char* const control_format, const char* const test_format, ...) const
        {
            char control_output[64];
            char test_output[64];
            
            // Create two strings from the standard and tp libraries.
            va_list args_control;
            va_start(args_control, test_format);
            vsnprintf(control_output, ARRAY_LENGTH(control_output), control_format, args_control);
            va_end(args_control);
            
            va_list args_test;
            va_start(args_test, test_format);
            str_format(test_output, ARRAY_LENGTH(test_output), test_format, args_test);
            va_end(args_test);
            
            // Check if the strings match.
            if(strcmp(control_output, test_output) == 0)
            {
                return true;
            }

            set_fail_reason(control_output, test_output);
            return false;
        }

        
        bool run_test() override
        {
            // Test limits.
            TEST_LIMIT(compare_std2tp, "%i", "$i", std::numeric_limits<int>::max())
            TEST_LIMIT(compare_std2tp, "%i", "$i", std::numeric_limits<int>::min())
            TEST_LIMIT(compare_std2tp, "%i", "$i", 0)
            
            for(int i = 0; i < NUM_RUNS; i++)
            {
                int random = rng_global.get_random(); // Generate a random number to feed into format functions. 
                if(!compare_std2tp("%i", "$i", random))
                {
                    return false;
                }
            }
            return true;
        }

    public:
        test_print()
        {
            set_name("Int to string");
        }
};

// SCAN TESTS

// Test scanning a string for integers.
class test_scan : public test
{
    private:
        void set_fail_reason(const int expected, const int actual)
        {
            
             snprintf((char*)fail_reason, ARRAY_LENGTH(fail_reason),
             "tp version did not match std version! Expected: %i, got %i.", expected, actual);
        }
       
       // Create a random set of characters containing a number.
        void generate_int_string_fuzzed(char* const buff, size_t buff_size, int input)
        {
            char num_str[16];
            size_t num_str_size = snprintf(num_str, ARRAY_LENGTH(num_str), "%i", input);
            for(size_t i = 0; i < buff_size; i++)
            {
                buff[i] = 'z';
            }
            size_t num_str_offset = rng_global.get_random(0, buff_size - num_str_size); 
            strcpy(buff + num_str_offset, num_str);
            buff[num_str_offset + num_str_size] = 'z';
            buff[buff_size - 1] = '\0';
        }
  
        bool scan(int input, bool search_whole)
        {
            char control_input[32] = "\0";
            if(search_whole)
            {
                generate_int_string_fuzzed(control_input, ARRAY_LENGTH(control_input), input);
            }
            else
            {
                snprintf(control_input, ARRAY_LENGTH(control_input), "%i", input); // Create input string with random integer
            }
            int result = str_to_num(control_input, ARRAY_LENGTH(control_input), search_whole).number; // Try reading integer
            if(result != input)
            {
                set_fail_reason(input, result);
                return false;
            }
            return true;
        }
 
        bool test_scan_int(bool search_whole)
        {
             // Test limits.
            TEST_LIMIT(scan, std::numeric_limits<int>::max(), search_whole);
            TEST_LIMIT(scan, std::numeric_limits<int>::min(), search_whole);
            TEST_LIMIT(scan, 0, search_whole);

            // Test random inputs.
            for(int i = 0; i < NUM_RUNS; i++)
            {
                int random = rng_global.get_random();
                if(!scan(random, search_whole))
                {
                    return false;
                }
            }
            return true;
        }

        bool run_test() override
        {
            if(!test_scan_int(false))
            {
                return false;
            }
            else
            {
                TEST_STATUS(PASS, "Regular scan");
            }
            if(!test_scan_int(true))
            {
               return false;
            }
            else
            {
                TEST_STATUS(PASS, "Fuzzed scan");
            }
            return true;
        }

    public:
        test_scan()
        {
            set_name("Scan string for int");
        }
};

int main()
{
    test_print print_test;
    test_scan scan_test;
    test* tests[] = {&print_test, &scan_test};
    for(size_t i = 0; i < ARRAY_LENGTH(tests); i++)
    {
        tests[i]->run();
    }
}
