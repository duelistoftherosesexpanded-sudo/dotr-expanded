#include "dotr/ee/small_state.h"

void Taku_Obj_Set(int a0, struct DotrSmallSheetItem *item, short x, short y)
{
    struct DotrSmallSheetRow *row = &tex_uv_data[item->row];

    Taku_Prim_Set_SP1(a0, item->x0 + x, item->y0 + y, item->x1 + x, item->y1 + y, row->x, row->y, row->width, row->height,
                  item->color[0], item->color[1], item->color[2], item->color[3], item->unknown_20,
                  row->unknown_10, item->unknown_21, item->unknown_22);
}
