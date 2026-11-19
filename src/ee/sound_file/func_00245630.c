#include "dotr/ee/sound_file.h"

/* Same body as copy2area. Writes the concatenation of a and b into first, spilling into
 * second, via func_00198790. Returns 0 if the destinations are too small, else the total. */
int cpy2area(char *first, int first_size, char *second, int second_size, char *a, int a_size, char *b, int b_size)
{
    int rest;
    int total = a_size + b_size;
    if (first_size + second_size < total) {
        return 0;
    }
    if (a_size >= first_size) {
        func_00198790(first, a, first_size);
        func_00198790(second, a + first_size, a_size - first_size);
        func_00198790(second + a_size - first_size, b, b_size);
    } else if (b_size >= (rest = first_size - a_size)) {
        func_00198790(first, a, a_size);
        func_00198790(first + a_size, b, rest);
        func_00198790(second, b + first_size - a_size, b_size - rest);
    } else {
        func_00198790(first, a, a_size);
        func_00198790(first + a_size, b, b_size);
    }
    return total;
}
