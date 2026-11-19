/* Synthetic host tests for the land_agreement group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/land_agreement.h"

static int toon_calls, toon_arg, toon_result;

int Ta_GetToonAgreement(int index)
{
    toon_calls++;
    toon_arg = index;
    return toon_result;
}

static int check(int land, int var_kind)
{
    struct jp_64b6d unit;

    memset(&unit, 0, sizeof unit);
    unit.bVarKind = (unsigned char)var_kind;
    unit.wCardNo = 0x2b7;
    return Do_CheckLandAgreement(land, &unit);
}

/* The expected result of kind k on land 0..5 and 9 (lands 6, 7, 8 handled apart). */
static int expected(int land, int k)
{
    switch (land) {
    case 0: return (k == 4 || k == 5 || k == 9 || k == 17 || k == 19) ? 1 : k == 7 ? 2 : 0;
    case 1: return (k == 2 || k == 10 || k == 14 || k == 18) ? 1 : (k == 12 || k == 13 || k == 16 || k == 19) ? 2 : 0;
    case 2: return (k == 0 || k == 6 || k == 8 || k == 15) ? 1 : k == 2 ? 2 : 0;
    case 3: return (k == 3 || k == 4) ? 1 : k == 1 ? 2 : 0;
    case 4: return (k == 12 || k == 13 || k == 15 || k == 16) ? 1 : (k == 14 || k == 17) ? 2 : 0;
    case 5: return (k == 1 || k == 2 || k == 7) ? 1 : k == 8 ? 2 : 0;
    case 9: return k == 20 ? 1 : 0;
    }
    return 0;
}

int main(void)
{
    static const int lands[7] = {0, 1, 2, 3, 4, 5, 9};
    int i, k;

    /* Every kind on every listed land, including kinds past the inner tables (20..31). */
    for (i = 0; i < 7; i++) {
        for (k = 0; k < 32; k++) {
            assert(check(lands[i], k) == expected(lands[i], k));
        }
    }
    /* Land 4: inside and just outside 12..17. */
    assert(check(4, 11) == 0 && check(4, 12) == 1 && check(4, 17) == 2 && check(4, 18) == 0);
    /* Lands 7, 8, 10 and above, negative: 0 for any kind. */
    for (k = 0; k < 32; k++) {
        assert(check(7, k) == 0 && check(8, k) == 0 && check(10, k) == 0 && check(255, k) == 0 && check(-1, k) == 0);
    }
    /* A non-zero top-three-bit field gives 0 before the land is looked at. */
    toon_calls = 0;
    toon_result = 1;
    assert(check(0, 0x20 | 4) == 0 && check(9, 0xe0 | 20) == 0 && check(6, 0x40) == 0);
    assert(toon_calls == 0);
    /* Land 6: the toon agreement of the card number, 1 when non-zero, else 2; any kind. */
    toon_result = 7;
    assert(check(6, 3) == 1 && toon_calls == 1 && toon_arg == 0x2b7);
    toon_result = 0;
    assert(check(6, 31) == 2 && toon_calls == 2);
    return 0;
}
