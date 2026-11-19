#include "dotr/ee/model_texanim.h"

DotrTexAnimWord SzModel_LoadTexAnim_Pipe(DotrTexAnimWord cursor, int index, int position, struct DotrTexAnimRecord *record)
{
    struct DotrTexAnimEntry *e;
    DotrTexAnimLong *p = (DotrTexAnimLong *)cursor;
    DotrTexAnimLong *start;
    struct DotrTexAnimTexture *tex;
    int n;
    int step;
    int last;
    int cells;
    int anim;
    unsigned int frame;
    int cell;

    tex = &record->head->tex;
    e = record->entries;
    n = tex->width >> 6;
    start = p;
    n = (n > 1) ? n : 1;
    step = SzModel_GetPauseFlag(index) ? 0 : 1;
    for (last = 0; !last; e++) {
        cells = e->cols * e->rows;
        anim = SzModel_GetAnim(index);
        frame = func_001a23b0(SzModel_GetAnimFrame(index));
        cell = 0;
        if (e->count) {
            frame = e->counter;
            e->counter = frame + step;
        }
        if (e->last) {
            last = 1;
        }
        if (cells == 0) {
            continue;
        }
        if (!(e->mask & (1 << anim))) {
            continue;
        }
        if (e->type == 0 || e->type == 2) {
            cells++;
        }
        switch (e->type) {
        case 0:
        case 1:
        {
            unsigned short span = e->u4.h.a;
            cell = e->u3.cell;
            if (step && frame && frame % span == 0) {
                cell = (cell + 1) % cells;
                e->u3.cell = cell;
            }
            break;
        }
        case 2:
        case 3: {
            unsigned short span;
            int dir = e->u3.swing.dir;
            if (dir > 0) {
                span = e->u4.h.a;
            } else {
                span = e->u4.h.b;
            }
            cell = e->u3.swing.pos;
            if (step && frame && frame % span == 0) {
                cell += dir;
                cell = ((cell > 0 ? cell : 0) < cells - 1) ? (cell > 0 ? cell : 0) : cells - 1;
                e->u3.swing.pos = cell;
                if (cell <= 0 || cell >= cells - 1) {
                    e->u3.swing.dir = -e->u3.swing.dir;
                    e->counter = 1;
                }
            }
            break;
        }
        case 6:
            if (e->u3.gate.on) {
                e->u4.b.time += step;
                if (e->u4.b.time < e->u4.b.limit) {
                    break;
                }
                e->u3.gate.on = 0;
                continue;
            }
            if (frame % e->u4.h.a) {
                continue;
            }
            if (step && e->u4.b.limit) {
                e->u3.gate.on = 1;
                e->u4.b.time = 0;
            }
            break;
        case 5:
            if (e->u3.gate.on) {
                if (frame == e->u4.h.a) {
                    break;
                }
                e->u4.b.time += step;
                if (e->u4.b.time < e->u4.b.limit) {
                    break;
                }
                e->u3.gate.on = 0;
                continue;
            }
            if (frame != e->u4.h.a) {
                continue;
            }
            if (step && e->u4.b.limit) {
                e->u3.gate.on = 1;
                e->u4.b.time = 0;
            }
            break;
        case 4:
            if (frame < e->u4.h.a) {
                continue;
            }
            if (frame > e->u4.h.b) {
                continue;
            }
            break;
        }
        if (e->type == 0 || e->type == 2) {
            if (cell == 0) {
                continue;
            }
            cell--;
        }
        if (e->sx == e->dx && e->sy == e->dy) {
            if (cell == 0) {
                continue;
            }
        }
        if (e->mode == 2 || e->mode == 3) {
            cell = -cell;
        }
        p[1] = 0;
        p[0] = 0;
        ((unsigned char *)p)[3] = 0x10;
        p[2] = 0x1000000000008004;
        p[3] = 0xe;
        p[4] = (DotrTexAnimLong)position | ((DotrTexAnimLong)n << 16) | ((DotrTexAnimLong)tex->psm << 24) | ((DotrTexAnimLong)position << 32) |
               ((DotrTexAnimLong)n << 48) | ((DotrTexAnimLong)tex->psm << 56);
        p[5] = 0x50;
        p[6] = (DotrTexAnimLong)(e->sx + e->w * (cell % e->cols)) | ((DotrTexAnimLong)(e->sy + e->h * (cell / e->cols)) << 16) |
               ((DotrTexAnimLong)e->dx << 32) | ((DotrTexAnimLong)e->dy << 48);
        p[7] = 0x51;
        p[8] = (DotrTexAnimLong)e->w | ((DotrTexAnimLong)e->h << 32);
        p[9] = 0x52;
        p[10] = 2;
        p[11] = 0x53;
        p += 12;
    }
    if (p != start) {
        unsigned short q = ((p - start) >> 1) - 1;
        *(unsigned short *)start = q;
        ((int *)start)[2] = 0x11000000;
        ((int *)start)[3] = q | 0x51000000;
    }
    return (DotrTexAnimWord)p;
}
