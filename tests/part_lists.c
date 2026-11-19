/* Synthetic host tests for the part_lists group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/part_lists.h"

static int log_id[32], log_a[32], log_b[32], log_count, gate_a, gate_b;
static void note(int id, int a, int b) { log_id[log_count] = id; log_a[log_count] = a; log_b[log_count] = b; log_count++; }
int RhBase_GetUserGsAddress(void) { return 0x4000; }
int SzText_InitClutManage(int base, int size) { assert(base == 0x4000 && size == 2); return 0x20; }
int SzModel_GetPauseFlag(int index) { assert(index == 7); return gate_a; }
int SzModel_GetStoneFlag(int index) { assert(index == 7); return gate_b; }
int func_00202fb0_out(int cursor, int a0, int a1, int *out)
{
    note(1, a0, a1);
    *out = a0 * 0x100;
    return cursor + 1;
}
int SzModel_LoadTexAnim_Pipe(int cursor, int index, int position, struct DotrPartRecord *record)
{
    assert(index == 7);
    note(2, record->unknown_0, position);
    return cursor + 0x10;
}
int RhText_SetTextureFlush(int cursor) { note(3, cursor, 0); return cursor + 0x1000; }

static struct {
    struct DotrPartModel model;
    int gap[2];
    struct DotrPartList list;
    struct DotrPartRecord more[3];
} fixture;

static void reset(int count)
{
    static const int first[4] = {2, 0, 3, 5}, extra[4] = {1, 9, 0, 5};
    int i;
    memset(&fixture, 0, sizeof(fixture));
    fixture.model.header.list_offset = (int)offsetof(__typeof__(fixture), list);
    fixture.model.header.unknown_2c = 1;
    fixture.list.count = count;
    for (i = 0; i < 4; i++) {
        (&fixture.list.records[0])[i].unknown_0 = first[i];
        (&fixture.list.records[0])[i].unknown_8 = extra[i];
        (&fixture.list.records[0])[i].unknown_c = 40;
        (&fixture.list.records[0])[i].unknown_4 = 0x77;
    }
    log_count = 0;
    gate_a = gate_b = 0;
}

/* SzModel_LoadTexture_Pipe (branch and loop coverage): null model, zero header word, null size
 * pointer, empty list, a four-record list with an empty record and a record whose word
 * +8 is clear, and each of the two index gates closing the second step. */
static void check_record_walk(void)
{
    struct DotrPartRecord *r = fixture.list.records;
    int size = -1;

    reset(4);
    assert(SzModel_LoadTexture_Pipe(100, 7, 0, &size) == 100 && size == 0 && log_count == 0);
    fixture.model.header.unknown_2c = 0;
    size = -1;
    assert(SzModel_LoadTexture_Pipe(100, 7, &fixture.model, &size) == 100 && size == 0 && log_count == 0);

    reset(0);
    size = -1;
    assert(SzModel_LoadTexture_Pipe(100, 7, &fixture.model, &size) == 0x1064 && size == 0x20);
    assert(log_count == 1 && log_id[0] == 3 && log_a[0] == 100);

    reset(4);
    size = -1;
    assert(SzModel_LoadTexture_Pipe(100, 7, &fixture.model, &size) == 100 + 3 + 0x20 + 0x1000);
    /* records 0, 2 and 3 emit; record 1 is empty, record 2 has word +8 clear */
    assert(log_count == 6 && size == 0x20 + 0x200 + 0x300 + 0x500);
    assert(log_id[0] == 1 && log_a[0] == 2 && log_b[0] == 0x4020);
    assert(log_id[1] == 2 && log_a[1] == 2 && log_b[1] == 0x4020);
    assert(log_id[2] == 1 && log_a[2] == 3 && log_b[2] == 0x4220);
    assert(log_id[3] == 1 && log_a[3] == 5 && log_b[3] == 0x4520);
    assert(log_id[4] == 2 && log_a[4] == 5 && log_b[4] == 0x4520 && log_id[5] == 3);
    assert(r[0].unknown_c == 41 && r[1].unknown_c == 40 && r[2].unknown_c == 40 && r[3].unknown_c == 41);
    assert(r[0].unknown_4 == 0x77 && r[0].unknown_0 == 2 && fixture.list.count == 4);

    reset(1);
    gate_a = 1;
    assert(SzModel_LoadTexture_Pipe(100, 7, &fixture.model, 0) == 0x1065 && log_count == 2 && r[0].unknown_c == 40);
    reset(1);
    gate_b = 1;
    assert(SzModel_LoadTexture_Pipe(100, 7, &fixture.model, 0) == 0x1065 && log_count == 2 && r[0].unknown_c == 40);
    reset(1);
    assert(SzModel_LoadTexture_Pipe(100, 7, &fixture.model, 0) == 0x1075 && log_count == 3 && r[0].unknown_c == 41);
}

int main(void)
{
    check_record_walk();
    return 0;
}
