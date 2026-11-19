#include "dotr/ee/view_state.h"

struct DotrDrawTag *RhView_SendViewParameters(struct DotrDrawTag *tag)
{
    unsigned char raw[0x50];
    struct DotrViewMatrix *m;
    struct DotrViewEntry *e;
    m = (struct DotrViewMatrix *)(((DotrViewWord)raw + 15) & -16);
    e = &gRhView_avl[gRhView_nStackIndex[0]];
    tag = RhVu1Mac_SetViewScreenMatrix(tag, &e->unknown_080);
    RhGeom_InvertMatrix(m, &e->unknown_000);
    tag = RhVu1Mac_SetScreenWorldMatrix(tag, m);
    tag = RhVu1Mac_SetClipParameter(tag, 1, e->unknown_160, e->unknown_16c, e->unknown_170);
    tag = (struct DotrDrawTag *)RhBase_SetFogColor((struct DotrDrawPacket *)tag, (unsigned char *)&e->unknown_184);
    return RhVu1Mac_SetFogParameterEx(tag, e->unknown_174, e->unknown_178, e->unknown_17c, e->unknown_180);
}
