#include "dotr/ee/small_misc.h"

void tri_diag(float *lower, float *diagonal, float *upper, float *right, float *solution, int n)
{
    float gamma[64];
    float pivot;
    int j;

    solution[0] = right[0] / (pivot = diagonal[0]);
    for (j = 1; j < n; j++) {
        gamma[j] = upper[j - 1] / pivot;
        pivot = diagonal[j] - lower[j] * gamma[j];
        solution[j] = (right[j] - lower[j] * solution[j - 1]) / pivot;
    }
    for (j = n - 2; j >= 0; j--)
        solution[j] -= gamma[j + 1] * solution[j + 1];
}
