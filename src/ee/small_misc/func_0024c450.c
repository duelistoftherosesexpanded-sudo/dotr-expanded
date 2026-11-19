#include "dotr/ee/small_misc.h"

void Swap(int *array, int indexA, int indexB)
{
    int tmp = array[indexA];
    array[indexA] = array[indexB];
    array[indexB] = tmp;
}
