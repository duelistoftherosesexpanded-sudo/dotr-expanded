#ifndef DOTR_EE_PACKET_BUILDS_H
#define DOTR_EE_PACKET_BUILDS_H

/* Build a GIF packet in the object at 0x32a5e0 through the SDK packet helpers at
 * 0x1875f8-0x1878b0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

#ifdef __MWERKS__
typedef __int128 DotrPacketWord128;
#else
__extension__ typedef __int128 DotrPacketWord128;
#endif

/* 0x20f290: one fixed packet built at 0x32a5e0 from the caller's cursor; returns what the
 * last helper returns. Assumed: the packet object's type (only its address is used), the
 * helper signatures (argument registers only; the helpers are SDK library code), and the
 * 16-byte value at 0x32a600 as one 128-bit object written in two 64-bit halves and passed
 * by value. */
extern int data_0032a5e0[];
extern DotrPacketWord128 data_0032a600;
extern void func_001875f8(void *packet, int cursor);
extern void func_00187660(void *packet, int unknown_1);
extern void func_001877f8(void *packet, int unknown_1);
extern void func_00187778(void *packet, int unknown_1, int unknown_2);
extern void func_00187858(void *packet, int unknown_1);
extern void func_001876b8(void *packet, DotrPacketWord128 value);
extern void func_00187828(void *packet, int unknown_1, long unknown_2);
extern void func_001876d0(void *packet);
extern void func_001878b0(void *packet);
extern int func_00187608(void *packet);
int RhText_SetTextureFlush(int cursor);

/* 0x20b000 lRhBase_InitGsAndVif (original name): the same helper sequence on the packet
 * object at 0x325f00 (`SceVif1$298` in the Japanese build) with the 16-byte value at
 * 0x325ef0 (`owGif$297`). Assumed: void result (the caller-visible v0 is whatever the last
 * helper leaves), func_00187810's signature, and a 64-bit third argument for func_00187828
 * (this function passes 64-bit values in a2). data_00326180_base views `gRhBase_dl` + 0x230,
 * which draw_state.h declares as the word table data_00326180: a 64-bit word followed by
 * the width and height words this function reads. */
struct DotrPacketBase {
    unsigned long test;
    int unknown_08;
    int width;
    int height;
};
extern struct DotrPacketBase data_00326180_base;
extern int data_00325f00[];
extern DotrPacketWord128 data_00325ef0;
extern void func_00187810(void *packet, int unknown_1);
void lRhBase_InitGsAndVif(int cursor);

/* 0x20b1e0 lRhBase_SetRandomDither (original name): rotates the 64-bit word at 0x326180
 * right by four times a random remainder and sends it as register 68 in a packet built in
 * a 16-byte-aligned stack buffer. Assumed: the buffer as 63 raw bytes aligned by hand (a
 * 32-byte packet object followed by the 128-bit value), func_0019a5c8 (library code,
 * unnamed) as an int random source, int cursor and result (draw_state.h declares the
 * same function with its DotrDrawWord). DotrPacketAddr is the address as an integer. */
#ifdef __MWERKS__
typedef int DotrPacketAddr;
#else
typedef __INTPTR_TYPE__ DotrPacketAddr;
#endif
extern int func_0019a5c8(void);
int lRhBase_SetRandomDither(int cursor);

/* 0x20a0b0 RhBase_SetZBufferMode (original name): builds one fixed packet at 0x325e40 that
 * sets register 78 to the 64-bit record at 0x325e70. With both flag bits of 0x325f40 set
 * the record's nine-bit field is (+0xc + +0x1c of 0x325f20) and its four-bit field +0x18;
 * otherwise the record is copied from 0x325fc0. Bit 32 is set when `a` is zero. Assumed:
 * int cursor and result (other groups declare the same function with their cursor word);
 * the record as a union of byte-sized bit-fields and one 64-bit word (the code reads and
 * writes halves and bytes for the fields, and 64 bits for the copy and the argument); the
 * `_packet` symbols are this group's views of storage draw_state.h declares as
 * data_00325f20_setup, data_00325f40 and data_00325fc0_fields (the two headers cannot be
 * included together). */
struct DotrPacketFlagPair {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bits_2_7 : 6;
};
struct DotrPacketFrameSetup {
    unsigned char unknown_00[0xc];
    int unknown_0c;
    unsigned char unknown_10[8];
    int unknown_18;
    int unknown_1c;
    struct DotrPacketFlagPair unknown_20;
};
struct DotrPacketZbufFields {
    unsigned short low_9 : 9;
    unsigned short unknown_bits_9_15 : 7;
    unsigned char unknown_02;
    unsigned char low_4 : 4;
    unsigned char unknown_bits_4_7 : 4;
    unsigned char mask : 1;
    unsigned char unknown_bits_33_39 : 7;
};
union DotrPacketZbuf {
    struct DotrPacketZbufFields f;
    long word;
};
extern struct DotrPacketFrameSetup data_00325f20_packet;
extern struct DotrPacketFlagPair data_00325f40_packet;
extern int data_00325e40[];
extern DotrPacketWord128 data_00325e60;
extern union DotrPacketZbuf data_00325e70;
extern union DotrPacketZbuf data_00325fc0_packet;
int RhBase_SetZBufferMode(int cursor, int a);

#endif
