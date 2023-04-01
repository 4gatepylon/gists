#include <stdio.h>

// Trying to get worthwhile recursive macros to write my code for me
// https://stackoverflow.com/questions/12447557/can-we-have-recursive-macros
// https://stackoverflow.com/questions/26053959/what-does-va-args-in-a-macro-mean
// https://stackoverflow.com/questions/12434626/a-define-in-c-with-three-dots
// https://stackoverflow.com/questions/860273/macro-producing-macros-in-c


//https://gcc.gnu.org/onlinedocs/gcc-3.0.1/cpp_3.html

# define EMPTY(...)
# define DEFER(...) __VA_ARGS__ EMPTY()
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
# define EXPAND(...) __VA_ARGS__

# define pr_id() pr
# define pr(n) ((n==1)? 1 : DEFER(pr_id)()(n-1))

int main ()
{
    printf("result: %d\n", pr(5,4));
    return 0;
}