#ifndef DOTR_EE_SHSLOT_PARTS_H
#define DOTR_EE_SHSLOT_PARTS_H

/* The slot machine's parts drawer ShSlot_DrawParts, which draws one screen's list of slot parts
 * as textured sprite packets.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* The data symbols carry the _parts suffix here; shslot.h declares the same storage with other views
 * (data_00351228 the packet cursor, 0x351240 the reels, 0x351241 and 0x351242 the line bits,
 * 0x351246 the frame counter, 0x351210 the board). Assumed: these meanings, from use. */
#ifdef __MWERKS__
typedef int DotrShPartsWord;
#else
#include <stdint.h>
typedef intptr_t DotrShPartsWord;
#endif
#ifndef NULL
#define NULL 0
#endif

typedef struct {
    short count;
    unsigned short flags;
    int reserved;
    long long registers;
} DotrShPartsTag;

typedef struct {
    unsigned char r, g, b, a;
    float q;
} DotrShPartsColor;

/* The game's code uses 64-bit bit-fields; the host spells the same little-endian layout in
 * 32-bit units. */
#ifdef __MWERKS__
typedef struct {
    unsigned long long x : 16;
    unsigned long long y : 16;
    unsigned long long z : 24;
    unsigned long long f : 8;
} DotrShPartsPosition;
#else
typedef struct {
    unsigned int x : 16;
    unsigned int y : 16;
    unsigned int z : 24;
    unsigned int f : 8;
} DotrShPartsPosition;
#endif

typedef struct {
    unsigned short u : 14;
    unsigned short : 2;
    unsigned short v : 14;
    unsigned short : 2;
    unsigned int padding;
} DotrShPartsUv;

typedef struct {
    unsigned long long texture;
    DotrShPartsPosition xyz;
} DotrShPartsVertex;

typedef struct {
    unsigned long long header[2];
    DotrShPartsColor color;
    DotrShPartsVertex v0, v1, v2, v3;
    unsigned long long texture[4];
} DotrShPartsPacket;

/* One part: screen rectangle, texel rectangle (u, v to uw, vh), kind and texture index; a list
 * ends at x == -1. The parts table at 0x2b87a0 holds 84 entries (screens start at 0, 0x16 and
 * 0x35); the pulse table at 0x2b8e30 16 bytes. */
typedef struct {
    short x;
    short y;
    short w;
    short h;
    short u;
    short v;
    short uw;
    short vh;
    short kind;
    short tex;
} DotrShPartsItem;

typedef struct {
    DotrShPartsItem items[84];
} DotrShPartsItems;

typedef struct {
    unsigned char values[16];
} DotrShPartsPulse;

/* One reel (0x40 bytes from 0x351240): state byte first. */
typedef struct {
    unsigned char state;
    unsigned char padding01[5];
    unsigned short frame;
    short limit;
    unsigned char enabled;
    unsigned char countdown;
    unsigned char padding0C[6];
    short position;
    short angle;
    short velocity;
    unsigned short center;
    unsigned short symbols[16];
} DotrShPartsReel;

#define DOTR_SHPARTS_CLAMP(wms, wmt, minu, maxu, minv, maxv) \
    ((unsigned long long)(wms) | ((unsigned long long)(wmt) << 2) | ((unsigned long long)(minu) << 4) | ((unsigned long long)(maxu) << 14) | ((unsigned long long)(minv) << 24) | \
     ((unsigned long long)(maxv) << 34))

extern DotrShPartsWord data_00351228_parts;
extern DotrShPartsReel data_00351240_parts[];
extern unsigned char data_00351241_parts;
extern unsigned char CanStillGet3InARow_parts;
extern unsigned short data_00351246_parts;

/* Callee prototypes, with the packet cursor and texture handles as DotrShPartsWord.
 * SzText_InitClutManage and ExSortPrim_Pipe are declared inside the function without a
 * prototype. */
extern DotrShPartsWord SzText_DoRequest_Pipe(DotrShPartsWord arg0, int arg1, int arg2, int *arg3);
extern DotrShPartsWord SzText_LoadRequest(int arg0, int arg1);
extern int SzText_FreeRequest(int textureFlags, int textureId);
extern int ExAssignAlpha(void *arg0, long long arg1);
extern int ExAssignTexture(void *arg0, void *arg1, int arg2);
extern int ExAssignClamp(void *arg0, long long arg1);
extern int RhBase_GetUserGsAddress(void);
extern DotrShPartsWord RhText_SetTextureFlush(DotrShPartsWord arg0);

/* 0x2818a0 ShSlot_DrawParts(screen): loads the two slot textures (group 9), then draws the
 * screen's part list (0, 1, 2 start at 0, 0x16, 0x35) as sprites at (x + 0x6c0, y + 0x720) << 4;
 * the kind picks the clamp, lights line parts by the line bits and the frame pulse, hides
 * unplayed lines and draws the level lamps twice (alpha 0x44 and 0x49). */
void ShSlot_DrawParts(int arg0);

#endif
