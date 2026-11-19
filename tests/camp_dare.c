/* Synthetic host tests for the camp_dare group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_dare.h"

struct DotrDareWindow camp_window_Teki_data[4];
struct DotrDareItem tex_bara_data2[3];
struct DotrDareItem camp_window_teki3[6];
unsigned char data_002999cc[8][4];
short dotr_dare_slide_01ed53b6[2];
short dotr_dare_name_01ed53ba[2];
signed char dotr_dare_side_01ed53be[2];
signed char dotr_dare_state_01ed5407[2];
struct DotrDareFace dotr_dare_face_01ed540c[2];
signed char dotr_dare_look_01ed5469[2];

/* Each stub appends its arguments to one log; the builders return the cursor plus one,
 * so the chain of cursors shows the call order. */
enum { OBJ = 1, WIN, FT4, FONT, FLUSH, INFO, RATIO, FREE, REQUEST, CLOSE };
static struct {
    int id;
    long a[9];
    float sx, sy;
} calls[40];
static int n_calls;
static struct DotrDareInfo info;

static DotrDareWord record(int id, DotrDareWord cursor, long a1, long a2, long a3, long a4,
                           long a5, long a6, long a7, long a8)
{
    long a[9];
    a[0] = (long)cursor, a[1] = a1, a[2] = a2, a[3] = a3, a[4] = a4;
    a[5] = a5, a[6] = a6, a[7] = a7, a[8] = a8;
    assert(n_calls < 40);
    calls[n_calls].id = id;
    memcpy(calls[n_calls++].a, a, sizeof a);
    return cursor + 1;
}

DotrDareWord Taku_Window_Set(DotrDareWord cursor, struct DotrDareWindow *window,
                             unsigned short flag, int a3, unsigned short x, int y, int a6,
                             int a7, int a8)
{
    return record(WIN, cursor, window - camp_window_Teki_data, flag, a3, x, y, a6, a7, a8);
}

DotrDareWord Taku_Obj_Set(DotrDareWord cursor, struct DotrDareItem *item, short x, short y)
{
    return record(OBJ, cursor, item - tex_bara_data2, x, y, 0, 0, 0, 0, 0);
}

DotrDareWord Taku_FT4_Set(DotrDareWord cursor, struct DotrDareItem *item, short x, short y,
                          int a4)
{
    return record(FT4, cursor, item - camp_window_teki3, x, y, a4, 0, 0, 0, 0);
}

DotrDareWord SzMsg_LoadFont_Pipe(DotrDareWord cursor, int a0, int a1, int a2, int a3)
{
    return record(FONT, cursor, a0, a1, a2, a3, 0, 0, 0, 0);
}

DotrDareWord RhText_SetTextureFlush(DotrDareWord cursor)
{
    return record(FLUSH, cursor, 0, 0, 0, 0, 0, 0, 0, 0);
}

struct DotrDareInfo *SzMsg_GetInfo(int index)
{
    record(INFO, 0, index, 0, 0, 0, 0, 0, 0, 0);
    return &info;
}

void SzMsg_Free(int index)
{
    record(FREE, 0, index, 0, 0, 0, 0, 0, 0, 0);
}

void SzMsg_DrawRect_Pipe(void)
{
}

DotrDareWord SzMsg_DrawRatioX_Pipe(DotrDareWord cursor, int index, int x, int y, float sx,
                                   float sy, void *e, void (*function)(void))
{
    unsigned char *c = e;

    assert(c[0] == 128 && c[1] == 128 && c[2] == 128 && c[3] == 128);
    assert(function == SzMsg_DrawRect_Pipe);
    calls[n_calls].sx = sx;
    calls[n_calls].sy = sy;
    return record(RATIO, cursor, index, x, y, 0, 0, 0, 0, 0);
}

int ShFace_Request(struct DotrDareFace *req)
{
    record(REQUEST, 0, req - dotr_dare_face_01ed540c, 0, 0, 0, 0, 0, 0, 0);
    return 0;
}

void ShFace_Close(struct DotrDareFace *req)
{
    record(CLOSE, 0, req - dotr_dare_face_01ed540c, 0, 0, 0, 0, 0, 0, 0);
}

static void expect(int i, int id, long a0, long a1, long a2, long a3, long a4, long a5,
                   long a6, long a7, long a8)
{
    long a[9];
    int k;

    a[0] = a0, a[1] = a1, a[2] = a2, a[3] = a3, a[4] = a4;
    a[5] = a5, a[6] = a6, a[7] = a7, a[8] = a8;
    assert(i < n_calls && calls[i].id == id);
    for (k = 0; k < 9; k++) {
        assert(calls[i].a[k] == a[k]);
    }
}

static void reset(void)
{
    memset(calls, 0, sizeof calls);
    n_calls = 0;
    memset(dotr_dare_slide_01ed53b6, 0, sizeof dotr_dare_slide_01ed53b6);
    memset(dotr_dare_face_01ed540c, 0x55, sizeof dotr_dare_face_01ed540c);
    dotr_dare_name_01ed53ba[0] = 197;
    dotr_dare_name_01ed53ba[1] = 200;
    dotr_dare_side_01ed53be[0] = 1;
    dotr_dare_side_01ed53be[1] = 2;
    dotr_dare_state_01ed5407[0] = 0;
    dotr_dare_state_01ed5407[1] = 0;
    dotr_dare_look_01ed5469[0] = 3; /* right side */
    dotr_dare_look_01ed5469[1] = 1; /* left side */
    camp_window_Teki_data[3].x = 40;
    camp_window_Teki_data[3].y = 60;
    data_002999cc[2][1] = 77;
    data_002999cc[5][3] = 88;
    info.w = 100;
}

/* Left panel with slide 512: x = -320, y = -128; the face state selects the branch. */
static void check_left(int state)
{
    struct DotrDareFace *f = &dotr_dare_face_01ed540c[0];
    int i = 2;

    reset();
    dotr_dare_slide_01ed53b6[0] = 512;
    dotr_dare_state_01ed5407[0] = (signed char)state;
    if (state == 9) { /* wide text: the ratio branch */
        info.w = 648;
    }
    assert(Taku_Camp_BUN_DARE(1000) == 1009);
    expect(0, OBJ, 1000, 1, -320, 0, 0, 0, 0, 0, 0);
    expect(1, WIN, 1001, 0, 1, 0, 0xfec0, 0, 0, 128, 128);
    if (state == 100) {
        expect(i++, CLOSE, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        assert(dotr_dare_state_01ed5407[0] == 0);
        assert(f->id == 0x55 && f->no == 0x55);
    } else if (state == 1 || state == 2 || state == 10) {
        expect(i++, REQUEST, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        assert(dotr_dare_state_01ed5407[0] == 2);
        assert(f->id == (state == 1 ? 255 : 0x55));
        assert(f->no == 2 && f->kind == 77 && f->mode == (state == 10 ? 3 : 1));
        assert(f->x == 40 && f->y == (unsigned short)(60 - 128));
    } else {
        assert(dotr_dare_state_01ed5407[0] == state);
        assert(f->id == 0x55 && f->no == 0x55 && f->mode == 0x55);
    }
    expect(i++, WIN, 1002, 3, 1, 0, 0, 0xff80, 1, 128, 0);
    expect(i++, FT4, 1003, 2, 0, -128, 0, 0, 0, 0, 0);
    expect(i++, FT4, 1004, 3, 0, -128, 0, 0, 0, 0, 0);
    expect(i++, FT4, 1005, 1, 0, -128, 0, 0, 0, 0, 0);
    expect(i++, FONT, 1006, 1, 197, 0, 22, 0, 0, 0, 0);
    expect(i++, FLUSH, 1007, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(i++, INFO, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    expect(i, RATIO, 1008, 1, -320 + 53, 21, 0, 0, 0, 0, 0);
    assert(calls[i].sx == (state == 9 ? 0.5f : 1.0f) && calls[i].sy == 1.0f);
    i++;
    expect(i++, FREE, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    assert(i == n_calls);
}

/* Right panel with slide 512: x = 320, y = 128; offsets -480 and 320 apply. */
static void check_right(int state)
{
    struct DotrDareFace *f = &dotr_dare_face_01ed540c[1];
    int i = 2;

    reset();
    dotr_dare_slide_01ed53b6[1] = 512;
    dotr_dare_state_01ed5407[1] = (signed char)state;
    if (state == 9) {
        info.w = 324; /* boundary: 324 takes the ratio branch, ratio 1 */
    }
    if (state == 8) {
        info.w = 323;
    }
    assert(Taku_Camp_BUN_DARE(2000) == 2009);
    expect(0, OBJ, 2000, 2, 320 + 144, 304, 0, 0, 0, 0, 0);
    expect(1, WIN, 2001, 0, 2, 0, 320 + 144, 304, 0, 128, 128);
    if (state == 100) {
        expect(i++, CLOSE, 0, 1, 0, 0, 0, 0, 0, 0, 0);
        assert(dotr_dare_state_01ed5407[1] == 0);
        assert(f->id == 0x55 && f->no == 0x55);
    } else if (state == 1 || state == 2 || state == 10) {
        expect(i++, REQUEST, 0, 1, 0, 0, 0, 0, 0, 0, 0);
        assert(dotr_dare_state_01ed5407[1] == 2);
        assert(f->id == (state == 1 ? 255 : 0x55));
        assert(f->no == 5 && f->kind == 88 && f->mode == (state == 10 ? 3 : 1));
        assert(f->x == (unsigned short)(40 - 480) && f->y == 60 + 320 + 128);
    } else {
        assert(dotr_dare_state_01ed5407[1] == state);
        assert(f->id == 0x55 && f->no == 0x55 && f->mode == 0x55);
    }
    expect(i++, WIN, 2002, 3, 2, 0, 0xfe20, 128 + 320, 1, 128, 0);
    expect(i++, FT4, 2003, 4, -480, 128 + 184, 2, 0, 0, 0, 0);
    expect(i++, FT4, 2004, 5, -480, 128 + 184, 2, 0, 0, 0, 0);
    expect(i++, FT4, 2005, 2, -480, 128 + 176, 0, 0, 0, 0, 0);
    expect(i++, FONT, 2006, 1, 200, 0, 22, 0, 0, 0, 0);
    expect(i++, FLUSH, 2007, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(i++, INFO, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    expect(i, RATIO, 2008, 1, 320 + 197, 325, 0, 0, 0, 0, 0);
    assert(calls[i].sx == 1.0f && calls[i].sy == 1.0f);
    i++;
    expect(i++, FREE, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    assert(i == n_calls);
}

static void check_bun_dare(void)
{
    static const int states[] = { 0, 1, 2, 9, 10, 100 };
    unsigned k;

    /* Both slide counters zero: nothing is drawn and the cursor comes back unchanged. */
    reset();
    assert(Taku_Camp_BUN_DARE(77) == 77 && n_calls == 0);
    for (k = 0; k < sizeof states / sizeof states[0]; k++) {
        check_left(states[k]);
        check_right(states[k]);
    }
    check_right(8);
    /* Both panels: the right half continues with the left half's cursor. */
    reset();
    dotr_dare_slide_01ed53b6[0] = 1024;
    dotr_dare_slide_01ed53b6[1] = 1024;
    assert(Taku_Camp_BUN_DARE(0) == 18 && n_calls == 22);
    expect(0, OBJ, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    expect(11, OBJ, 9, 2, 144, 304, 0, 0, 0, 0, 0);
}

int main(void)
{
    check_bun_dare();
    return 0;
}
