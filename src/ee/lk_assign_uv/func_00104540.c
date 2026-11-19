#include "dotr/ee/lk_assign_uv.h"

/* LkAssignUV (0x104540): writes a texture rectangle to a sprite packet's four uv pairs. */
int LkAssignUV(void *arg0, void *arg1, int arg2) {
    struct DotrLkAssignSprite *s = arg0;
    struct DotrLkAssignRect *r = arg1;

    s->flags |= 0x800;
    switch (arg2) {
    case 0:
        s->v[0].x.val = r->x << 4;
        s->v[0].y.val = r->y << 4;
        s->v[1].x.val = (r->x + r->w) << 4;
        s->v[1].y.val = r->y << 4;
        s->v[2].x.val = r->x << 4;
        s->v[2].y.val = (r->y + r->h) << 4;
        s->v[3].x.val = (r->x + r->w) << 4;
        s->v[3].y.val = (r->y + r->h) << 4;
        break;
    case 1:
        s->v[0].x.val = r->x << 4;
        s->v[0].y.val = r->y << 4;
        s->v[1].x.val = r->x << 4;
        s->v[1].y.val = (r->y + r->h) << 4;
        s->v[2].x.val = (r->x + r->w) << 4;
        s->v[2].y.val = r->y << 4;
        s->v[3].x.val = (r->x + r->w) << 4;
        s->v[3].y.val = (r->y + r->h) << 4;
        break;
    case 2:
        s->v[0].x.val = r->x << 4;
        s->v[0].y.val = (r->y + r->h) << 4;
        s->v[1].x.val = r->x << 4;
        s->v[1].y.val = r->y << 4;
        s->v[2].x.val = (r->x + r->w) << 4;
        s->v[2].y.val = (r->y + r->h) << 4;
        s->v[3].x.val = (r->x + r->w) << 4;
        s->v[3].y.val = r->y << 4;
        break;
    case 3:
        s->v[0].x.val = r->x << 4;
        s->v[0].y.val = (r->y + r->h) << 4;
        s->v[1].x.val = (r->x + r->w) << 4;
        s->v[1].y.val = (r->y + r->h) << 4;
        s->v[2].x.val = r->x << 4;
        s->v[2].y.val = r->y << 4;
        s->v[3].x.val = (r->x + r->w) << 4;
        s->v[3].y.val = r->y << 4;
        break;
    case 4:
        s->v[0].x.val = (r->x + r->w) << 4;
        s->v[0].y.val = r->y << 4;
        s->v[1].x.val = r->x << 4;
        s->v[1].y.val = r->y << 4;
        s->v[2].x.val = (r->x + r->w) << 4;
        s->v[2].y.val = (r->y + r->h) << 4;
        s->v[3].x.val = r->x << 4;
        s->v[3].y.val = (r->y + r->h) << 4;
        break;
    case 5:
        s->v[0].x.val = (r->x + r->w) << 4;
        s->v[0].y.val = r->y << 4;
        s->v[1].x.val = (r->x + r->w) << 4;
        s->v[1].y.val = (r->y + r->h) << 4;
        s->v[2].x.val = r->x << 4;
        s->v[2].y.val = r->y << 4;
        s->v[3].x.val = r->x << 4;
        s->v[3].y.val = (r->y + r->h) << 4;
        break;
    case 6:
        s->v[0].x.val = (r->x + r->w) << 4;
        s->v[0].y.val = (r->y + r->h) << 4;
        s->v[1].x.val = r->x << 4;
        s->v[1].y.val = (r->y + r->h) << 4;
        s->v[2].x.val = (r->x + r->w) << 4;
        s->v[2].y.val = r->y << 4;
        s->v[3].x.val = r->x << 4;
        s->v[3].y.val = r->y << 4;
        break;
    case 7:
        s->v[0].x.val = (r->x + r->w) << 4;
        s->v[0].y.val = (r->y + r->h) << 4;
        s->v[1].x.val = (r->x + r->w) << 4;
        s->v[1].y.val = r->y << 4;
        s->v[2].x.val = r->x << 4;
        s->v[2].y.val = (r->y + r->h) << 4;
        s->v[3].x.val = r->x << 4;
        s->v[3].y.val = r->y << 4;
        break;
    }
    return 0;
}
