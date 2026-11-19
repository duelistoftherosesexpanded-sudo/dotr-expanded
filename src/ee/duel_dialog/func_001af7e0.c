#include "dotr/ee/duel_dialog.h"

/* SzDuel_DrawDialog_Pipe (0x1af7e0): draws the up to four duel dialogs whose kinds are the
 * bytes +0xcb2..+0xcb5 of the control block (0x18 = none; nothing is drawn when all four are
 * none). It opens a window session (depth 0xfffffe for windows and messages), then per kind
 * (jump table at 0x2be960): titled message windows (0..6, 15, 16, 19..22), the six-option
 * menu (7), the yes/no choice (8, 22), the card name and rank windows (9, 10), the card
 * window with its card drawn (11), the card name window (12, 13), the deck list with portraits,
 * counters and up to five card rows (14), the card detail (17), the hand row of up to five
 * cards (18) and the hand cursor (23). Positions come from the halfwords at +0xc9a/+0xc9c and
 * values from +0xcaa. Frees message slot 0 and returns the advanced cursor.
 * The callees are declared inside the function body; the int casts of pointers use the cursor
 * word type (int on the target) and NULL is written 0. */
int SzDuel_DrawDialog_Pipe(int arg0)
{
    extern int lSzDuel_DrawDialog_Window_Pipe();
    extern int lSzDuel_DrawWindow_Icon_Pipe();
    extern int lSzDuel_DrawWindow_Frame_Pipe();
    extern int lSzDuel_DrawWindow_Board_Pipe();
    extern int lSzDuel_DrawBuildDeck_Detail_Pipe();
    extern int SzMsg_LoadFont_Pipe();
    extern int SzMsg_DrawX_Pipe();
    extern int SzDuel_DrawWindow_Font_Pipe();
    extern int SzData_GetRank();
    extern int lSzDuel_OpenHandCursor_Pipe();
    extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, int, void (*)(void));
    extern void SzDuel_ClearDrawCard();
    int x_c21;
    int x_c20;
    int y_c21;
    int y_c20;
    char *text_c12;
    int x_c12;
    int value_c21;
    int x_c18;
    int width_c21;
    int value_c20;
    int i_c18;
    int width_c20;
    char *text_c8;
    char *text_c7;
    int side_c7;
    int width;
    char *text1_c8;
    DotrDialogCard card;
    int y_c7;
    char *text2_c8;
    DotrDialogDraw draw;
    DotrDialogDraw draw2;
    int slot_c7;
    DotrDialogCard card2;
    int mode2;
    unsigned char *entry;
    unsigned char *state = data_00317490_dialog.b;
    int mode4;
    char *text;
    int x_c7;
    int index;
    unsigned char *p2;
    int lowB;
    unsigned char type;
    int icon;
    int extra_c9;
    int packet;
    char *text_c14;
    unsigned short *list;
    float scale;
    int y;
    int start;
    int page;
    int end;
    int cardId;
    int count;
    int value;
    int row;
    int side;
    int cursor;
    int on;
    int color;
    int x;
    int lowA;
    int dir;
    int w;
    int msg;
    int id;
    unsigned char *pic;
    unsigned char *p4;
    char *text1;
    char *text2;
    int y_c22;
    int value_c22_c22;
    int width_c22;
    int i;
    int side_c9;
    int id_c9;
    int res;

    index = count = 0;
    for (; index < 4; index++) {
        if (state[index + 0xCB2] != 0x18) {
            count++;
        }
    }
    if (count == 0) {
        return arg0;
    }
    packet = SzDuel_StartWindowSessionWithPalette_Pipe(arg0, 0);
    SzDuel_SetWindowDefPrioZ(0xFFFFFE);
    SzMsg_SetDefPrioZ(0xFFFFFE);
    for (index = 0, p4 = state, p2 = state; index < 4; p4 += 4, index++, p2 += 2) {
        type = state[index + 0xCB2];
        if (type == 0x18) {
            continue;
        }
        switch (type) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            side = state[0xD54];
            text = data_00291130_dialog[type];
            y = 0x200 - ((*(short *)(p4 + 0xC9C) << 9) >> 6);
            cardId = *(unsigned short *)(p2 + 0xCAA);
            color = side | 0x8000;
            if (type == 2U || type == 3U || type == 4U || type == 5U) {
                cardId = -1;
            }
            if (type == 6) {
                cardId = -1;
                side = 2;
            }
            packet = lSzDuel_DrawDialog_Window_Pipe(packet, side, color, cardId, 0x140, y + 0x80, 0x15A, 0, func_0019bef0(text), (DotrDialogWord)text, 1);
            break;
        case 7:
            y_c7 = *(short *)(p4 + 0xC9C);
            x_c7 = y_c7 + 0xD9;
            text_c7 = data_00291130_dialog[type];
            slot_c7 = state[0xD35];
            side_c7 = state[0xD54];
            mode2 = state[0xD36] & 2;
            mode4 = state[0xD36] & 4;
            packet = lSzDuel_DrawDialog_Window_Pipe(packet, side_c7, -1, -1, 0x140, y_c7 + 0x94, 0x234, 0x98, func_0019bef0(text_c7), (DotrDialogWord)text_c7, 0);
            packet = SzMsg_LoadFont_Pipe(packet, 4, 0x73, 0, 0x58);
            packet = SzMsg_LoadFont_Pipe(packet, 5, 0x77, 0, 0x6E);
            packet = SzMsg_LoadFont_Pipe(packet, 6, 0x78, 0, 0x84);
            packet = SzMsg_LoadFont_Pipe(packet, 7, 0x74, 0, 0x9A);
            packet = SzMsg_LoadFont_Pipe(packet, 8, 0x7A, 0, 0xB0);
            packet = SzMsg_LoadFont_Pipe(packet, 9, 0x79, 0, 0xC6);
            packet = RhText_SetTextureFlush(packet);
            packet = SzMsg_DrawX_Pipe(packet, 4, 0x3E, x_c7, 0, SzMsg_DrawRect_Pipe);
            packet = SzMsg_DrawX_Pipe(packet, 5, 0x11A, x_c7, (DotrDialogWord)(mode2 != 0 ? 0 : data_002be944_dialog), SzMsg_DrawRect_Pipe);
            packet = SzMsg_DrawX_Pipe(packet, 6, 0x1BA, x_c7, (DotrDialogWord)(mode2 != 0 ? data_002be944_dialog : 0), SzMsg_DrawRect_Pipe);
            packet = SzMsg_DrawX_Pipe(packet, 7, 0x3E, x_c7 + 0x28, 0, SzMsg_DrawRect_Pipe);
            packet = SzMsg_DrawX_Pipe(packet, 8, 0x11A, x_c7 + 0x28, (DotrDialogWord)(mode4 != 0 ? 0 : data_002be944_dialog), SzMsg_DrawRect_Pipe);
            packet = SzMsg_DrawX_Pipe(packet, 9, 0x1BA, x_c7 + 0x28, (DotrDialogWord)(mode4 != 0 ? data_002be944_dialog : 0), SzMsg_DrawRect_Pipe);
            SzMsg_Free(1);
            SzMsg_Free(2);
            SzMsg_Free(3);
            SzMsg_Free(4);
            SzMsg_Free(5);
            SzMsg_Free(6);
            SzMsg_Free(7);
            SzMsg_Free(8);
            SzMsg_Free(9);
            packet = lSzDuel_DrawWindow_Frame_Pipe(packet, -1, -1, 7, 0x3E, x_c7 + slot_c7 * 0x28, -0x204, -0x16, 1);
            break;
        case 8:
            side = state[0xD54];
            text_c8 = data_00291130_dialog[type];
            text1_c8 = data_002bef6f_dialog;
            text2_c8 = data_002bef73_dialog;
            y = 0x200 - ((*(short *)(p4 + 0xC9C) << 9) >> 6);
            packet = lSzDuel_DrawDialog_Window_Pipe(packet, side, -1, 0x7B, 0x140, y + 0x9F, 0x15A, 0x82, func_0019bef0(text_c8), (DotrDialogWord)text_c8, 0);
            res = SzDuel_DrawWindow_Font_Pipe(packet, 0xEA, y + 0xFD, 2, 3, func_0019bef0(text1_c8), text1_c8, state[0xD35] != 0 ? 0 : (DotrDialogWord)data_002be944_dialog);
            packet = SzDuel_DrawWindow_Font_Pipe(res, 0x196, y + 0xFD, 2, 3, func_0019bef0(text2_c8), text2_c8, state[0xD35] != 0 ? (DotrDialogWord)data_002be944_dialog : 0);
            break;
        case 9:
        case 10:
            value = *(unsigned short *)(p2 + 0xCAA);
            text = data_00291130_dialog[type + ((value & 0x4000) ? 1 : 0)];
            y = 0x200 - ((*(short *)(p4 + 0xC9C) << 9) >> 6);
            id_c9 = value & 0x3FFF;
            if (value & 0x8000) {
                side_c9 = 1;
            } else {
                side_c9 = 0;
            }
            msg = (value & 0x4000) ? 0x60 : 0x5F;
            icon = SzDuel_GetMsgName(id_c9);
            extra_c9 = SzData_GetRank(side_c9, id_c9);
            packet = lSzDuel_DrawDialog_Window_Pipe(packet, side_c9, id_c9, icon, 0x140, y + 0x4C, -0x1CC, 0x128, func_0019bef0(text), (DotrDialogWord)text, 1);
            packet = SzMsg_LoadFont_Pipe(packet, 1, extra_c9 + 0x52, 0, 0x16);
            packet = SzMsg_LoadFont_Pipe(packet, 2, msg, 0, 0x2C);
            packet = RhText_SetTextureFlush(packet);
            SzMsg_GetInfo(1);
            packet = SzMsg_DrawX_Pipe(packet, 2, 0x140 - (*(int *)((unsigned char *)SzMsg_GetInfo(2) + 0xC) >> 1), y + 0x136, 0, SzMsg_DrawRect_Pipe);
            SzMsg_Free(1);
            SzMsg_Free(2);
            break;
        case 11:
            value = *(unsigned short *)(p2 + 0xCAA);
            text = data_00291130_dialog[type];
            y = 0x200 - ((*(short *)(p4 + 0xC9C) << 9) >> 6);
            icon = SzDuel_GetMsgName(value);
            width = 0x15A;
            res = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(packet, 2, icon, 0, 0x16), 3, 0x98, 0, 0x2C));
            w = *(int *)((unsigned char *)SzMsg_GetInfo(2) + 0xC) + 0x18;
            if (w > 0x15A) {
                width = w;
            }
            w = *(int *)((unsigned char *)SzMsg_GetInfo(3) + 0xC) + 0x18;
            if (width < w) {
                width = w;
            }
            packet = lSzDuel_DrawDialog_Window_Pipe(res, 2, -1, -1, 0x140, y + 0x32, width, 0x15C, func_0019bef0(text), (DotrDialogWord)text, 1);
            packet = SzMsg_DrawX_Pipe(packet, 2, 0x140 - (*(int *)((unsigned char *)SzMsg_GetInfo(2) + 0xC) >> 1), y + 0x12E, 0, SzMsg_DrawRect_Pipe);
            packet = SzMsg_DrawX_Pipe(packet, 3, 0x140 - (*(int *)((unsigned char *)SzMsg_GetInfo(3) + 0xC) >> 1), y + 0x150, 0, SzMsg_DrawRect_Pipe);
            SzMsg_Free(2);
            SzMsg_Free(3);
            SzDuel_InitCardInfo(&card, value);
            SzDuel_ClearDrawCard(&draw);
            draw.scale[0] = 0x3EF00000;
            draw.scale[1] = 0x3EFA6F4E;
            draw.scale[2] = 0x3F800000;
            draw.scale[3] = 0;
            draw.flags.reserved = 0;
            SzMsg_Free(0);
            packet = SzDuel_StartCardSession_Pipe(packet);
            SzDuel_SetCardDefPrioZ(0xFFFFFE);
            SzDuel_GetCardNodeOnScreen(&draw, 0, 0.0f, (float)(y - 0x18));
            packet = SzDuel_DrawCard_Pipe(SzDuel_SetupCard2_Pipe(packet, (DotrDialogWord)&card, &draw), (DotrDialogWord)&draw);
            packet = SzDuel_StartWindowSessionWithPalette_Pipe(SzDuel_FinishCardSessionX_Pipe(packet), 0);
            SzDuel_SetWindowDefPrioZ(0xFFFFFE);
            SzMsg_SetDefPrioZ(0xFFFFFE);
            break;
        case 12:
        case 13:
            value = *(unsigned short *)(p2 + 0xCAA);
            x_c12 = *(short *)(p4 + 0xC9A);
            text_c12 = data_00291130_dialog[type];
            y = *(short *)(p4 + 0xC9C);
            icon = SzDuel_GetMsgName(value);
            packet = lSzDuel_DrawDialog_Window_Pipe(packet, 2, value, icon, x_c12 + 0x140, y + 0x4C, 0x15A, 0x128, func_0019bef0(text_c12), (DotrDialogWord)text_c12, 1);
            packet = SzMsg_LoadFont_Pipe(packet, 1, 0x99, 0, 0x16);
            packet = RhText_SetTextureFlush(packet);
            entry = (unsigned char *)SzMsg_GetInfo(1);
            packet = SzMsg_DrawX_Pipe(packet, 1, (0x140 - (*(int *)(entry + 0xC) >> 1)) + x_c12, y + 0x136, 0, SzMsg_DrawRect_Pipe);
            SzMsg_Free(1);
            break;
        case 14:
            text_c14 = data_00291130_dialog[type];
            lowA = SzDuel_GetPlayer(0);
            lowB = SzDuel_GetPlayer(1);
            lowA = (lowA & 0xFF) < 0x64 ? 1 : 0;
            lowB = (lowB & 0xFF) < 0x64 ? 1 : 0;
            y = 0x200 - ((*(short *)(p4 + 0xC9C) << 9) >> 6);
            on = (state[0xD36] & 0x80) ? 1 : 0;
            page = state[0xD36] & 0x7F;
            start = page != 0 ? *(unsigned short *)(state + 0xC8E) : *(unsigned short *)(state + 0xC8A);
            end = page != 0 ? *(unsigned short *)(state + 0xC90) : *(unsigned short *)(state + 0xC8C);
            cursor = page != 0 ? state[0xD5F] : state[0xD5E];
            list = page != 0 ? *(unsigned short **)(state + 0x6DC) : *(unsigned short **)(state + 0x6D8);
            packet = lSzDuel_DrawDialog_Window_Pipe(packet, 2, -1, -1, 0x140, y + 0x68, 0x15A, 0xF0, func_0019bef0(text_c14), (DotrDialogWord)text_c14, 0);
            if (on != 0) {
                w = y + 0x7BE;
                if (page == 0) {
                    packet = lSzDuel_DrawWindow_Icon_Pipe(packet, gSzDuel_Icon_dialog + (lowA + 0xB9) * 10, 0x7E8, w, 0, 1, 0, 0, 1, 0, 1);
                } else {
                    packet = lSzDuel_DrawWindow_Icon_Pipe(packet, gSzDuel_Icon_dialog + (lowB + 0xBB) * 10, 0x7E8, w, 0, 1, 0, 0, 1, 0, 1);
                }
            } else {
                dir = page == 0 ? 1 : -1;
                packet = lSzDuel_DrawWindow_Icon_Pipe(packet, gSzDuel_Icon_dialog + (lowA + 0xB9) * 10, 0x7B8, y + 0x7BE, 0, 1, 0, 0, (page == 0 ? 1 : 0), 0, dir);
                packet = lSzDuel_DrawWindow_Icon_Pipe(packet, data_00290b02_dialog, 0x7F1, y + 0x7C3, 0, 1, 0, 0, 1, 0, -1);
                packet = lSzDuel_DrawWindow_Icon_Pipe(packet, data_00290af8_dialog, 0x804, y + 0x7C3, 0, 1, 0, 0, 1, 0, -1);
                w = y + 0x7BE;
                if (page == 1) {
                    dir = 1;
                } else {
                    dir = -1;
                }
                packet = lSzDuel_DrawWindow_Icon_Pipe(packet, gSzDuel_Icon_dialog + (lowB + 0xBB) * 10, 0x818, w, 0, 1, 0, 0, (page == 1 ? 1 : 0), 0, dir);
            }
            packet = lSzDuel_DrawWindow_Icon_Pipe(packet, data_00290828_dialog, 0x867, y + 0x7C2, 0, 1, 0, 0, 1, 0, -1);
            packet = lSzDuel_DrawWindow_Icon_Pipe(packet, data_00290aa8_dialog, 0x88B, y + 0x7C7, 0, 1, 0, 0, 1, 0, -1);
            packet = lSzDuel_DrawWindow_Board_Pipe(packet, page + 0xE, 0xB4, y + 0xB8, -0x118, -0x7E, page + 0xE);
            packet = lSzDuel_DrawWindow_Icon_Pipe(packet, data_00290832_dialog, 0x867, y + 0x85C, 0, 1, 0, 0, 1, 0, -1);
            packet = lSzDuel_DrawWindow_Icon_Pipe(packet, data_00290ab2_dialog, 0x88B, y + 0x85C, 0, 1, 0, 0, 1, 0, -1);
            for (i = 0, w = y + 0xBB; i < 5 && start + i < end; w += 0x18, i++) {
                id = list[start + i];
                entry = SzDuel_GetCardConstInfo(id);
                if (entry != 0) {
                    msg = *entry >> 5;
                    value = *entry & 0x1F;
                    if (msg == 0) {
                        pic = gSzDuel_Icon_dialog + value * 10;
                    } else {
                        pic = gSzDuel_Icon_dialog + (msg + 0x14) * 10;
                    }
                    scale = 1.0f;
                    packet = SzMsg_LoadFont_Pipe(packet, 1, SzDuel_GetMsgName(id), 0, 0x16);
                    packet = RhText_SetTextureFlush(packet);
                    packet = lSzDuel_DrawWindow_Icon_Pipe(packet, pic, 0x777, w + 0x720, 0, 1, 0, 0, 1, 0, -1);
                    width = *(int *)((unsigned char *)SzMsg_GetInfo(1) + 0xC);
                    if (width > 0xFA) {
                        scale = 250.0f / (float)width;
                    }
                    packet = SzMsg_DrawRatioX_Pipe(packet, 1, 0xCF, w, scale, 1.0f, 0, SzMsg_DrawRect_Pipe);
                    SzMsg_Free(1);
                }
            }
            if (end != 0) {
                w = y + 0xBB;
                w += cursor * 0x18;
                packet = lSzDuel_DrawWindow_Frame_Pipe(packet, -1, -1, 7, 0xB7, w, -0x112, -0x18, 1);
            }
            break;
        case 15:
        case 16:
            x = *(short *)(p4 + 0xC9A);
            y = *(short *)(p4 + 0xC9C);
            text = data_00291130_dialog[type];
            value = *(unsigned short *)(p2 + 0xCAA);
            packet = lSzDuel_DrawDialog_Window_Pipe(packet, 2, -1, value, x + 0x140, y + 0xE0, 0, 0, func_0019bef0(text), (DotrDialogWord)text, 0);
            break;
        case 17:
            y = 0x200 - ((state[0xD3A] << 9) >> 6);
            if (state[0xD35] == 24) {
                value = *(unsigned short *)(*(unsigned char **)(state + 0x6C8) + 0x3C);
                state[0xD5C] = *(unsigned char *)(*(unsigned char **)(state + 0x6C8) + 0x44);
            } else if (state[0xD35] == 45) {
                value = *(unsigned short *)(p2 + 0xCAA);
                state[0xD5C] = state[0xD36] & 0x7F;
            } else if (state[0xD35] == 48) {
                value = *(unsigned short *)(p2 + 0xCAA);
                state[0xD5C] = state[0xD36] & 0x7F;
            } else {
                value = SzDuel_InqHand(state[0xD54], state[0xD4F]);
                if (value < 0) {
                    value = 0x3E7;
                } else {
                    value &= 0xFFF;
                }
                state[0xD5C] = state[0xD54];
            }
            packet = lSzDuel_DrawBuildDeck_Detail_Pipe(packet, value, -0x1E0, y, 1, 1, 1, 0);
            break;
        case 18:
            x_c18 = *(short *)(p4 + 0xC9A);
            y = *(short *)(p4 + 0xC9C);
            value = *(unsigned short *)(p2 + 0xCAA);
            SzMsg_Free(0);
            packet = SzDuel_StartCardSession_Pipe(packet);
            SzDuel_SetCardDefPrioZ(0xFFFFFE);
            SzDuel_ClearDrawCard(&draw2);
            row = -0xFA;
            draw2.flags.horizontal = 0;
            draw2.flags.colored = 0;
            for (i_c18 = 0; i_c18 < 5; i_c18++) {
                id = SzDuel_InqHand(value, i_c18);
                if (id >= 0) {
                    id &= 0xFFF;
                    if (id != 0x3E7) {
                        SzDuel_InitCardInfo(&card2, id);
                        card2.owner = value;
                        SzDuel_GetCardNodeOnScreen(&draw2, 0, (float)(x_c18 + row), (float)y);
                        packet = SzDuel_DrawCard_Pipe(SzDuel_SetupCard2_Pipe(packet, (DotrDialogWord)&card2, &draw2), (DotrDialogWord)&draw2);
                    }
                }
                row += 0x7D;
            }
            packet = SzDuel_StartWindowSessionWithPalette_Pipe(SzDuel_FinishCardSessionX_Pipe(packet), 0);
            SzDuel_SetWindowDefPrioZ(0xFFFFFE);
            SzMsg_SetDefPrioZ(0xFFFFFE);
            break;
        case 19:
            packet = lSzDuel_DrawDialog_Window_Pipe(packet, -1, -1, *(unsigned short *)(p2 + 0xCAA), *(short *)(p4 + 0xC9A) + 0x140, *(short *)(p4 + 0xC9C) + 0xE0, 0, 0, 0, 0, 0);
            break;
        case 20:
            value_c20 = *(unsigned short *)(p2 + 0xCAA);
            x_c20 = *(short *)(p4 + 0xC9A);
            y_c20 = *(short *)(p4 + 0xC9C);
            res = SzMsg_LoadFont_Pipe(packet, 1, value_c20, 0, 0x16);
            res = RhText_SetTextureFlush(res);
            width_c20 = *(int *)((unsigned char *)SzMsg_GetInfo(1) + 0x10) + 0x18;
            SzMsg_Free(1);
            packet = lSzDuel_DrawDialog_Window_Pipe(res, -1, -1, value_c20, x_c20 + 0x140, (0xE0 - (width_c20 / 2)) + y_c20, 0, width_c20, 0, 0, 0);
            break;
        case 21:
            value_c21 = *(unsigned short *)(p2 + 0xCAA);
            x_c21 = *(short *)(p4 + 0xC9A);
            y_c21 = *(short *)(p4 + 0xC9C);
            res = SzMsg_LoadFont_Pipe(packet, 1, value_c21, 0, 0x16);
            res = RhText_SetTextureFlush(res);
            width_c21 = *(int *)((unsigned char *)SzMsg_GetInfo(1) + 0x10) + 0x34;
            SzMsg_Free(1);
            packet = lSzDuel_DrawDialog_Window_Pipe(res, -1, -1, value_c21, x_c21 + 0x140, (0xE0 - (width_c21 / 2)) + y_c21, 0, width_c21, 0, 0, 1);
            break;
        case 22:
            text1 = data_002bef6f_dialog;
            y_c22 = *(short *)(p4 + 0xC9C);
            text2 = data_002bef73_dialog;
            value_c22_c22 = *(unsigned short *)(p2 + 0xCAA);
            res = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(packet, 1, value_c22_c22, 0, 0x16));
            width_c22 = *(int *)((unsigned char *)SzMsg_GetInfo(1) + 0x10) + 0x3C;
            SzMsg_Free(1);
            packet = lSzDuel_DrawDialog_Window_Pipe(res, 2, -1, value_c22_c22, 0x140, (0xE0 - (width_c22 >> 1)) + y_c22, 0x15A, width_c22, 0, 0, 0);
            res = SzDuel_DrawWindow_Font_Pipe(packet, 0xEA, (width_c22 >> 1) + 0xBC + y_c22, 2, 3, func_0019bef0(text1), text1, state[0xD35] != 0 ? 0 : (DotrDialogWord)data_002be944_dialog);
            packet = SzDuel_DrawWindow_Font_Pipe(res, 0x196, (width_c22 >> 1) + 0xBC + y_c22, 2, 3, func_0019bef0(text2), text2, state[0xD35] != 0 ? (DotrDialogWord)data_002be944_dialog : 0);
            break;
        case 23:
            packet = lSzDuel_OpenHandCursor_Pipe(packet, *(unsigned short *)(p2 + 0xCAA), *(short *)(p4 + 0xC9A), *(short *)(p4 + 0xC9C));
            break;
        }
    }
    SzMsg_Free(0);
    return packet;
}
