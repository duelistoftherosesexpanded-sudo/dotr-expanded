#include "dotr/ee/sort_prims.h"

/* 0x207650. Addresses kept in ints are DotrDrawWord (`int` on the target). */

DotrDrawWord ExSortPrim_Pipe(DotrDrawWord arg0, int arg1, int arg2, int arg3) {
    extern unsigned char gExPrim_aaRegs_sortprim[];
    unsigned long long *cursor;
    unsigned long long *start;
    unsigned long long registers;
    int flags;
    int context;
    unsigned long long *data;
    int replace;
    unsigned short count;
    int texture;
    int alpha;
    int alphaMask;
    int contextMask;
    unsigned int primitive;
    int restore;
    int i;
    unsigned long long reg;
    unsigned short length;
    unsigned long long prim64;
    struct DotrSortPrimRegister saved;

    cursor = (unsigned long long *)arg0;
    start = cursor;
    registers = ((struct DotrSortPrimSource *)(DotrDrawWord)arg1)->registers;
    count = ((struct DotrSortPrimSource *)(DotrDrawWord)arg1)->count;
    data = ((struct DotrSortPrimSource *)(DotrDrawWord)arg1)->data;
    flags = ((struct DotrSortPrimSource *)(DotrDrawWord)arg1)->flags;
    alphaMask = flags & 0x200;
    contextMask = flags & 0x1000;
    if (alphaMask != 0) {
        alpha = 1;
    } else {
        alpha = 0;
    }
    if (contextMask != 0) {
        context = 1;
    } else {
        context = 0;
    }
    primitive = flags & 0xF;
    if (primitive == 7) {
        return arg0;
    }
    cursor[1] = 0;
    cursor[0] = 0;
    *((unsigned char *)cursor + 3) = 0x10;
    if (arg2 != 0) {
        struct DotrSortPrimTable *table = (struct DotrSortPrimTable *)(DotrDrawWord)arg2;
        int limit = 1 << table->shift;
        if (flags & 0x8000) {
            arg3 = limit - 1;
        } else if ((unsigned int)(limit - 2) < (unsigned int)arg3) {
            arg3 = limit - 2;
        }
        *(void **)((unsigned char *)cursor + 4) =
            *(void **)((DotrDrawWord)table->links + arg3 * 8 + 4);
        *(void **)((DotrDrawWord)table->links + arg3 * 8 + 4) = cursor;
    }
    prim64 = primitive;
    cursor[2] = 0x1000000000008000ULL;
    cursor[3] = 0xE;
    texture = flags & 0x20;
    replace = flags & 0x800;
    cursor[4] = ((long long)((flags & 0x2000) ? 1 : 0) << 10) |
        (((long long)(contextMask ? 1 : 0) << 9) |
        (((long long)(replace ? 1 : 0) << 8) |
        (((long long)((flags & 0x400) ? 1 : 0) << 7) |
        (((long long)(alphaMask ? 1 : 0) << 6) |
        (((long long)((flags & 0x100) ? 1 : 0) << 5) |
        (prim64 |
        ((long long)((flags & 0x10) ? 1 : 0) * 8) |
        ((long long)(texture ? 1 : 0) * 16)))))));
    cursor[5] = 0;
    cursor[6] = (long long)(alpha ? 1 : 0) | 0x5000C;
    cursor[7] = (long long)(context + 0x47);
    cursor += 8;
    if (alphaMask != 0) {
        cursor[0] = data[(unsigned short)count];
        cursor[1] = (long long)(context + 0x42);
        cursor += 2;
    }
    if (texture != 0) {
        unsigned long long *extra = &data[(unsigned short)count];
        cursor[0] = extra[1];
        cursor[1] = (long long)(context + 6);
        cursor[2] = extra[2];
        cursor[3] = (long long)(context + 0x14);
        cursor += 4;
        if (flags & 0x4000) {
            cursor[0] = extra[3];
            cursor[1] = (long long)(context + 8);
            cursor += 2;
        }
    }
    restore = flags & 0x8000;
    if (restore != 0) {
        func_0020a280_sortprim(&saved);
        saved.enabled = 0;
        cursor[0] = *(unsigned long long *)&saved;
        cursor[1] = (long long)(context + 0x4E);
        cursor += 2;
    }
    for (i = 0; i < (unsigned short)count; i++) {
        reg = registers & 0xF;
        if (replace != 0 && reg == 2) {
            reg = 3;
        }
        registers >>= 4;
        cursor[0] = *data;
        cursor[1] = reg;
        cursor += 2;
        data++;
    }
    if (texture != 0 && (flags & 0x4000)) {
        unsigned long long *extra = (unsigned long long *)(gExPrim_aaRegs_sortprim + context * 0x70);
        cursor[0] = extra[8];
        cursor[1] = extra[9];
        cursor += 2;
    }
    if (restore != 0) {
        saved.enabled = 1;
        cursor[0] = *(unsigned long long *)&saved;
        cursor[1] = (long long)(context + 0x4E);
        cursor += 2;
    }
    length = ((cursor - start) >> 1) - 1;
    *(unsigned short *)start = length;
    *(unsigned int *)((unsigned char *)start + 8) = 0x11000000;
    *(unsigned int *)((unsigned char *)start + 0xC) = length | 0x51000000;
    ((struct DotrSortPrimTag *)((unsigned char *)start + 0x10))->loops = length - 1;
    return (DotrDrawWord)cursor;
}
