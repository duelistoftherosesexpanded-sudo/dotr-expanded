#include "dotr/ee/map_edit_graph.h"

/* lSzDuel_DrawMapEdit_Graph_Pipe: the map-edit screen's field graph. Reads the field kind under
 * the cursor, draws the panel frame, then for each of the ten field kinds counts the cells of
 * the 7 x 7 map holding it and draws its icon (highlighted when it is the kind under the
 * cursor), a bar frame, a two-band gouraud bar count * 84 / 49 wide and 8 + 8 high, the
 * frame's second pass and the count in digits. Returns the cursor.
 * Two inline helpers set the vertex colours and positions. */

static inline void SetColor_MapGraph(struct DotrMapGraphColor *a, struct DotrMapGraphColor *b,
                                     struct DotrMapGraphSource *src)
{
    a->r = b->r = src->r;
    a->g = b->g = src->g;
    a->b = b->b = src->b;
    a->a = b->a = src->a;
    a->q = b->q = 1.0f;
}

static inline void SetXyz_MapGraph(struct DotrMapGraphXyz *v, int x, int y, unsigned char *state)
{
    v->x = x << 4;
    v->y = y << 4;
    v->z = *(int *)(state + 0xC58);
    v->fog = 0;
}

DotrMapGraphWord lSzDuel_DrawMapEdit_Graph_Pipe(DotrMapGraphWord arg0, int x, int y)
{
    unsigned char *state = data_00317490_mapgraph;
    unsigned char cur;
    int j;
    int i;
    int row;
    int count;
    int px;
    int py;
    int w;
    int h;
    DotrMapGraphWord id;
    struct DotrMapGraphPacket packet __attribute__((aligned(8)));

    cur = SzDuel_GetField(DOTR_MAPGRAPH_CURSOR_X, DOTR_MAPGRAPH_CURSOR_Y);
    id = lSzDuel_DrawWindow_Frame_Pipe(arg0, 0, -1, 0xB, x, y, -0x6E, -0xAA, 1);
    for (row = 0; row < 10; row++) {
        count = 0;
        for (j = 0; j < 7; j++) {
            for (i = 0; i < 7; i++) {
                if (row == SzDuel_GetField(i, j)) {
                    count++;
                }
            }
        }
        px = x + 0x6C5;
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, gSzDuel_Icon_mapgraph + (row + 0x10E) * 10, px, y + 0x725 + row * 16, 0,
                                            1, 0, 0, 0, 0, 1, 0, (cur == row) ? 1 : -1, 0x80);
        id = lSzDuel_DrawWindow_Frame_Pipe(id, 0xD, -1, 5, x + 0x15, y + 5 + row * 16, -0x54, -0x10, 1);
        px = x + 0x6D5;
        py = y + 0x725 + row * 16;
        w = (int)((float)count * 84.0f / 49.0f);
        h = 8;
        ((struct DotrMapGraphTag *)packet.tag)->flags = 0x14;
        ((struct DotrMapGraphTag *)packet.tag)->count = 8;
        ((struct DotrMapGraphTag *)packet.tag)->registers = 0x41414141;
        ((struct DotrMapGraphTag *)packet.tag)->padding = 0;
        SetColor_MapGraph(&packet.points[0].color, &packet.points[1].color,
                          &((struct DotrMapGraphSource *)gSzDuel_aaicolField_mapgraph)[row * 3]);
        SetColor_MapGraph(&packet.points[2].color, &packet.points[3].color,
                          &((struct DotrMapGraphSource *)gSzDuel_aaicolField_mapgraph)[row * 3 + 1]);
        SetXyz_MapGraph(&packet.points[0].xyz, px, py, state);
        SetXyz_MapGraph(&packet.points[1].xyz, px + w, py, state);
        SetXyz_MapGraph(&packet.points[2].xyz, px, py + h, state);
        SetXyz_MapGraph(&packet.points[3].xyz, px + w, py + h, state);
        id = ExSortPrim_Pipe(id, (DotrMapGraphWord)packet.tag, 0, 0);
        SetColor_MapGraph(&packet.points[0].color, &packet.points[1].color,
                          &((struct DotrMapGraphSource *)gSzDuel_aaicolField_mapgraph)[row * 3 + 1]);
        SetColor_MapGraph(&packet.points[2].color, &packet.points[3].color,
                          &((struct DotrMapGraphSource *)gSzDuel_aaicolField_mapgraph)[row * 3 + 2]);
        SetXyz_MapGraph(&packet.points[0].xyz, px, py + h, state);
        SetXyz_MapGraph(&packet.points[1].xyz, px + w, py + h, state);
        SetXyz_MapGraph(&packet.points[2].xyz, px, py + h * 2, state);
        SetXyz_MapGraph(&packet.points[3].xyz, px + w, py + h * 2, state);
        id = ExSortPrim_Pipe(id, (DotrMapGraphWord)packet.tag, 0, 0);
        id = lSzDuel_DrawWindow_Frame_Pipe(id, -1, -1, 5, x + 0x15, y + 5 + row * 16, -0x54, -0x10, 1);
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, x + 0x3F, y + 7 + row * 16, 0, 2, 2, count);
    }
    return id;
}
