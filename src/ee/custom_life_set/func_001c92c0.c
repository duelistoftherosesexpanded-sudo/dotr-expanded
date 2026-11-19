#include "dotr/ee/custom_life_set.h"

/* lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe (0x1c92c0): draws the life-point page of the
 * custom-duel menu. */

static inline void set_color_lifeset(struct DotrLifeSetColor *a, struct DotrLifeSetColor *b,
                                     struct DotrLifeSetSource *src) {
    a->r = b->r = src->r;
    a->g = b->g = src->g;
    a->b = b->b = src->b;
    a->a = b->a = src->a;
    a->q = b->q = 1.0f;
}

static inline void set_xyz_lifeset(struct DotrLifeSetVertex *v, int x, int y, unsigned char *state) {
    v->x = x << 4;
    v->y = y << 4;
    v->z = *(int *)(state + 0xC58);
    v->fog = 0;
}

DotrLifeSetWord lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe(DotrLifeSetWord arg0, int player, int x, int y) {
    unsigned char *state = data_00317490_lifeset;
    signed char *text3 = data_002bf05b_lifeset;
    signed char *text4 = data_002bef93_lifeset;
    int tmp = (DOTR_LIFE_SET_MENU(player) & 0xF) + 0x81;
    int width;
    int hint = 0;
    int cx;
    DotrLifeSetWord id;
    short *life;
    int px;
    int py;
    int w;
    int h;
    struct DotrLifeSetPair colors;
    struct DotrLifeSetPacket packet __attribute__((aligned(8)));

    if (DOTR_LIFE_SET_WIDE != 0) {
        width = 0x244;
    } else {
        width = 0x114;
    }
    id = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(arg0, 1, tmp, 0, 0x16));
    cx = x + (width >> 1);
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, 3, 4, cx - 0x59, y, -0xB2, -0x20, 1);
    id = SzMsg_DrawX_Pipe(id, 1, cx - (((int *)SzMsg_GetInfo(1))[3] >> 1), y + 5, 0, SzMsg_DrawRect_Pipe);
    SzMsg_Free(1);
    id = lSzDuel_DrawWindow_Frame_Pipe(id, player, -1, 0xB, x, y + 0x20, -width, -0x96, 1);
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xC, -1, 5, cx - 0x38, y + 0x38, -0x70, -0x14, 1);
    px = x + 0x68A + (width >> 1);
    py = y + 0x75A;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a1c_lifeset, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1,
                                        hint != 0 ? 0x40 : 0x80);
    life = &DOTR_LIFE_SET_LIFE[player];
    id = SzDuel_DrawWindow_FontDigit_Pipe(id, cx - 0xE, y + 0x3C, 0, 0, 4, *life);
    px = x + 0x64E + (width >> 1);
    py = y + 0x76C;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290896_lifeset, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1,
                                        hint != 0 ? 0x40 : 0x80);
    tmp = cx - 0x58;
    id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xD, -1, 7, tmp, y + 0x50, -0xB0, -0x10, 1);
    px = x + 0x668 + (width >> 1);
    py = y + 0x770;
    w = (int)((float)*life * 176.0f / 9999.0f);
    h = 8;
    colors = data_002914a0_lifeset;
    if (player == 0) {
        colors.c[1].b = 0;
        colors.c[1].g = 0;
        colors.c[0].b = 0;
        colors.c[0].g = 0;
    }
    ((struct DotrLifeSetTag *)packet.tag)->flags = 0x14;
    ((struct DotrLifeSetTag *)packet.tag)->count = 8;
    ((struct DotrLifeSetTag *)packet.tag)->registers = 0x41414141;
    ((struct DotrLifeSetTag *)packet.tag)->padding = 0;
    set_color_lifeset(&packet.points[0].color, &packet.points[1].color, &colors.c[0]);
    set_color_lifeset(&packet.points[2].color, &packet.points[3].color, &colors.c[1]);
    set_xyz_lifeset(&packet.points[0].xyz, px, py, state);
    set_xyz_lifeset(&packet.points[1].xyz, px + w, py, state);
    set_xyz_lifeset(&packet.points[2].xyz, px, py + h, state);
    set_xyz_lifeset(&packet.points[3].xyz, px + w, py + h, state);
    id = ExSortPrim_Pipe(id, (DotrLifeSetWord)packet.tag, 0, 0);
    set_color_lifeset(&packet.points[0].color, &packet.points[1].color, &colors.c[1]);
    set_color_lifeset(&packet.points[2].color, &packet.points[3].color, &colors.c[0]);
    set_xyz_lifeset(&packet.points[0].xyz, px, py + h, state);
    set_xyz_lifeset(&packet.points[1].xyz, px + w, py + h, state);
    set_xyz_lifeset(&packet.points[2].xyz, px, py + h * 2, state);
    set_xyz_lifeset(&packet.points[3].xyz, px + w, py + h * 2, state);
    id = ExSortPrim_Pipe(id, (DotrLifeSetWord)packet.tag, 0, 0);
    id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, -1, 7, tmp, y + 0x50, -0xB0, -0x10, 1);
    px = x + 0x71A + (width >> 1);
    py = y + 0x76C;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_002908a0_lifeset, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1,
                                        hint != 0 ? 0x40 : 0x80);
    id = lSzDuel_DrawWindow_ExBoard_Pipe(id, player, x + 0x12, y + 0x7A, -(width - 0x24), -0x34, -1, 0);
    px = x + 0x6C2 + ((width - 0x24) >> 2);
    py = y + 0x7A1;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a58_lifeset, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1,
                                        hint != 0 ? 0x40 : 0x80);
    px = x + 0x6D2 + ((width - 0x24) >> 2);
    py = y + 0x7A1;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a62_lifeset, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1,
                                        hint != 0 ? 0x40 : 0x80);
    px = x + 0x12 + ((width - 0x24) >> 2);
    py = y + 0x9C;
    id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 2, func_0019bef0((char *)text3), text3,
                                     hint != 0 ? (DotrLifeSetWord)data_002be944_lifeset : 0);
    px = x + 0x6CA + (((width - 0x24) * 2) >> 2);
    py = y + 0x7A1;
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a6c_lifeset, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1,
                                        hint != 0 ? 0x40 : 0x80);
    px = x + 0x12 + (((width - 0x24) * 2) >> 2);
    py = y + 0x9C;
    return SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 2, func_0019bef0((char *)text4), text4,
                                       hint != 0 ? (DotrLifeSetWord)data_002be944_lifeset : 0);
}
