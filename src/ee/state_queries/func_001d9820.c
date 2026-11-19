#include "dotr/ee/state_queries.h"

/* The side element the byte at 0x3181e4 selects; the original reads the byte back
 * after storing it. The name is not the original's. */
static inline struct DotrQuerySide *current(void)
{
    return &data_00317490_query_sides[turnOwner[0]];
}

void SzDuel_InitTurn(int unused, int side)
{
    struct DotrQueryControlEnd *c = &data_00317490_query_end;
    struct DotrQuerySide *p;
    struct DotrQuerySide *q;
    float v[4];
    float out[4];
    int b;
    int a;

    (void)unused;
    turnOwner[0] = side;
    p = &data_00317490_query_sides[side];
    a = func_001a22c0((p->unknown_33e - 3) * 50);
    b = func_001a22c0((p->unknown_33d - 3) * 50);
    q = current();
    func_001fc950_words(q->unknown_1d0, out);
    v[0] = b;
    v[2] = a;
    v[1] = q->unknown_345;
    func_001fa8a0_vec(0, 0, 3, 32, 0, v);
    func_001fa8a0_vec(0, 0, 4, 32, 0, out);
    p->unknown_346_bit_0 = -1;
    if (TaTuto_IsTuto(side)) {
        p = (struct DotrQuerySide *)c;
    }
    TaLogic_AdjustLogicData((struct DotrQueryObject1a2 *)p);
    c->unknown_d66_bit_7 = -1;
    c->unknown_d67_bit_0 = c->unknown_d66_bit_7;
}
