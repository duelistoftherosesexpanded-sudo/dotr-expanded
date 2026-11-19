#ifndef DOTR_EE_POSITION_ORDER_H
#define DOTR_EE_POSITION_ORDER_H

/* Shared by position_rules.h and kind_gates.h, which cannot be included
 * together (their callee views differ). */
struct DotrPositionOrder {
    unsigned short position; /* +0x00 */
    unsigned short unknown_02;
    unsigned short kind;     /* +0x04 */
    unsigned short unknown_06;
    union {
        unsigned short half; /* +0x08, read as a halfword */
        unsigned int word;   /* +0x08, written as a word */
    } flags;
};

#endif
