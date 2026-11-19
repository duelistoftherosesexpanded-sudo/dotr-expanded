#ifndef DOTR_EE_LK_ASSIGN_UV_H
#define DOTR_EE_LK_ASSIGN_UV_H

/* Sprite texture-coordinate setters: a rectangle written to the four uv words of a packet in
 * one of eight orientations, and the animated-frame forms LkAssignUVST / LkAssignUVST3.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* A 14-bit coordinate in a 16-bit word (12.4 fixed point). */
struct DotrLkAssignCoord {
    unsigned short val : 14;
    unsigned short hi : 2;
};

/* One 32-byte vertex group of the sprite packet: the uv word pair at +8 (u, v halfwords). */
struct DotrLkAssignVert {
    unsigned char pad0[8];
    struct DotrLkAssignCoord x;
    struct DotrLkAssignCoord y;
    unsigned char padC[0xC];
};

/* The sprite packet: a flags halfword at +2 (0x800 set here), four vertex groups from +0x10.
 * Assumed: field meanings from their use here. */
struct DotrLkAssignSprite {
    unsigned short pad0;
    unsigned short flags;
    unsigned char pad4[0xC];
    struct DotrLkAssignVert v[4];
};

/* The texture rectangle: x, y, width, height. */
struct DotrLkAssignRect {
    short x;
    short y;
    short w;
    short h;
};

/* 0x104540 LkAssignUV (name from the Japanese build's symbols): sets bit 0x800 of the packet
 * flags and writes the rectangle's corners << 4 to the four uv pairs in orientation arg2
 * (0..7: the four corner orders of the rotations and mirrors; other values write nothing).
 * Returns 0. */
int LkAssignUV(void *arg0, void *arg1, int arg2);

/* --- LkAssignUVST (0x104dc0) and LkAssignUVST3 (0x105260): animated texture frames --- */

/* LkAssignUVST's packet view: a flags halfword at +2 and four 24-byte vertex
 * groups from +0x10 whose 8 bytes at +8 hold either two 14-bit uv coordinates or the float
 * s and t. Assumed: field meanings from their use here. */
struct DotrLkAssignStCoord {
    unsigned short val : 14;
    unsigned short hi : 2;
};
struct DotrLkAssignStVert {
    unsigned char pad0[8];
    union {
        struct {
            struct DotrLkAssignStCoord x;
            struct DotrLkAssignStCoord y;
        } uv;
        struct {
            float s;
            float t;
        } st;
    } tex;
    unsigned char pad10[8];
};
struct DotrLkAssignStSprite {
    unsigned short pad0;
    unsigned short flags;
    unsigned char pad4[0xC];
    struct DotrLkAssignStVert v[4];
};
/* LkAssignUVST's animation record: the current cell as integer texels (u, v, size, base, sheet
 * size) or as floats (x, y, w, h in texture units), flag bits at +0x38 (1 integer texels,
 * 2 frames run along rows), and the frame counters at +0x48. Assumed: field meanings. */
struct DotrLkAssignCell {
    short u;
    short v;
    short w;
    short h;
    short baseU;
    short baseV;
    short texW;
    short texH;
};
struct DotrLkAssignCellF {
    float x;
    float y;
    float w;
    float h;
};
struct DotrLkAssignAnim {
    union {
        struct DotrLkAssignCell i;
        struct DotrLkAssignCellF f;
    } r;
    unsigned char pad10[0x28];
    int flags;
    unsigned char pad3C[0xC];
    short timer;
    short delay;
    short last;
    short loopLength;
    short frame;
    short loop;
};
/* 0x104dc0 LkAssignUVST (Japanese build name). */
void LkAssignUVST(void *arg0, void *arg1);

/* LkAssignUVST3's views: the uv word pair at +0x18, +0x30, +0x48, +0x60 of the packet (two
 * 14-bit coordinates in separate halfwords, or the float s and t), the flags halfword at +2;
 * the state record with flag bits at +4 (1 integer texels, 2 frames along rows, 4 restart at a
 * random offset), the base position at +0x10, the cell at +0x20, sixteen 16-byte per-instance
 * frame records from +0x30 (position, frame index, timer), the random ranges at +0x136/+0x138
 * and the delay, last frame and rewind count at +0x13c..+0x140. Assumed: field meanings. */
struct DotrLkAssignUv3 {
    unsigned short u : 14;
    unsigned short unusedU : 2;
    unsigned short v : 14;
    unsigned short unusedV : 2;
};
struct DotrLkAssignFrame3 {
    float x;
    float y;
    unsigned int unused;
    short index;
    short timer;
};
struct DotrLkAssignState3 {
    unsigned int unused0;
    unsigned int flags;
    unsigned char unused8[8];
    float x;
    float y;
    unsigned char unused18[8];
    union {
        struct {
            short u, v;
            short width, height;
            short baseU, baseV;
            short imageWidth, imageHeight;
        } fixed;
        struct {
            float u, v;
            float width, height;
        } floating;
    } uv;
    struct DotrLkAssignFrame3 frames[16];
    unsigned char unused130[6];
    short randomX;
    short randomY;
    short unused13A;
    short delay;
    short end;
    short rewind;
};
/* Library block: assumed to return the next pseudo-random number. */
extern int func_0019a5c8(void);
/* 0x105260 LkAssignUVST3 (Japanese build name). arg2 selects the frame record. */
void LkAssignUVST3(void *arg0, void *arg1, int arg2);

#endif
