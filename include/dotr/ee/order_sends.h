#ifndef DOTR_EE_ORDER_SENDS_H
#define DOTR_EE_ORDER_SENDS_H

/* Functions that fill the halfword at +4 and the word at +8 of the 12-byte order record
 * lower_rules callers pass.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Same layout as lower_rules.h's DotrRuleOrder (not included: it declares 0x27e210 with
 * its own order type). The word at +8 is read as a halfword and written as a word. Member names assumed. */
struct DotrSendOrder {
    unsigned short position;
    unsigned short summonOrder;
    unsigned short code;
    unsigned short unknown_06;
    union {
        unsigned short half;
        int word;
    } flags;
};

/* 0x27bbc0 and 0x27be10: argument counts are what 0x27e210 passes and int results are
 * assumed. 0x1de6d0 is defined in state_records. */
extern int TaLogic_IsAttackedNow(int row, int column, int c);
extern int TaLogic_IsAttackedNow2(int row, int column, int a, int b, int c);
extern int SzDuel_InqHandPhase(int side);

void TaLogic_CommandStdLDUnit(struct DotrSendOrder *order, int destinationCol, int destinationRow, int c);

#endif
