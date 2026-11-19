/* Synthetic host tests for the slot_setup group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/slot_setup.h"

union DotrSlotSetupQuad data_00291370 = { 0 };
union DotrSlotSetupElement data_00291380;
union DotrSlotSetupQuad data_00291410 = { 0 };
struct DotrSlotSetupTriple data_00291420 = { 1.0f, 0.4f, 1.0f };
unsigned char data_00317490_bytes[0xd40];

/* Every stub appends its address and its integer arguments; a pointer argument is
 * appended as 1 when it holds the expected template copy (and is a local, not the
 * template itself), 0 for a null pointer, and fails otherwise. */
static long seen[512];
static int seen_count;
static void put(long value) { assert(seen_count < 512); seen[seen_count++] = value; }
static long same(void *in, void *template_object, size_t size)
{
    if (in == 0) {
        return 0;
    }
    assert(in != template_object && memcmp(in, template_object, size) == 0);
    return 1;
}
int SzDuel_New2Xax(int value) { put(0x1de1a0); put(value); return value + 1000; }
int SzModel_Load(int a, int b, int c, int d) { put(0x1e8780); put(a); put(b); put(c); put(d); return 1; }
int SzSound_TransferSEGrp(int index, int id, int force) { put(0x217590); put(index); put(id); put(force); return 0; }
int SzModel_SetDrawFlag(int index, int value) { put(0x1f30b0); put(index); put(value); return 0; }
int SzModel_SetScale(int index, void *in) { put(0x1f25d0); put(index); put(in != 0); return 0; }
int SzModel_SetRot(int index, void *in) { put(0x1f2430); put(index); put(same(in, &data_00291370, 16)); return 0; }
int SzModel_SetRotC(int index, void *in) { put(0x1f24c0); put(index); put(same(in, &data_00291370, 16)); return 0; }
int SzModel_SetPos(int index, void *in) { put(0x1f22b0); put(index); put(same(in, &data_00291410, 16)); return 0; }
int SzModel_SetDeform(int index, int value) { put(0x1f29e0); put(index); put(value); return 0; }
int SzModel_SetDeformRatio(int index, int value) { put(0x1f2ad0); put(index); put(value); return 0; }
int SzModel_SetAnimDirect(int index, int slot) { put(0x1f1830); put(index); put(slot); return 0; }
int SzModel_SetAnimDir(int index, int value) { put(0x1f1a00); put(index); put(value); return 0; }
int SzModel_SetAnimSpeed(int index, float value) { put(0x1f1a80); put(index); put(value == 1.0f); return 0; }
int SzModel_SetRgba(int index, int value, void *in) { put(0x1f26e0); put(index); put(value); put(in != 0); return 0; }
int SzModel_SetFilter(int index, int value, void *in) { put(0x1f2860); put(index); put(value); put(in != 0); return 0; }
int SzModel_SetIntensity(int index, void *in) { put(0x1f1ee0); put(index); put(same(in, &data_00291420, 12)); return 0; }
int SzView_SetStraightLight(int index, int k, void *in) { put(0x1f9770); put(index); put(k); put(same(in, &data_00291380, 144)); return 0; }
int SzView_SetCP(int index, void *in) { put(0x1f9d80); put(index); put(same(in, &data_00291410, 16)); return 0; }

static void start(int value)
{
    memset(data_00317490_bytes, 0x5a, sizeof(data_00317490_bytes));
    seen_count = 0;
    SzDuel_Init3DView(value);
}

/* Branch coverage: both sides of the sign test with the boundary values 0 and -1, a
 * larger positive and negative value, and both loops checked at their first and last
 * index (0 and 83; 34 and 73). */
static void check_setup(void)
{
    static const long tail[] = {
        0x1f30b0, 29, 1, 0x1f25d0, 29, 0, 0x1f2430, 29, 1, 0x1f24c0, 29, 1, 0x1f22b0, 29, 1,
        0x1f29e0, 29, 0, 0x1f2ad0, 29, 0, 0x1f1830, 29, 0, 0x1f1a00, 29, 1, 0x1f1a80, 29, 1,
        0x1f26e0, 29, 0, 0, 0x1f2860, 29, 1, 0, 0x1f1ee0, 29, 1, 0x1f9770, 0, 4, 1,
        0x1f9d80, 0, 1
    };
    static const int positive[] = { 0, 7 };
    static const int negative[] = { -1, -50 };
    size_t n = sizeof(tail) / sizeof(tail[0]);
    size_t v;
    int i;

    for (i = 0; i < 36; i++) {
        data_00291380.values[i] = (float)(i + 1);
    }
    for (v = 0; v < 2; v++) {
        start(positive[v]);
        assert(seen_count == 11);
        assert(seen[0] == 0x1de1a0 && seen[1] == positive[v]);
        assert(seen[2] == 0x1e8780 && seen[3] == 29 && seen[4] == -1);
        assert(seen[5] == positive[v] + 1000 && seen[6] == 1);
        assert(seen[7] == 0x217590 && seen[8] == 2 && seen[9] == positive[v] + 1000);
        assert(seen[10] == 0);
        /* The byte is written only on the other path. */
        assert(data_00317490_bytes[0xd35] == 0x5a);
    }
    for (v = 0; v < 2; v++) {
        start(negative[v]);
        assert((size_t)seen_count == 124 * 3 + n);
        for (i = 0; i < 84; i++) {
            assert(seen[i * 3] == 0x1f30b0 && seen[i * 3 + 1] == i && seen[i * 3 + 2] == 0);
        }
        for (i = 0; i < 40; i++) {
            long *row = &seen[(84 + i) * 3];
            assert(row[0] == 0x1f30b0 && row[1] == i + 34 && row[2] == 1);
        }
        assert(memcmp(&seen[124 * 3], tail, sizeof(tail)) == 0);
        assert(data_00317490_bytes[0xd35] == 0);
        assert(data_00317490_bytes[0xd34] == 0x5a && data_00317490_bytes[0xd36] == 0x5a);
    }
}

int main(void)
{
    check_setup();
    return 0;
}
