#include "dotr/ee/block_table.h"

struct DotrDrawTag *SzView_Set_Pipe(struct DotrDrawTag *tag, int index)
{
    struct DotrBlockViewArgs args;
    struct DotrBlock *p;
    struct DotrBlockHead *h;

    if (index < 0 || index >= 1) {
        return tag;
    }
    p = gSzView_hView_blocks[index];
    if (p == 0) {
        return tag;
    }
    h = (struct DotrBlockHead *)p;
    RhView_SetScreen(h->screenDistance, 640.0f, 448.0f, 320.0f, 224.0f);
    RhView_SetAspect(h->xAspectRatio, h->yAspectRatio);
    RhView_SetClipZ(h->minZClip, h->maxZClip);
    Ex3d_InitClipPlane();
    RhView_SetFogParameter(p->unknown_7e0[9].x, p->unknown_7e0[9].y, p->unknown_830[9].x, p->unknown_830[9].y,
                  (struct DotrViewQuad *)&p->fogColour[9]);
    args.unknown_40 = h->sourcePoint;
    args.unknown_50 = h->relativeDirection;
    args.unknown_80 = h->unknown_40;
    RhView_Set((struct DotrViewBlock *)&args);
    return RhView_SendViewParameters(tag);
}
