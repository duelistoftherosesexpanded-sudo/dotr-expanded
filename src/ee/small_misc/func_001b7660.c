#include "dotr/ee/small_misc.h"

/* Maps a character to a base-36-style digit: 'A'-'Z' and 'a'-'z' to 0-25,
 * '0'-'9' to 26-35, '?' to 36, anything else to -1. */
int lSzDuel_GetFontIndex(char character)
{
    int fontIndex = -1;
    if (character >= 65 && character <= 90) fontIndex = character - 65;
    else if (character >= 97 && character <= 122) fontIndex = character - 97;
    else if (character >= 48 && character <= 57) fontIndex = character - 22;
    else if (character == 63) fontIndex = 36;
    return fontIndex;
}
