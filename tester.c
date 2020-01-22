/**
 * Generic test - inspired by http://eradman.com/posts/tdd-in-c.html
 */
#include <stdio.h>
#include "main.h"

int tests_run = 0;
int assertions = 0;

#define FAIL() printf("\n\033[31mfailure in %s() line %d\033[0m\n", __func__, __LINE__)
#define _assert(test) do { assertions++; if (!(test)) { FAIL(); return 1; } } while(0)
#define _verify(test) do { int r = test(); tests_run++; if (r) return r; } while(0)

int sum_01()
{
    //int a = 5, b = 10;
    //_assert(intSum(a, b) == 15);
    return 0;
}

int all_tests()
{
    _verify(sum_01);

    return 0;
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    int result = all_tests();
    if (result == 0) 
        printf("\n\033[32mTESTS PASSED\033[0m\n");
    printf ("Tests run: %d, assertions: %d\n", tests_run, assertions);

    return result != 0;
}
