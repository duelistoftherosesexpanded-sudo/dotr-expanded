#ifndef DOTR_EE_CAMP_BUN_HYOUJI_H
#define DOTR_EE_CAMP_BUN_HYOUJI_H

/* The campaign map's message box Taku_Camp_BUN_HYOUJI, which draws the camp message window, its
 * text pages, speaker faces and choice cursor from the camp work area at 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Taku_Camp_BUN_HYOUJI reads the camp work area at the literal address 0x1ed0000 through a work
 * view and the camp state at 0x299e10 as bytes. The target build keeps the literal and the symbol;
 * the host build points them at test objects. Assumed: these meanings, from use. */
#ifdef __MWERKS__
extern signed char data_00299e10_bunhyouji;
#define DOTR_BUN_HYOUJI_AT(a) (a)
#else
#include <stdint.h>
extern signed char dotr_camp_bun_hyouji_board[0x100];
#define data_00299e10_bunhyouji (dotr_camp_bun_hyouji_board[0])
union DotrBunHyoujiWork {
    unsigned char bytes[0x5500];
    double align;
};
extern union DotrBunHyoujiWork dotr_camp_bun_hyouji_work;
#define DOTR_BUN_HYOUJI_AT(a) ((intptr_t)dotr_camp_bun_hyouji_work.bytes + ((a) - 0x01ED0000))
#endif

/* Views and declarations: window rectangle, texture box and info, the 16-byte-aligned rect at
 * 0x29aeb0, the G4 template at 0x298af6 and the window template at 0x297b72, and the callee views
 * Taku_G4_Set, SzMsg_DrawRatioX_Pipe, SzMsg_DrawRect_Pipe (only its address is passed) and
 * SzMsg_DrawX_Pipe. The work view names the fields the function uses (message number, step, mode,
 * face requests, levels, flag bytes). Assumed: names from use. */
typedef struct {
    short x0, y0, x1, y1;
    short kind;
} DotrBunWin;

typedef struct {
    int x, y, w, h;
} DotrBunBox;

typedef struct {
    int id;
    DotrBunBox box;
} DotrBunInfo;

typedef struct {
    int v[4];
} __attribute__((aligned(16))) DotrBunRect;

typedef struct {
    short v[19];
} DotrBunG4;

extern int Taku_G4_Set(int, void *, short, short);
extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, void *, void *);
extern void SzMsg_DrawRect_Pipe(void);
extern int SzMsg_DrawX_Pipe(int, int, int, int, void *, void *);
extern DotrBunRect data_0029aeb0_bunhyouji;
extern DotrBunG4 data_00298af6_bunhyouji __attribute__((aligned(2)));
extern DotrBunWin data_00297b72_bunhyouji __attribute__((aligned(2)));


typedef struct {
    unsigned char pad0000[0x4];
    unsigned short f0004;
    unsigned char pad0006[0x5A];
    short f0060;
    unsigned char pad0062[0xA];
    short f006C;
    signed char f006E;
    unsigned char pad006F[0x3];
    signed char f0072;
    unsigned char pad0073[0x119];
    signed char f018C;
    unsigned char pad018D[0x51DF];
    short f536C;
    unsigned char pad536E[0x4];
    signed char f5372;
    signed char f5373;
    signed char f5374;
    signed char f5375;
    signed char f5376;
    unsigned char pad5377[0x1];
    short msg;
    signed char f537A;
    signed char step;
    signed char f537C;
    signed char f537D;
    signed char mode;
    unsigned char pad537F[0x1];
    short tbl[4][4];
    signed char f53A0;
    unsigned char pad53A1[0xA];
    signed char f53AB;
    unsigned char pad53AC[0x8];
    signed char f53B4;
    signed char f53B5;
    short f53B6;
    short f53B8;
    unsigned char pad53BA[0x6];
    signed char f53C0;
    unsigned char pad53C1[0x46];
    signed char f5407;
    signed char f5408;
    unsigned char pad5409[0x1];
    signed char f540A;
    unsigned char pad540B[0x1];
    unsigned char face0[8];
    unsigned char face1[8];
    unsigned char pad541C[0x10];
    short lv[5];
    unsigned char pad5436[0x1];
    signed char f5437;
    unsigned char pad5438[0x8];
    signed char f5440;
    unsigned char pad5441[0x27];
    signed char f5468;
    unsigned char pad5469[0x11];
    signed char f547A;
    unsigned char pad547B[0x6];
    signed char f5481;
    signed char f5482;
    signed char f5483;
} DotrBunWork;

/* Further callees. Several are declared in other groups with other views. Cursors are 32-bit words
 * (int). */
extern int SzEffect_IsUse2(int arg0, int arg1);
extern int SzMsg_LoadFont_Pipe(int arg0, int msgInfoIndex, int stringIndex, int arg3, int arg4);
extern int RhText_SetTextureFlush(int arg0);
extern void *SzMsg_GetInfo(int arg0);
extern int Taku_Camp_BUN_HANTEI(void);
extern int SzMsg_DrawRectG_Pipe(int cursor, int index, int x, int y, void *rect_v, unsigned char *colour);
extern void SzMsg_Free(int arg0);
extern void ShFace_Close(unsigned char *arg0);
extern int Taku_Window_Set(int p, void *rect_arg, unsigned short style, unsigned short mode, unsigned short dx,
                           unsigned short dy, unsigned short depth, int unused, short alpha);

/* 0x2311d0: draws the camp message box; returns the cursor. */
int Taku_Camp_BUN_HYOUJI(int p);

#endif
