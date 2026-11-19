#ifndef DOTR_EE_SORT_PRIMS_H
#define DOTR_EE_SORT_PRIMS_H

/* ExSortPrim_Pipe (0x207650), the register-list packet builder next to ExSortNode_Pipe.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/math-prims.md. */

/* Types and the assumed declaration of ExSortPrim_Pipe (integer header and chain addresses)
 * come from draw_state.h; the host test places both below 4 GiB. */
#include "dotr/ee/draw_state.h"

/* Types, assumed from their use: the packet header (count, flag word, packed four-bit
 * register numbers, then the list words); a chain of eight-byte links whose shift selects the
 * slot count (host: the same offsets as DotrDrawChain); the Z-buffer register word whose bit 32
 * is cleared and set around the list; the loop count of the tag at +0x10. */
struct DotrSortPrimSource {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
    unsigned long long data[1];
};
struct DotrSortPrimLink {
    unsigned int tag;
    void *next;
};
struct DotrSortPrimTable {
    struct DotrSortPrimLink *links;
    unsigned char padding[8];
    int shift;
};
struct DotrSortPrimRegister {
    unsigned long long lower : 32;
    unsigned long long enabled : 1;
    unsigned long long upper : 31;
};
struct DotrSortPrimTag {
    unsigned short loops : 15;
    unsigned short end : 1;
};

/* RhBase_GetZBufInfo (0x20a280, defined in draw_state). The register table gExPrim_aaRegs
 * (0x293d80) is declared inside the function as gExPrim_aaRegs_sortprim (0x70 bytes per context). */
void func_0020a280_sortprim(void *arg0);

#endif
