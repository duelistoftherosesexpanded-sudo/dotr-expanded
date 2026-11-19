#include "dotr/ee/cursor_packets.h"

/* The custom-duel screen's background: lSzDuel_DrawBuildDeck_Base_Pipe with the colours at data_002be860. */
DotrDrawWord lSzDuel_DrawCustomDuel_Base_Pipe(DotrDrawWord cursor)
{
    struct DotrBasePipePacket packet;
    /* MATCHING DEVICE: the cast keeps the table address in a register. */
    unsigned char *col = (unsigned char *)data_002be860;
    int w = 320;
    int h = 448;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 20;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 8;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x41414141;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    packet.v[0].colour[0] = packet.v[2].colour[0] = col[2];
    packet.v[0].colour[1] = packet.v[2].colour[1] = col[1];
    packet.v[0].colour[2] = packet.v[2].colour[2] = col[0];
    packet.v[0].colour[3] = packet.v[2].colour[3] = col[3];
    packet.v[0].q = packet.v[2].q = 1.0f;
    packet.v[1].colour[0] = packet.v[3].colour[0] = col[6];
    packet.v[1].colour[1] = packet.v[3].colour[1] = col[5];
    packet.v[1].colour[2] = packet.v[3].colour[2] = col[4];
    packet.v[1].colour[3] = packet.v[3].colour[3] = col[7];
    packet.v[1].q = packet.v[3].q = 1.0f;
    packet.v[0].xyz.unknown_x = 1728 << 4;
    packet.v[0].xyz.unknown_y = 1824 << 4;
    packet.v[0].xyz.unknown_z = data_003180e8;
    packet.v[0].xyz.unknown_f = 0;
    packet.v[1].xyz.unknown_x = (w + 1728) << 4;
    packet.v[1].xyz.unknown_y = 1824 << 4;
    packet.v[1].xyz.unknown_z = data_003180e8;
    packet.v[1].xyz.unknown_f = 0;
    packet.v[2].xyz.unknown_x = 1728 << 4;
    packet.v[2].xyz.unknown_y = (h + 1824) << 4;
    packet.v[2].xyz.unknown_z = data_003180e8;
    packet.v[2].xyz.unknown_f = 0;
    packet.v[3].xyz.unknown_x = (w + 1728) << 4;
    packet.v[3].xyz.unknown_y = (h + 1824) << 4;
    packet.v[3].xyz.unknown_z = data_003180e8;
    packet.v[3].xyz.unknown_f = 0;
    cursor = ExSortPrim_Pipe(cursor, packet.header, 0, 0);

    packet.v[0].colour[0] = packet.v[2].colour[0] = col[6];
    packet.v[0].colour[1] = packet.v[2].colour[1] = col[5];
    packet.v[0].colour[2] = packet.v[2].colour[2] = col[4];
    packet.v[0].colour[3] = packet.v[2].colour[3] = col[7];
    packet.v[0].q = packet.v[2].q = 1.0f;
    packet.v[1].colour[0] = packet.v[3].colour[0] = col[2];
    packet.v[1].colour[1] = packet.v[3].colour[1] = col[1];
    packet.v[1].colour[2] = packet.v[3].colour[2] = col[0];
    packet.v[1].colour[3] = packet.v[3].colour[3] = col[3];
    packet.v[1].q = packet.v[3].q = 1.0f;
    packet.v[0].xyz.unknown_x = (w + 1728) << 4;
    packet.v[0].xyz.unknown_y = 1824 << 4;
    packet.v[0].xyz.unknown_z = data_003180e8;
    packet.v[0].xyz.unknown_f = 0;
    packet.v[1].xyz.unknown_x = (w * 2 + 1728) << 4;
    packet.v[1].xyz.unknown_y = 1824 << 4;
    packet.v[1].xyz.unknown_z = data_003180e8;
    packet.v[1].xyz.unknown_f = 0;
    packet.v[2].xyz.unknown_x = (w + 1728) << 4;
    packet.v[2].xyz.unknown_y = (h + 1824) << 4;
    packet.v[2].xyz.unknown_z = data_003180e8;
    packet.v[2].xyz.unknown_f = 0;
    packet.v[3].xyz.unknown_x = (w * 2 + 1728) << 4;
    packet.v[3].xyz.unknown_y = (h + 1824) << 4;
    packet.v[3].xyz.unknown_z = data_003180e8;
    packet.v[3].xyz.unknown_f = 0;
    /* MATCHING DEVICE: header address spelled from the leading member, as in SzMsg_DrawRect_Pipe. */
    return ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
}
