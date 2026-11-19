#include "dotr/ee/ex_texture.h"

int ExAssignTexture(struct DotrExTexHdr *h, struct DotrExTexTex *tex, int a)
{
    unsigned short n = h->count;
    union DotrExTexWord *base = h->w;
    union DotrExTexWord *w;
    if (!(h->flags & 0x20)) {
        return 0;
    }
    if (tex == 0) {
        return 0;
    }
    w = &base[n];
    w[1].l = tex->w48;
    w[2].l = tex->w64;
    w[2].b.b5 = w[2].h.f6 = a;
    return 1;
}
