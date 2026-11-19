/* Synthetic host tests for the camp_number group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_number.h"

struct DotrCampRect data_0029a7d0 = {{11, 22, 33, 44}};
static struct DotrCampInfo info;
static int step, cells, cell_x[4], cell_col[4], cell_row[4];

DotrCampWord SzMsg_LoadFont_Pipe(DotrCampWord cursor, int a0, int a1, int a2, int a3)
{
    assert(step++ % 5 == 0 && a0 == 1 && a1 == 7 && a2 == 0 && a3 == 22);
    info.unknown_00 = 9;
    info.rect.w[0] = 1000;
    info.rect.w[1] = 2000;
    info.rect.w[2] = 5;
    info.rect.w[3] = 6;
    return cursor + 1;
}

DotrCampWord RhText_SetTextureFlush(DotrCampWord cursor)
{
    assert(step++ % 5 == 1);
    return cursor + 2;
}

struct DotrCampInfo *SzMsg_GetInfo(int index)
{
    assert(step++ % 5 == 2 && index == 1);
    return &info;
}

DotrCampWord SzMsg_DrawRect_Pipe(DotrCampWord cursor, int index, int x, int y,
                                 struct DotrCampRect *rect, struct DotrCampColor *color)
{
    assert(step++ % 5 == 3 && index == 1 && y == 272 && cells < 4);
    assert(memcmp(rect, &data_0029a7d0, sizeof(*rect)) == 0 && rect != &data_0029a7d0);
    assert(color->r == 255 && color->g == 255 && color->b == 255 && color->a == 128);
    assert(info.unknown_00 == 9 && info.rect.w[2] == 22 && info.rect.w[3] == 22);
    assert((info.rect.w[0] - 1000) % 22 == 0 && (info.rect.w[1] - 2000) % 22 == 0);
    cell_x[cells] = x;
    cell_col[cells] = (info.rect.w[0] - 1000) / 22;
    cell_row[cells] = (info.rect.w[1] - 2000) / 22;
    cells++;
    return cursor + 4;
}

void SzMsg_Free(int index)
{
    assert(step++ % 5 == 4 && index == 1);
}

/* Branch coverage: every digit class (0, 1, 5, 6, 9, the fixed 10) and all four cells. */
static void check_number(int a, int b, int x, const int col[4], const int row[4])
{
    int i;

    step = 0;
    cells = 0;
    assert(Taku_Camp_Kazu_Disp(0x4000, a, b, x) == 0x4000 + 4 * 7);
    assert(step == 20 && cells == 4);
    for (i = 0; i < 4; i++) {
        assert(cell_x[i] == 160 + 16 * i + x);
        assert(cell_col[i] == col[i] && cell_row[i] == row[i]);
    }
}

int main(void)
{
    static const int col0[4] = {4, 2, 4, 1}, row0[4] = {1, 4, 1, 1};
    static const int col1[4] = {4, 2, 3, 0}, row1[4] = {0, 4, 1, 1};
    static const int col2[4] = {0, 2, 4, 0}, row2[4] = {0, 4, 0, 0};

    check_number(0, 7, 0, col0, row0);
    check_number(15, 96, 40, col1, row1);
    check_number(21, 51, -8, col2, row2);
    puts("camp_number: ok");
    return 0;
}
