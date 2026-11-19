#include "dotr/ee/small_misc.h"

/* Measures the first n characters of str in font: *w receives the widest
 * line and *h the line height times the number of lines (a '\n' ends a line).
 * With bit 0 of flags set a known character advances by its own rectangle
 * width and an unknown one by half the cell width; otherwise every character
 * advances by the cell width. Either result pointer may be null. */
void lSzDuel_GetFontSize(int font, int flags, int n, char *str, int *w, int *h)
{
    int cw = gSzDuel_abFontSize[font][0];
    int ch = gSzDuel_abFontSize[font][1];
    int prop = 0;
    int maxw = 0;
    int cur = 0;
    int height = 0;
    int i;
    int idx;
    struct DotrFontRect *r;

    if (flags & 1) {
        prop = 1;
    }
    for (i = 0; i < n; i++) {
        idx = lSzDuel_GetFontIndex(str[i]);
        if (idx < 0) {
            if (str[i] == 10) {
                if (maxw < cur) {
                    maxw = cur;
                }
                height += ch;
                cur = 0;
            } else {
                cur += prop ? cw >> 1 : cw;
            }
        } else {
            r = &gSzDuel_Font[idx + font * 37];
            if (prop) {
                cur += r->right - r->left;
            } else {
                cur += cw;
            }
        }
    }
    if (maxw < cur) {
        maxw = cur;
    }
    height += ch;
    if (w) {
        *w = maxw;
    }
    if (h) {
        *h = height;
    }
}
