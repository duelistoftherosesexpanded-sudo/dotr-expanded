#ifndef DOTR_EE_SH_FACE_PARTS_H
#define DOTR_EE_SH_FACE_PARTS_H

/* Face animation helpers that read the face index table at 0x29d488 and build one face sprite
 * packet.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Views: the face index table at 0x29d488 holds one pointer per face resource; each resource
 * starts with a word, a pointer to 8-byte index rows (byte 1 the first record) and a pointer to
 * 8-byte records whose first byte links to the next record (0 or 0xff ends a chain; byte 6 of a
 * record is the animation chain head). Assumed: these meanings, from use. */
typedef struct DotrShFaceQuad {
    short x;
    short y;
    short z;
    short w;
} DotrShFaceQuad;

typedef struct DotrShFaceFixRecord {
    short x;
    short y;
    short width;
    short height;
} DotrShFaceFixRecord;

typedef struct DotrShFaceFixData {
    int unk0;
    unsigned char (*indices)[8];
    DotrShFaceFixRecord *records;
} DotrShFaceFixData;

typedef struct DotrShFaceAnmRecord {
    short x;
    short y;
    short width;
    short height;
} DotrShFaceAnmRecord;

typedef struct DotrShFaceAnmData {
    int unk0;
    unsigned char (*indices)[8];
    union {
        DotrShFaceAnmRecord *records;
        unsigned char (*links)[8];
    } u;
} DotrShFaceAnmData;

/* 0x245a30 ShAnime_GetFixInfo(resource, object, steps, out): starts at the object's index row
 * byte 1, follows up to `steps` record links (stopping at a link of 0 or 0xff) and copies that
 * 8-byte record to out. */
void ShAnime_GetFixInfo(int arg0, int arg1, int arg2, void *arg3);
/* 0x245ae0 ShAnime_GetAnmDat(resource, object, steps, frame, out): as GetFixInfo for the record
 * chain, then from that record's byte 6 follows up to `frame` links and copies that record. */
void ShAnime_GetAnmDat(int arg0, int arg1, int arg2, int arg3, void *arg4);

/* GS packet view for ShFace_Prim: a GIF tag (count 9, flags 0x24 | 0x800, registers
 * 0x424242421), rgbaq, four uv/xyz vertices and four state words. The game's code uses 64-bit
 * bit-fields; the host spells the same little-endian layout in 32-bit units. */
typedef struct DotrShFaceTag {
    unsigned short count;
    unsigned short flags;
    unsigned int padding;
    unsigned long long registers;
} DotrShFaceTag;

#ifdef __MWERKS__
typedef struct DotrShFaceUv {
    unsigned long long u : 14;
    unsigned long long pad0 : 2;
    unsigned long long v : 14;
    unsigned long long pad1 : 34;
} DotrShFaceUv;

typedef struct DotrShFaceXyz {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long fog : 8;
} DotrShFaceXyz;
#else
typedef struct DotrShFaceUv {
    unsigned int u : 14;
    unsigned int pad0 : 2;
    unsigned int v : 14;
    unsigned int pad1 : 2;
    unsigned int pad2;
} DotrShFaceUv;

typedef struct DotrShFaceXyz {
    unsigned int x : 16;
    unsigned int y : 16;
    unsigned int z : 24;
    unsigned int fog : 8;
} DotrShFaceXyz;
#endif

typedef struct DotrShFaceVertex {
    unsigned long long uv;
    DotrShFaceXyz xyz;
} DotrShFaceVertex;

typedef struct DotrShFacePacket {
    unsigned long long tag[2];
    unsigned char r, g, b, a;
    float q;
    DotrShFaceVertex vertex[4];
    unsigned long long state[4];
} DotrShFacePacket;

/* Face object view: position halfwords at +0x2c/+0x2e and the texture pointer at +0x30. */
typedef struct DotrShFaceObject {
    unsigned char pad0[0x2C];
    unsigned short x;
    unsigned short y;
    void *texture;
} DotrShFaceObject;

/* Packet word type: the VIF cursor at 0x350fc0 and the packet address are 32-bit words on the
 * target; the host uses intptr_t. */
#ifdef __MWERKS__
typedef int DotrShFaceWord;
#else
#include <stdint.h>
typedef intptr_t DotrShFaceWord;
#endif

/* Callee prototypes (defined in ex_texture, draw_state and
 * sort_prims). */
extern int ExAssignTexture(void *packet, void *texture, int a);
extern int ExAssignClamp(void *packet, long long value);
extern int ExAssignAlpha(void *packet, long long value);
extern DotrShFaceWord ExSortPrim_Pipe(DotrShFaceWord cursor, DotrShFaceWord packet, int a2, int a3);

/* 0x245db0 ShFace_Prim(object, rect, dx, dy): builds a textured sprite packet for the face
 * rectangle (rect halfwords v0, u0, v1, u1) at the object's position + (0x6c0, 0x720) plus
 * (dx, dy), z 0xfffff8, with texture, clamp and alpha 0x44, and sorts it into the VIF cursor at
 * 0x350fc0 when that is not 0. */
void ShFace_Prim(void *arg0, void *arg1, int arg2, int arg3);

#endif
