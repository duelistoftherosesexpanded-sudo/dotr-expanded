#include "dotr/ee/trade_bag.h"

/* lSzDuel_DrawTrade_Bag_Pipe (0x1c3e20): side arg1's bag panel of the trade screen at
 * (arg2, arg3), every x taken from the left or the right column by the side: title and
 * frames, the card totals of both sides, the bag count, the ten sort tabs (the current one
 * highlighted; with sort selection enabled in its own colour) and the sort name, five card
 * rows from the list top, the description, the three labels (the side's chosen one in the
 * plain colour) and, when the list is not empty, a grey bar and the position and length.
 * Compiled with #pragma opt_propagation off, set directly before the function and reset after
 * it (docs/DECOMPILATION.md). */

#ifndef NULL
#define NULL 0
#endif

static inline int side_001C3E20(int player, int left, int right)
{
    return player != 0 ? right : left;
}

#define SIDE_001C3E20(left, right) side_001C3E20(arg1, left, right)

static inline DotrTradeBagWord text_001C3E20(DotrTradeBagWord cursor, int x, int y, int align, int flags,
                                             signed char *text, void *color)
{
    int length = func_0019bef0((char *)text);
    return func_001b6f70_tradebag(cursor, x, y, align, flags, length, text, color);
}

static inline int total_001C3E20(int player)
{
    unsigned char *state = data_00317490_tradebag;
    unsigned char *cards;
    int total;
    int i;

    if (player != 0) {
        cards = DOTR_TRADE_BAG_PTR(unsigned char *, state, 0x6E4);
    } else {
        cards = DOTR_TRADE_BAG_PTR(unsigned char *, state, 0x6E0);
    }
    total = 0;
    i = 0;
    do {
        total += *cards++;
        i++;
    } while (i < 0x356);
    return total;
}

static inline DotrTradeBagWord number_001C3E20(DotrTradeBagWord cursor, int x, int y, int player)
{
    int count = SzData_GetBagCountAll(player);
    return func_001b6ea0_tradebag(cursor, x, y, 1, 0, 4, count);
}

static inline DotrTradeBagWord rectangle_001C3E20(DotrTradeBagWord cursor, struct DotrTradeBagPacket *packet, int x,
                                                  int y, unsigned char *data)
{
    ((struct DotrTradeBagHeader *)(DotrTradeBagWord)&packet->header)->count = 6;
    ((struct DotrTradeBagHeader *)(DotrTradeBagWord)&packet->header)->type = 3;
    ((struct DotrTradeBagHeader *)(DotrTradeBagWord)&packet->header)->registers = 0x441;
    ((struct DotrTradeBagHeader *)(DotrTradeBagWord)&packet->header)->pad = 0;
    packet->r = 0xA0;
    packet->g = 0xA0;
    packet->b = 0xA0;
    packet->a = 0x80;
    packet->q = 1.0f;
    packet->first.x = (x + 0x6CA) << 4;
    packet->first.y = (y + 0x738) << 4;
    packet->first.z = *(int *)(data + 0xC58);
    packet->first.fog = 0;
    packet->second.x = (x + 0x6F2) << 4;
    packet->second.y = (y + 0x739) << 4;
    packet->second.z = *(int *)(data + 0xC58);
    packet->second.fog = 0;
    return func_00207650_tradebag(cursor, (DotrTradeBagWord)&packet->header, 0, 0);
}

#ifdef __MWERKS__
#pragma opt_propagation off
#endif

DotrTradeBagWord lSzDuel_DrawTrade_Bag_Pipe(DotrTradeBagWord arg0, int arg1, int arg2, int arg3)
{
    unsigned char *data;
    char *descText;
    signed char *sortText;
    int flags;
    unsigned short *list;
    unsigned short top;
    unsigned short count;
    unsigned char pos;
    unsigned char *state;
    int x;
    int special;
    int style;
    int sortY;
    unsigned char *color;
    int labelY;
    int labelX;
    int active;
    signed char *subText;
    int textX;
    int textY;
    signed char **labelPtr;
    signed char *labelText;
    int pageX;
    int sortIndex;
    unsigned char *sortData;
    DotrTradeBagWord cursor;
    int selection;
    int sortX;
    int pageY;
    signed char *titleText;
    int enabled;
    struct DotrTradeBagWork work;

    data = data_00317490_tradebag;
    special = (data_0031814e_tradebag[arg1] == 1) ? 1 : 0;
    enabled = 0;
    if (special != 0) {
        if (SzPad_Check(arg1, 1, 0x100) != 0) {
            enabled = 1;
        }
    }
    selection = enabled != 0 ? 1 : 0;
    color = NULL;
    if (selection != 0) {
        color = data_002be8a4_tradebag;
    } else {
        color = NULL;
    }
    if (arg1 != 0) {
        titleText = (signed char *)data_002bf021_tradebag;
    } else {
        titleText = (signed char *)data_002bf024_tradebag;
    }
    subText = (signed char *)(data_002bf024_tradebag + 3);
    descText = data_002bef98_tradebag;
    work.labels = data_00291490_tradebag;
    if (arg1 != 0) {
        list = DOTR_TRADE_BAG_PTR(unsigned short *, data, 0x6DC);
    } else {
        list = DOTR_TRADE_BAG_PTR(unsigned short *, data, 0x6D8);
    }
    top = arg1 != 0 ? *(unsigned short *)(data + 0xC8E) : *(unsigned short *)(data + 0xC8A);
    count = arg1 != 0 ? *(unsigned short *)(data + 0xC90) : *(unsigned short *)(data + 0xC8C);
    pos = arg1 != 0 ? data[0xD5F] : data[0xD5E];
    sortData = (arg1 != 0 ? data + 0xCB9 : data + 0xCB6) + selection;
    sortText = data_00290fd0_tradebag[*sortData];
    flags = 3;
    textX = arg2 + SIDE_001C3E20(0x10, 0x240);
    textY = arg3 + 0x10;
    arg0 = func_001b4780_tradebag(arg0, 0xC, -1, 0xB, textX, textY, -0x30, -0x30, 1);
    textX = arg2 + SIDE_001C3E20(0x10, 0x240) + 0x18;
    textY = arg3 + 0x1F;
    cursor = text_001C3E20(arg0, textX, textY, 2, 3, titleText, NULL);
    x = arg2 + SIDE_001C3E20(0x40, 0x1B0);
    cursor = func_001b4780_tradebag(cursor, 0xC, -1, 6, x, arg3 + 0x10, -0x8C, -0x2C, 1);
    textX = arg2 + SIDE_001C3E20(0x40, 0x1B0) + 0x48;
    textY = arg3 + 0x15;
    cursor = text_001C3E20(cursor, textX, textY, 1, 3, subText, NULL);
    textX = arg2 + SIDE_001C3E20(0x40, 0x1B0) + 0x24;
    textY = arg3 + 0x23;
    cursor = text_001C3E20(cursor, textX, textY, 1, 3, work.labels.text[1], NULL);
    textY = arg3 + 0x31;
    cursor = func_001b6ea0_tradebag(cursor, textX, textY, 1, 2, 4, total_001C3E20(arg1));
    textX = arg2 + SIDE_001C3E20(0x40, 0x1B0) + 0x6C;
    textY = arg3 + 0x23;
    cursor = text_001C3E20(cursor, textX, textY, 1, 3, work.labels.text[2], NULL);
    textY = arg3 + 0x31;
    cursor = func_001b6ea0_tradebag(cursor, textX, textY, 1, 2, 4, total_001C3E20(arg1 ^ 1));
    x = arg2 + SIDE_001C3E20(0x10, 0x230);
    cursor = func_001b4780_tradebag(cursor, 0xC, -1, 5, x, arg3 + 0x40, -0x40, -0x14, 1);
    x = arg2 + 0x6C2 + SIDE_001C3E20(0x10, 0x230);
    cursor = func_001b3cc0_tradebag(cursor, data_0029086e_tradebag, x, arg3 + 0x762, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    {
        DotrTradeBagWord c = cursor;
        int x = arg2 + SIDE_001C3E20(0x10, 0x230) + 0x17;
        int y = arg3 + 0x44;
        cursor = number_001C3E20(c, x, y, arg1);
    }
    x = arg2 + SIDE_001C3E20(0x10, 0x230);
    cursor = func_001b65f0_tradebag(cursor, 3, x, arg3 + 0x54, -0x40, -0x1C, -1, 0);
    x = arg2 + 0x6C0 + SIDE_001C3E20(0x15, 0x255);
    cursor = func_001b3cc0_tradebag(cursor, data_00290aa8_tradebag, x, arg3 + 0x77D, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    x = arg2 + 0x6C0 + SIDE_001C3E20(0x2C, 0x234);
    cursor = func_001b3cc0_tradebag(cursor, data_00290828_tradebag, x, arg3 + 0x77C, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    x = arg2 + SIDE_001C3E20(0x50, 0x140);
    cursor = func_001b65f0_tradebag(cursor, 3, x, arg3 + 0x40, -0xF0, -0x30, -1, 0);
    x = arg2 + 0x6CE + SIDE_001C3E20(0x50, 0x140);
    cursor = func_001b3cc0_tradebag(cursor, data_00290896_tradebag, x, arg3 + 0x765, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    x = arg2 + 0x6C7 + SIDE_001C3E20(0x50, 0x140);
    cursor = func_001b3cc0_tradebag(cursor, data_00290abc_tradebag, x, arg3 + 0x77E, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    sortX = arg2 + 0x6E8 + SIDE_001C3E20(0x50, 0x140);
    sortY = arg3 + 0x769;
    sortIndex = 0;
    do {
        style = -1;
        if (sortIndex == *sortData) {
            style = selection != 0 ? 2 : 1;
        }
        cursor = func_001b3cc0_tradebag(cursor, gSzDuel_Icon_tradebag + (sortIndex + 0xD1) * 10, sortX, sortY,
                                        0, 1, 0, 0, 0, 0, 1, color, style, 0x80);
        sortIndex++;
        sortX += 0x10;
    } while (sortIndex < 10);
    x = arg2 + 0x78A + SIDE_001C3E20(0x50, 0x140);
    cursor = func_001b3cc0_tradebag(cursor, data_002908a0_tradebag, x, arg3 + 0x765, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    x = arg2 + 0x793 + SIDE_001C3E20(0x50, 0x140);
    cursor = func_001b3cc0_tradebag(cursor, data_00290ac6_tradebag, x, arg3 + 0x77E, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    sortX = arg2 + SIDE_001C3E20(0x50, 0x140) + 0x78;
    sortY = arg3 + 0x5E;
    if (color != NULL) {
        flags |= 8;
    }
    cursor = text_001C3E20(cursor, sortX, sortY, 0, flags, sortText, color);
    textX = arg2 + SIDE_001C3E20(8, 0x148);
    sortY = arg3 + 0x70;
    sortIndex = 0;
    do {
        cursor = func_001c4b00_tradebag(cursor, arg1, list[top + sortIndex], textX, sortY, 1);
        sortY += 0x36;
        sortIndex++;
    } while (sortIndex < 5);
    x = arg2 + SIDE_001C3E20(0x10, 0x184);
    cursor = func_001b65f0_tradebag(cursor, 3, x, arg3 + 0x17E, -0xEC, -0x32, -1, 0);
    x = arg2 + 0x6C0 + SIDE_001C3E20(0x2C, 0x234);
    cursor = func_001b3cc0_tradebag(cursor, data_00290832_tradebag, x, arg3 + 0x8A2, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    x = arg2 + 0x6C0 + SIDE_001C3E20(0x15, 0x255);
    cursor = func_001b3cc0_tradebag(cursor, data_00290ab2_tradebag, x, arg3 + 0x8A7, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    x = arg2 + 0x6C0 + SIDE_001C3E20(0x17, 0x259);
    cursor = func_001b3cc0_tradebag(cursor, data_00290a6c_tradebag, x, arg3 + 0x8BA, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    {
        DotrTradeBagWord c = cursor;
        int x = arg2 + SIDE_001C3E20(0x2C, 0x231);
        int y = arg3 + 0x19D;
        cursor = text_001C3E20(c, x, y, 1, 0, (signed char *)descText, NULL);
    }
    labelX = arg2 + SIDE_001C3E20(0x7E, 0x18A);
    labelY = arg3 + 0x182;
    labelPtr = work.labels.text;
    sortIndex = 0;
    state = data + arg1;
    do {
        if (state[0xCC0] == sortIndex) {
            active = 1;
        } else {
            active = 0;
        }
        cursor = func_001b65f0_tradebag(cursor, 3, labelX, labelY, -0x78, -0xE, -1, 1);
        labelText = *labelPtr;
        cursor = func_001b6f70_tradebag(cursor, labelX + 1, labelY + 1, 1, 0, func_0019bef0((char *)labelText),
                                        labelText,
                                        (unsigned char *)(active != 0 ? 0 : (DotrTradeBagWord)data_002be944_tradebag));
        sortIndex++;
        labelY += 0xE;
        labelPtr++;
    } while (sortIndex < 3);
    sortX = arg2 + SIDE_001C3E20(0xFC, 0x148);
    sortY = arg3 + 0x17E;
    cursor = func_001b65f0_tradebag(cursor, 3, sortX, sortY, -0x3C, -0x32, -1, 0);
    if (count) {
        cursor = rectangle_001C3E20(cursor, &work.packet, sortX, sortY, data);
        pageX = arg2 + SIDE_001C3E20(0xFC, 0x148) + 0x1E;
        pageY = arg3 + 0x186;
        cursor = func_001b6ea0_tradebag(cursor, pageX, pageY, 1, 2, 3, top + pos + 1);
        cursor = func_001b6ea0_tradebag(cursor, pageX, pageY + 0x17, 1, 2, 3, count);
    }
    return cursor;
}

#ifdef __MWERKS__
#pragma opt_propagation reset
#endif
