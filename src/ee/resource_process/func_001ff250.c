#include "dotr/ee/resource_process.h"

void lSzFile_StrNormalize(char *text)
{
    int backslash = 0;
    int c;
    if ((text[0] == '/' && text[1] == '/') || (text[0] == '\\' && text[1] == '\\')) {
        backslash = 1;
    }
    lRhFile_StrUpper(text);
    for (; (c = *text) != 0; text++) {
        if (backslash) {
            if (c == '/') {
                *text = '\\';
            }
        } else if (c == '\\') {
            *text = '/';
        }
    }
}
