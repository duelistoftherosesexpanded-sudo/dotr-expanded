#include "dotr/ee/small_misc.h"

void Taku_Coord_Init(struct DotrMiscTriple *m)
{
    m->unknown_00.unknown_w = 1.0f;
    m->unknown_00.z = 1.0f;
    m->unknown_00.y = 1.0f;
    m->unknown_00.x = 1.0f;
    m->unknown_10.unknown_w = 0.0f;
    m->unknown_10.z = 0.0f;
    m->unknown_10.y = 0.0f;
    m->unknown_10.x = 0.0f;
    m->unknown_20.unknown_w = 0.0f;
    m->unknown_20.z = 0.0f;
    m->unknown_20.y = 0.0f;
    m->unknown_20.x = 0.0f;
}
