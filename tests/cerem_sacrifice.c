/* Synthetic host tests for the cerem_sacrifice group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cerem_sacrifice.h"

/* Branch coverage: every case of the switch with its three words (the values are the
 * original's immediates), the default, and the neighbours of the lowest and highest case. */
static void check_cerem_sacrifice(void)
{
    static const struct { int obj; int want[3]; } rows[] = {
        { 189, { 0x07D00039, 0x07D00039, 0x07D00039 } },
        { 13, { 0x00000004, 0x00000000, 0x00000011 } },
        { 145, { 0x0063001C, 0x023D001C, 0x0216001C } },
        { 143, { 0x009B001C, 0x05DC0025, 0x05DC0025 } },
        { 337, { 0x012C001C, 0x00000007, 0x00000007 } },
        { 248, { 0x00F9001C, 0x00000005, 0x00000005 } },
        { 338, { 0x0149001C, 0x0121001C, 0x00000007 } },
        { 2, { 0x0000001C, 0x0000001C, 0x0000001C } },
        { 195, { 0x0236001C, 0x00000006, 0x00000003 } },
        { 147, { 0x00D2001C, 0x0150001C, 0x0295001C } },
        { 220, { 0x0279001C, 0x00000004, 0x00000004 } },
        { 135, { 0x0023001C, 0x00000003, 0x00000003 } },
        { 10, { 0x0005001C, 0x0125001C, 0x0006001C } },
        { 11, { 0x0008001C, 0x00000003, 0x00000006 } },
        { 596, { 0x0248001C, 0x00000010, 0x00000010 } },
        { 77, { 0x0131001C, 0x00000001, 0x00000001 } },
        { 352, { 0x014A001C, 0x03E80025, 0x03E80025 } },
        { 353, { 0x00AD001C, 0x0000000E, 0x0000000E } },
        { 422, { 0x018E001C, 0x00000009, 0x00000009 } },
        { 148, { 0x0092001C, 0x00000003, 0x00000003 } },
        { 76, { 0x029B001C, 0x00CC001C, 0x004F001C } },
        { 128, { 0x00000002, 0x0000000E, 0x00000009 } },
        { 472, { 0x0214001C, 0x0000000C, 0x0000000C } },
        { 78, { 0x003C001C, 0x05DC0025, 0x05DC0025 } },
    };
    static const int none[] = { -1, 0, 1, 3, 9, 12, 79, 190, 595, 597, 999 };
    int buf[4];
    size_t i;

    for (i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
        memset(buf, 0x55, sizeof(buf));
        Ta_GetCeremSacrifice(buf, rows[i].obj);
        assert(buf[0] == rows[i].want[0] && buf[1] == rows[i].want[1] && buf[2] == rows[i].want[2]);
        assert(buf[3] == 0x55555555);
    }
    for (i = 0; i < sizeof(none) / sizeof(none[0]); i++) {
        memset(buf, 0x55, sizeof(buf));
        Ta_GetCeremSacrifice(buf, none[i]);
        assert(buf[0] == -1 && buf[1] == -1 && buf[2] == -1 && buf[3] == 0x55555555);
    }
}

int main(void)
{
    check_cerem_sacrifice();
    return 0;
}
