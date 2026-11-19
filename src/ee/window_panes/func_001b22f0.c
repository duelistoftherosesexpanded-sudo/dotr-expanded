#include "dotr/ee/window_panes.h"

#ifndef __MWERKS__
/* Host build only: the block-scope SzMsg_DrawRatioX_Pipe declaration takes the drawing
 * callback as void *, which ISO C does not allow for a function pointer. */
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#ifdef __MWERKS__
#pragma opt_propagation off
#endif
int lSzDuel_DrawWindow_CardInfo_Pipe(int arg0, void *arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, long arg8, long arg9)
{
    extern int lSzDuel_DrawWindow_Icon_Pipe();
    extern int lSzDuel_DrawWindow_Board_Pipe();
    extern int SzDuel_DrawWindow_Digit_Pipe();
    extern int SzData_GetRank();
    extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, int, void *);
    extern unsigned char data_00290020_panes[];
    extern unsigned char gSzDuel_Icon_panes[];
    extern unsigned char data_002901d4_panes[];
    extern unsigned char data_002901de_panes[];
    extern unsigned char data_002901e8_panes[];
    extern unsigned char data_002901f2_panes[];
    extern unsigned char data_002901fc_panes[];
    extern unsigned char data_00290206_panes[];
    extern unsigned char data_00290210_panes[];
    extern unsigned char data_0029021a_panes[];
    extern unsigned char data_00290224_panes[];
    extern unsigned char data_0029022e_panes[];
    extern unsigned char data_00290238_panes[];
    extern unsigned char data_00290242_panes[];
    extern unsigned char data_0029024c_panes[];
    extern unsigned char data_002909a4_panes[];
    extern unsigned char data_002909ae_panes[];
    extern unsigned char data_002909c2_panes[];
    extern unsigned char data_002909e0_panes[];
    extern unsigned char data_002909ea_panes[];
    unsigned char *globalBase;
    unsigned char *playerState;
    int x;
    int cardState;
    int cardType;
    int attribute;
    int cardIndex;
    int icon1;
    int icon3;
    int defenseBase;
    int effect;
    unsigned char *label;
    int enabled;
    int text2;
    int mode;
    float scale1;
    float scale2;
    float scale3;
    int y;
    int labelX;
    int labelY;
    int cur;
    int typeIcon;
    int owner;
    int hidden;
    int textX1;
    int textX2;
    int textY2;
    int textX3;
    int text3;
    int width1;
    int width2;
    int width3;
    int amount;
    int canUse;
    int count;
    int attackBase;
    int numX2;
    int attack;
    int defense;
    int iconY;
    int statX;
    int statY;
    int numX1;
    int numY2;
    long long color;
    DotrPaneInfoCard *card;
    int iconX;

    globalBase = data_00317490_panes.b;
    playerState = data_00317490_panes.b + arg4 * 0x350;
    x = arg5 + 3;
    y = arg6 + 4;
    labelX = x + 0x6C0;
    mode = arg7;
    labelY = y + 0x720;
    card = arg1;
    cur = lSzDuel_DrawWindow_Frame_Pipe(arg0, arg4, (int)arg9 != 0 ? 7 : -1, (int)arg8, arg5, arg6, 0x33, 10, 1);
    if (card != 0 && card->active != 0) {
        owner = card->owner;
        cardState = card->state;
        cardType = (int)card->type >> 5;
        typeIcon = card->type & 0x1F;
        attribute = SzDuel_Kind2Attr(cardType, card->attribute);
        if (cardType == 0) {
            label = gSzDuel_Icon_panes + typeIcon * 10;
        } else {
            label = gSzDuel_Icon_panes + (cardType + 0x14) * 10;
        }
        cardIndex = SzData_GetRank(arg4, card->id);
        icon1 = SzDuel_GetMsgName(card->id);
        if (cardType == 0) {
            typeIcon += 0x25;
        } else {
            typeIcon = cardType + 0x1E;
        }
        hidden = 0;
        icon3 = cardIndex + 0x52;
        if (SzGame_GetMenuNum(0x1E) != 0 && cardState > 0) {
            hidden = lSzDuel_IsUnitHide(card);
        }
        if (hidden != 0) {
            icon1 = 0x1C;
        }
        arg0 = SzMsg_LoadFont_Pipe(cur, 1, icon1, 0, 0x16);
        if (hidden != 0) {
            typeIcon = 0x1D;
        }
        arg0 = SzMsg_LoadFont_Pipe(arg0, 2, typeIcon, 0, 0x2C);
        if (hidden != 0) {
            icon3 = 0x51;
        }
        arg0 = SzMsg_LoadFont_Pipe(arg0, 3, icon3, 0, 0x42);
        arg0 = RhText_SetTextureFlush(arg0);
        textX1 = x + 2;
        typeIcon = lSzDuel_DrawWindow_Board_Pipe(arg0, arg4, textX1, y, 0x1B, 1, -1);
        width1 = *(int *)((unsigned char *)SzMsg_GetInfo(1) + 0xC);
        scale1 = (width1 < 229) ^ 1 ? 228.0f / (float)width1 : 1.0f;
        arg0 = SzMsg_DrawRatioX_Pipe(typeIcon, 1, textX1 + 5, y + 2, scale1, 1.0f, 0, SzMsg_DrawRect_Pipe);
        arg0 = lSzDuel_DrawWindow_Icon_Pipe(arg0, label, labelX + 3, labelY + 0x1F, hidden, enabled = !hidden, hidden, 0, 1, 0, -1);
        textY2 = y + 0x1D;
        textX2 = x + 0x1D;
        text2 = lSzDuel_DrawWindow_Board_Pipe(arg0, arg4, textX2, textY2, 0xB, 1, -1);
        width2 = *(int *)((unsigned char *)SzMsg_GetInfo(2) + 0xC);
        scale2 = width2 > 100 ? 100.0f / (float)width2 : 1.0f;
        arg0 = SzMsg_DrawRatioX_Pipe(text2, 2, textX2 + 5, textY2 + 2, scale2, 1.0f, 0, SzMsg_DrawRect_Pipe);
        iconX = labelX + 0x90;
        iconY = labelY + 0x1F;
        if (mode == 2) {
            color = 1;
        } else {
            color = mode == 0 ? 2LL : -1LL;
        }
        cur = lSzDuel_DrawWindow_Icon_Pipe(arg0, data_00290020_panes + attribute * 10, iconX, iconY, 1, enabled, 1, 0, 1, 0, color);
        if (cardType == 0) {
            if (cardState == 0) {
                arg0 = lSzDuel_DrawWindow_Emblem_Pipe(cur, x + 0xF1, y + 2, cardIndex, 1, 0);
                textX3 = x + 0x10C;
                text3 = lSzDuel_DrawWindow_Board_Pipe(arg0, arg4, textX3, y, 9, 1, -1);
                width3 = *(int *)((unsigned char *)SzMsg_GetInfo(3) + 0xC);
                scale3 = width3 > 88 ? 88.0f / (float)width3 : 1.0f;
                cur = SzMsg_DrawRatioX_Pipe(text3, 3, textX3 + 3, y + 2, scale3, 1.0f, 0, SzMsg_DrawRect_Pipe);
                if (globalBase[0xD3C] == owner) {
                    amount = SzDuel_GetDeckCount(owner);
                    count = SzDuel_GetHandCount(owner);
                    if (((DotrPaneInfoFlags *)(playerState + 0x346))->bit0 != 0 && amount + count > 0) {
                        canUse = 0;
                        if ((amount > 0 && playerState[0x344] > 0) != 0) {
                            if (!(playerState[0x344] & 0x80)) {
                                canUse = 1;
                            }
                        }
                        iconX = labelX + 0x16E;
                        iconY = labelY + 2;
                        cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002909b8_panes, iconX, iconY, 0, 1, 0, 0, 1, 0, (canUse != 0 ? 1LL : -1LL));
                    }
                }
                statX = labelX + 0xF1;
                statY = labelY + 0x1F;
                if (SzDuel_GetUnitLeaderAbility2(card, 4, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002901d4_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 5, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002901de_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 6, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002901e8_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 7, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002901f2_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 8, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002901fc_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 9, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_00290206_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 10, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_00290210_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 11, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_0029021a_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 12, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_00290224_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 13, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_0029022e_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 14, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_00290238_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 15, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_00290242_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                    statX += 0x19;
                }
                if (SzDuel_GetUnitLeaderAbility2(card, 16, 0) != 0) {
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_0029024c_panes, statX, statY, 0, 1, 0, 0, 1, 0, -1);
                }
            } else if (hidden == 0) {
                attackBase = Ta_GetParamOfs((DotrPaneWord)card, arg2, arg3, 1);
                defenseBase = Ta_GetParamOfs((DotrPaneWord)card, arg2, arg3, 0);
                attack = attackBase + SzDuel_GetUnitAP2(card, 0);
                defense = defenseBase + SzDuel_GetUnitDP2(card, 0);
                effect = CheckLandAgreement(SzDuel_GetField(arg2, arg3), card);
                attack = DOTR_PANE_INFO_MIN(DOTR_PANE_INFO_MAX(attack, 0), 9999);
                defense = DOTR_PANE_INFO_MIN(DOTR_PANE_INFO_MAX(defense, 0), 9999);
                if (SzDuel_GetUnitConstEffect2(card, 0) != 0) {
                    iconX = labelX + 0xAD;
                    iconY = labelY + 0x1C;
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002909ea_panes, iconX, iconY, 0, 1, 0, 0, 1, 0, -1);
                }
                iconX = labelX + 0xF2;
                iconY = labelY + 2;
                arg0 = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002909a4_panes, iconX, iconY, 0, 1, 0, 0, 1, 0, -1);
                numX1 = x + 0x10C;
                arg0 = lSzDuel_DrawWindow_Board_Pipe(arg0, arg4, numX1, y, 9, 1, -1);
                arg0 = SzDuel_DrawWindow_Digit_Pipe(arg0, numX1 + 6, y + 2, 4, attack, 0);
                iconX = labelX + 0xF2;
                iconY = labelY + 0x1F;
                arg0 = lSzDuel_DrawWindow_Icon_Pipe(arg0, data_002909ae_panes, iconX, iconY, 0, 1, 0, 0, 1, 0, -1);
                numY2 = y + 0x1D;
                numX2 = x + 0x10C;
                arg0 = lSzDuel_DrawWindow_Board_Pipe(arg0, arg4, numX2, numY2, 9, 1, -1);
                cur = SzDuel_DrawWindow_Digit_Pipe(arg0, numX2 + 6, numY2 + 2, 4, defense, 0);
                if (effect != 0) {
                    iconX = labelX + 0x16E;
                    iconY = labelY + 2;
                    cur = lSzDuel_DrawWindow_Icon_Pipe(cur, gSzDuel_Icon_panes + (effect + 0xED) * 10, iconX, iconY, 0, 1, 0, 0, 1, 0, -1);
                }
            }
            if (hidden == 0 && (GetMoveRev((DotrPaneWord)card, arg2, arg3) & 0xF) > 1U) {
                iconX = labelX + 0x18B;
                iconY = labelY + 2;
                cur = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002909e0_panes, iconX, iconY, 0, 1, 0, 0, 1, 0, -1);
            }
        }
        SzMsg_Free(1);
        SzMsg_Free(2);
        SzMsg_Free(3);
        if ((cardType != 0 || cardState != 0) && card->flags.bit2 == 0) {
            amount = card->count1;
            if (amount == 0xFF) {
                count = 0x7FFFFFFF;
            } else {
                canUse = card->count2;
                if (canUse == 0xFF) {
                    count = 0x7FFFFFFF;
                } else {
                    count = (unsigned char)(canUse < amount ? amount : canUse);
                }
            }
            if (count != 0) {
                arg0 = lSzDuel_DrawWindow_Icon_Pipe(cur, data_002909c2_panes, labelX + 0x16E, labelY + 0x1F, 0, 1, 0, 0, 1, 0, -1);
                y += 0x1D;
                labelX = x + 0x188;
                arg0 = lSzDuel_DrawWindow_Board_Pipe(arg0, arg4, labelX, y, 1, 1, -1);
                cur = SzDuel_DrawWindow_Digit_Pipe(arg0, labelX + 4, y + 2, 1, count, 0);
            }
        }
    }
    return cur;
}
#ifdef __MWERKS__
#pragma opt_propagation reset
#endif
