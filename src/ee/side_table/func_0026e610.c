#include "dotr/ee/state_queries.h"

int Ta_IsHandHeap(int side, int pos, int other_side, int other_pos, int *out)
{
    int cardAID = SzDuel_GetUnitCardNo(side, pos);
    int cardBID = SzDuel_GetUnitCardNo(other_side, other_pos);
    unsigned char *cardAConstInfo = (unsigned char *)SzDuel_GetCardConstInfo(cardAID);
    unsigned char *cardBConstInfo = (unsigned char *)SzDuel_GetCardConstInfo(cardBID);
    int kind;
    int r;

    if (cardAConstInfo == 0 || cardBConstInfo == 0) {
        return -1;
    }
    kind = *cardAConstInfo >> 5;
    if (kind == 0 && *cardBConstInfo >> 5 == 0) {
        r = Fusion(cardAID, cardBID);
        if (r == 999) {
            out[0] = 2;
            out[1] = side << 8 | pos;
            out[2] = side << 8 | other_pos;
            out[3] = 0;
            return -1;
        } else {
            out[0] = 0;
            out[1] = side << 8 | pos;
            out[2] = side << 8 | other_pos;
            out[3] = r;
            return 0;
        }
    } else if (kind == 0) {
        switch (*cardBConstInfo >> 5) {
        case 2:
            r = Ta_GetHandEnchant2(side, pos, cardBID);
            if (r < 0) {
                out[0] = 2;
                out[1] = side << 8 | pos;
                out[2] = side << 8 | other_pos;
                out[3] = 0;
                return -1;
            } else if (r >> 16 == 0) {
                out[0] = 0;
                out[1] = side << 8 | pos;
                out[2] = side << 8 | other_pos;
                out[3] = r & 0xffff;
                return 0;
            } else {
                out[0] = 1;
                out[1] = (side << 8 | pos) << 16 | (side << 8 | other_pos);
                out[2] = SzDuel_GetTurnOwner();
                out[3] = cardBID;
                return 1;
            }
        default:
            out[0] = 2;
            out[1] = side << 8 | pos;
            out[2] = side << 8 | other_pos;
            out[3] = 0;
            return -1;
        }
    } else if (kind == 2) {
        switch (*cardBConstInfo >> 5) {
        case 0:
            r = Ta_GetHandEnchant2(other_side, other_pos, cardAID);
            if (r < 0) {
                out[0] = 2;
                out[1] = side << 8 | pos;
                out[2] = side << 8 | other_pos;
                out[3] = 0;
                return -1;
            } else if (r >> 16 == 0) {
                out[0] = 0;
                out[1] = side << 8 | pos;
                out[2] = side << 8 | other_pos;
                out[3] = r & 0xffff;
                return 0;
            } else {
                out[0] = 1;
                out[1] = (side << 8 | pos) << 16 | (side << 8 | other_pos);
                out[2] = SzDuel_GetTurnOwner();
                out[3] = cardAID;
                return 0x10001;
            }
        default:
            out[0] = 2;
            out[1] = side << 8 | pos;
            out[2] = side << 8 | other_pos;
            out[3] = 0;
            return -1;
        }
    } else {
        out[0] = 2;
        out[1] = side << 8 | pos;
        out[2] = side << 8 | other_pos;
        out[3] = 0;
        return -1;
    }
}
