#include "dotr/ee/slot_table.h"

/* The width test goes through a 0/1 value in the original (li, movz, beqz); an inline
 * helper spells that. */
static inline int narrow_p(unsigned short x) { return (x & 0x2000) ? 1 : 0; }

int lSzMsg_FontTrace(unsigned short *compressedText, char (*charWidths)[53], short (*code)[53], unsigned short *cols,
                     unsigned short *widths, unsigned short *line)
{
    struct DotrSlotTable *ml = &gSzMsg_ml;
    unsigned short word = *compressedText;
    unsigned short c = word & 0x1fff;
    unsigned char spacing = narrow_p(word) ? 12 : 22;
    int ret = 1;
    int numberOf2bytesToSkip;
    int subLength;
    unsigned short *q;

    if (word & 0x4000) {
        word = *++compressedText;
        subLength = c & 0x3f;
        q = &StringBlob[(&StringOffsetTable)[word & 0x3fff]];
        q += (c >> 6) & 0x7f;
        while (subLength != 0) {
            numberOf2bytesToSkip = lSzMsg_FontTrace(q, charWidths, code, cols, widths, line);
            if (q[numberOf2bytesToSkip - 1] & 0x8000) {
                break;
            }
            subLength -= numberOf2bytesToSkip;
            q += numberOf2bytesToSkip;
        }
        ret = 2;
    } else if (c == ml->unknown_278) {
        (*line)++;
    } else {
        widths[*line] += spacing;
        charWidths[*line][cols[*line]] = spacing >> 1;
        code[*line][cols[*line]] = c;
        cols[*line]++;
    }
    return ret;
}
