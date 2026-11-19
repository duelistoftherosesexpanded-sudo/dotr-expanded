#include "dotr/ee/state_records.h"

/* For the record selected by the byte at 0x3181e4: converts its four words
 * at +0x1d0 through SzView_IV2V and submits {x, signed byte +0x345, y}
 * with kind 3 and the converted values with kind 4, both tagged with the
 * low 16 bits of b. */
void lSzDuel_LockCamera(int b, int x, int y)
{
    struct DotrStateRecordView *r = &data_00317490_recview[turnOwner[0]];
    float out[4];
    float v[4];
    func_001fc950_words(r->unknown_1d0, out);
    v[0] = x;
    v[2] = y;
    v[1] = r->unknown_345;
    func_001fa8a0_vec(0, 0, 3, b, 0, v);
    func_001fa8a0_vec(0, 0, 4, b, 0, out);
}
