#include "dotr/ee/texanim_conv.h"

/* 0x1e9340. Its switch table is at 0x2cf930 (28 bytes). */
#ifndef __MWERKS__
/* Host build only: this C passes addresses as 32-bit words, as the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

unsigned char *lSzModel_TexAnimConv(DotrTexAnimWord arg0, unsigned char *arg1)
{
    char buf[0x400];
    char name[0x80];
    char flags[0x80];
    char place[0x80];
    char size[0x80];
    int x8;
    int xA;
    int xC;
    int xE;
    int x10;
    int x12;
    int a4;
    int b4;
    int b6;
    int c4;
    int c6;
    int d4;
    int d6;
    int e4;
    int e6;
    int sizeX;
    int sizeY;
    int count;
    int i;
    int kind;
    char **names;
    char *c;
    char *line;
    struct DotrTexAnimEntry *e;
    char *p;
    struct DotrTexAnimEntry *entries;
    char *tok;
    struct DotrTexAnimHeader *hdr;

    line = buf;
    hdr = (struct DotrTexAnimHeader *)(arg0 + 0x10);
    p = line;
    count = 0;
    tok = func_0019d430((char *)arg1, data_002cfa72);
    while (tok != NULL) {
        if (*(unsigned char *)tok != '#') {
            func_0019bdd8(p, tok);
            p += func_0019bef0(tok) + 1;
            count++;
        }
        tok = func_0019d430(NULL, data_002cfa72);
    }
    entries = count > 0 ? (struct DotrTexAnimEntry *)SzMem_Alloc(count * 24) : NULL;
    if (entries == NULL) {
        e = (struct DotrTexAnimEntry *)arg1;
        e->last = 1;
        e->lo = 0;
        e->hi = 0;
        return 0;
    }
    e = entries;
    for (i = 0; i < count; i++) {
        e->last = 1;
        e->lo = 0;
        e->hi = 0;
        kind = 0;
        names = apchMode;
        do {
            if (func_0019c028(line, *names, func_0019bef0(*names)) == 0) {
                break;
            }
            kind++;
            names++;
        } while (kind < 7);
        switch (kind) {
        case 0:
        case 1:
            if (func_0019b6e0(line, data_002cfa75, name, flags, place, size, &a4, &x8, &xA, &xC, &xE, &x10, &x12) < 11) {
                func_0017da98(data_002cfaa2, name);
                goto next;
            }
            e->b3.raw = 0;
            e->v4 = DOTR_TEXANIM_CLAMP(a4, 1, 0xFFFF);
            break;
        case 2:
        case 3:
            if (func_0019b6e0(line, data_002cfac2, name, flags, place, size, &b4, &b6, &x8, &xA, &xC, &xE, &x10, &x12) < 12) {
                func_0017da98(data_002cfaa2, name);
                goto next;
            }
            e->b3.s.a = 1;
            e->b3.s.b = 0;
            e->v4 = DOTR_TEXANIM_CLAMP(b4, 1, 0xFFFF);
            e->v6.w = DOTR_TEXANIM_CLAMP(b6, 1, 0xFFFF);
            break;
        case 6:
            if (func_0019b6e0(line, data_002cfaf2, name, flags, &c4, &c6, &x8, &xA, &xC, &xE, &x10, &x12) < 10) {
                func_0017da98(data_002cfaa2, name);
                goto next;
            }
            func_0019bdd8(place, data_002cfb16);
            e->b3.u.c = 0;
            e->v4 = DOTR_TEXANIM_CLAMP(c4, 1, 0xFFFF);
            e->v6.b.lo = DOTR_TEXANIM_CLAMP(c6, 0, 0xFF);
            e->v6.b.hi = 0;
            break;
        case 5:
            if (func_0019b6e0(line, data_002cfaf2, name, flags, &d4, &d6, &x8, &xA, &xC, &xE, &x10, &x12) < 10) {
                func_0017da98(data_002cfaa2, name);
                goto next;
            }
            func_0019bdd8(place, data_002cfb16);
            e->b3.u.c = 0;
            e->v4 = DOTR_TEXANIM_CLAMP(d4, 1, 0xFFFF);
            e->v6.b.lo = DOTR_TEXANIM_CLAMP(d6, 0, 0xFF);
            e->v6.b.hi = 0;
            break;
        case 4:
            if (func_0019b6e0(line, data_002cfaf2, name, flags, &e4, &e6, &x8, &xA, &xC, &xE, &x10, &x12) < 10) {
                func_0017da98(data_002cfaa2, name);
                goto next;
            }
            func_0019bdd8(place, data_002cfb16);
            e->v4 = DOTR_TEXANIM_CLAMP(e4, 1, 0xFFFF);
            e->v6.w = DOTR_TEXANIM_CLAMP(e6, 1, 0xFFFF);
            break;
        default:
            func_0017da98(data_002cfb18);
            goto next;
        }
        e->kind = kind;
        e->align = 0;
        if (func_0019b8f8(place, data_002cfb16) == 0) {
            e->align = 0;
        } else if (func_0019b8f8(place, data_002cfb32) == 0) {
            e->align = 1;
        } else if (func_0019b8f8(place, data_002cfb34) == 0) {
            e->align = 2;
        } else if (func_0019b8f8(place, data_002cfb34 + 3) == 0) {
            e->align = 3;
        } else if (func_0019b8f8(place, data_002cfb3a) == 0) {
            e->align = 0;
        } else if (func_0019b8f8(place, data_002cfb3c) == 0) {
            e->align = 1;
        } else if (func_0019b8f8(place, data_002cfb3c + 2) == 0) {
            e->align = 2;
        } else if (func_0019b8f8(place, data_002cfb40) == 0) {
            e->align = 3;
        } else if (func_0019b8f8(place, data_002cfb40 + 2) == 0) {
            e->align = 4;
        }
        e->lo = 1;
        e->hi = 1;
        switch (e->align) {
        case 0:
        case 2:
            e->lo = DOTR_TEXANIM_CLAMP(func_001950a0(size), 1, 0xF);
            break;
        case 1:
        case 3:
            e->hi = DOTR_TEXANIM_CLAMP(func_001950a0(size), 1, 0xF);
            break;
        case 4:
            sizeX = 1;
            sizeY = 1;
            func_0019b6e0(size, data_002cfb45, &sizeX, &sizeY);
            e->lo = DOTR_TEXANIM_CLAMP(sizeX, 1, 0xF);
            e->hi = DOTR_TEXANIM_CLAMP(sizeY, 1, 0xF);
            break;
        }
        e->mask = 0;
        e->plain = 1;
        if (func_0019bc90(flags, data_002cfb4b) == 0) {
            e->mask = 0xFF;
            e->plain = 0;
        } else if (func_0019bc90(flags, data_002cfb4d) == 0) {
            e->mask = 0xFF;
        } else {
            kind = 0;
            if (kind < (unsigned int)func_0019bef0(flags)) {
                c = flags;
                do {
                    if (*c >= 'A' && *c < 'I') {
                        e->mask |= 1 << (*(unsigned char *)c - 'A');
                        e->plain = 0;
                    } else if (*c >= 'a' && *c < 'i') {
                        e->mask |= 1 << (*(unsigned char *)c - 'a');
                    }
                    c++;
                    kind++;
                } while (kind < (unsigned int)func_0019bef0(flags));
            }
        }
        e->last = (i + 1 >= count) ? 1 : 0;
        e->x8 = DOTR_TEXANIM_CLAMP(x8, 1, hdr->w);
        e->xA = DOTR_TEXANIM_CLAMP(xA, 1, hdr->h);
        e->xC = DOTR_TEXANIM_CLAMP(xC, 0, hdr->w - 1);
        e->xE = DOTR_TEXANIM_CLAMP(xE, 0, hdr->h - 1);
        e->x10 = DOTR_TEXANIM_CLAMP(x10, 0, hdr->w - 1);
        e->x12 = DOTR_TEXANIM_CLAMP(x12, 0, hdr->h - 1);
        e->x14 = 0;
        e++;
    next:
        line += func_0019bef0(line) + 1;
    }
    return (unsigned char *)entries;
}
