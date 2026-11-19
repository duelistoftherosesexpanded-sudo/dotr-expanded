#include "dotr/ee/resource_process.h"

void lRhFile_StrUpper(char *text)
{
    int c;
    while ((c = *text) != 0) {
        if (c >= 'a' && c <= 'z') {
            *text = c - 32;
        }
        text++;
    }
}
