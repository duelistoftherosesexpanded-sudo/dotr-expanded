#ifndef DOTR_EE_PAINT_PRIMS_H
#define DOTR_EE_PAINT_PRIMS_H

/* PAINT_ primitive initialisers at 0x109050-0x109374: fill a GS primitive packet header and
 * colour fields, calling ExAssignAlpha or Ex3d_RotTransPersAuto and ExSortPrim.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* One 16-byte vertex slot as these functions store to it; member names are
 * offsets except rgba, which PAINT__2G_SET_IN copies byte by byte. */
struct DotrPaintEntry {
    unsigned char rgba[4];
    int unknown_04;
    unsigned char unknown_08[7];
    unsigned char unknown_0f;
};

/* 16-byte header (count, flags, a word, a 64-bit register list) and slots. */
struct DotrPaintPacket {
    unsigned short count;
    unsigned short flags;
    int unknown_04;
    long regs;
    struct DotrPaintEntry entries[4];
};

/* The same slots seen by PAINT_PIRM_LG2: 24 bits at bit 32 of the second
 * 64-bit word of the first two slots are replaced as a bit-field. */
struct DotrPaintWideEntry {
    long unknown_00;
    unsigned long unknown_low : 32;
    unsigned long color : 24;
    unsigned long unknown_high : 8;
};

struct DotrPaintWidePacket {
    long header[2];
    struct DotrPaintWideEntry entries[4];
};

/* Callees: argument types as these callers pass them (assumed). */
int ExAssignAlpha(struct DotrPaintPacket *packet, long value);
int Ex3d_RotTransPersAuto(void *source, int base, int arg2,
                          struct DotrPaintWidePacket *packet, void *out);
int ExSortPrim(struct DotrPaintWidePacket *packet, int arg1, int arg2);

/* 0x108f90 PAINT_PIRM_G4 / 0x108ff0 PAINT_PIRM_GT4: transform, and when that returns non-zero
 * sort the packet at the depth func_001a23b0(first result float). Return 0. func_001a23b0 is the
 * float-to-integer helper as ex3d_clip.h declares it. Assumed: the parameter types, as
 * PAINT_PIRM_LG2's. */
extern int func_001a23b0(float value);
int PAINT_PIRM_G4(void *source, struct DotrPaintWidePacket *packet, int arg2, int arg3);
int PAINT_PIRM_GT4(void *source, struct DotrPaintWidePacket *packet, int arg2, int arg3);
/* 0x109050: transform, and when that returns non-zero set both colours to 0xffffff and sort. Returns 0. */
int PAINT_PIRM_LG2(void *source, struct DotrPaintWidePacket *packet, int arg2, int arg3);
/* 0x1091b0: two-slot header, alpha 0x8000000048, colours copied from the two byte arrays. Returns 0. */
int PAINT__2G_SET_IN(struct DotrPaintPacket *packet, unsigned char *first, unsigned char *second);
/* 0x109270: four-slot textured header with flag 0x800, alpha 0x8000000048. Returns 0. */
int PAINT__GT4_SET_IN(struct DotrPaintPacket *packet);
/* 0x1092e0: four-slot header; alpha 0x8000000048 when mode is 0, else 0x8000000042. Returns 0. */
int PAINT__G4_SET_IN(struct DotrPaintPacket *packet, long mode);

/* 0x109100: copies four floats and calls MATRIX_XYZ_ININ33 (0x108950) for
 * four consecutive 16-byte rows with the copy and the two trailing arguments; returns 0.
 * The four-float layout and the int arguments are assumed; the callee (defined in
 * matrix_rot) returns 0 in v0, which this caller ignores. */
struct DotrPaintVector {
    float x, y, z, w;
};
extern int MATRIX_XYZ_ININ33(struct DotrPaintVector *row, struct DotrPaintVector *v, int arg2, int arg3);
int MATRIX_XYZ_ININ_4(struct DotrPaintVector *rows, struct DotrPaintVector *v, int arg2, int arg3);

/* 0x109380: sets the 24-bit field to 0xffffff in all four vertices, last vertex first
 * (one chained assignment). mode 0 addresses 24-byte vertices with the field in their third
 * 64-bit word, any other mode the 16-byte slots of DotrPaintWidePacket. Both layouts are
 * assumptions from the offsets. */
struct DotrPaintGt4Entry {
    long unknown_00;
    long unknown_08;
    unsigned long unknown_low : 32;
    unsigned long color : 24;
    unsigned long unknown_high : 8;
};
struct DotrPaintGt4Packet {
    long header[2];
    struct DotrPaintGt4Entry entries[4];
};
int GT4_Xyz_ZIN(void *packet, int mode);

/* 0x1095d0: writes the screen rectangle of a four-vertex textured packet. The corners are
 * 2048 -/+ half the size plus the position, in 1/16 units (each through the float-to-int helper
 * 0x1a22c0, the position values truncated to 16 bits); every vertex gets z 0xffffff and a zero
 * top byte. Vertex order: (x0,y0), (x1,y0), (x0,y1), (x1,y1). Returns 0. The 16/16/24/8 field
 * layout of the third 64-bit word is an assumption from the stores. The function is built from
 * the gt4_box original-file unit (src/ee/gt4_box/func_001095d0.c); this declaration serves the
 * host test. */
extern int func_001a22c0(float value);
struct DotrPaintXyzEntry {
    long unknown_00;
    long unknown_08;
    unsigned long x : 16;
    unsigned long y : 16;
    unsigned long z : 24;
    unsigned long unknown_high : 8;
};
struct DotrPaintXyzPacket {
    long header[2];
    struct DotrPaintXyzEntry entries[4];
};
int GT4_BOX_IN__2DMAP(struct DotrPaintXyzPacket *packet, struct DotrPaintVector *size_in, struct DotrPaintVector *pos_in);

/* 0x109830 R3D_RRR_XYZ: out = a - b for x, y and z, w copied from a. Assumed: three four-float
 * vectors. */
void R3D_RRR_XYZ(struct DotrPaintVector *out, struct DotrPaintVector *a_in, struct DotrPaintVector *b_in);

/* 0x1094f0: the same 16-byte slots seen as G4_BOX_IN__ writes them: the word at +4, then
 * 16-bit x and y, a 24-bit z and the top byte of the second 64-bit word (member names
 * assumed from GT4_BOX_IN__2DMAP's slots; the first four bytes are not touched). */
struct DotrPaintBoxEntry {
    unsigned char rgba[4];
    int unknown_04;
    unsigned long x : 16;
    unsigned long y : 16;
    unsigned long z : 24;
    unsigned long unknown_high : 8;
};
/* Clears the +4 word of the four slots and sets them to the corners of a 640x480 box
 * centred on 2048 in 12.4 fixed point (x 0x6c00/0x9400, y 0x7100/0x8f00), z 0xffffff,
 * top byte 0. Returns 0. Parameter type assumed (shared with the PAINT__ setters). */
int G4_BOX_IN__(struct DotrPaintPacket *packet);

#endif
