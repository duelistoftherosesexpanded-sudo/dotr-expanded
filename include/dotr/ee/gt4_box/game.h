#ifndef DOTR_EE_GT4_BOX_GAME_H
#define DOTR_EE_GT4_BOX_GAME_H

/* Declarations for the GT4_BOX_IN__2DMAP original-file unit (src/ee/gt4_box/func_001095d0.c;
 * docs/modules/other.md). Only the prototypes the unit uses are kept. Types and signatures are
 * assumed views, not recovered declarations. This file holds the globals and types. */


#include "dotr/ee/gt4_box.h"












extern s32 *g_SDValue_ptr;




extern u8 data_01fe0000;


typedef struct Duel_00317490_s {
    u8 players[2][0x350];          /* 0x000: one record per player (callers use their own views) */
    u8 pad6A0[0x6D0 - 0x6A0];
    struct {
        u8 pad00[0x20];
        u16 ids[3];                /* 0x20: the three cards picked */
    } *pick;                       /* 0x6D0 */
    u8 pad6D4[0x6D8 - 0x6D4];
    u16 *list;                     /* 0x6D8: the trunk list */
    u16 *list2;                    /* 0x6DC: the deck list (40) */
    u8 pad6E0[0xC58 - 0x6E0];
    s32 depth;                     /* 0xC58: GS z of the duel screen sprites */
    u8 padC5C[0xC86 - 0xC5C];
    u16 wave1;                     /* 0xC86 */
    u16 wave2;                     /* 0xC88 */
    u16 offset;                    /* 0xC8A */
    u16 count;                     /* 0xC8C */
    u16 offset2;                   /* 0xC8E */
    u16 count2;                    /* 0xC90 */
    s16 scroll;                    /* 0xC92 */
    s16 scrollTo;                  /* 0xC94 */
    u16 idle;                      /* 0xC96 */
    u8 padC98[0xCB6 - 0xC98];
    u8 sort[3];                    /* 0xCB6 */
    u8 sort2[3];                   /* 0xCB9 */
    u8 sort3;                      /* 0xCBC */
    u8 sort4;                      /* 0xCBD */
    u8 pstate[4];                  /* 0xCBE: per-player state bytes, [player] */
    char code[8];                  /* 0xCC2: password digits */
    u8 padCCA[0xD32 - 0xCCA];
    u8 dirty;                      /* 0xD32 */
    u8 padD33[0xD35 - 0xD33];
    u8 prevMode;                   /* 0xD35 */
    u8 sel;                        /* 0xD36 */
    u8 padD37[0xD57 - 0xD37];
    u8 step;                       /* 0xD57 */
    u8 padD58[0xD5C - 0xD58];
    u8 player;                     /* 0xD5C */
    u8 mode;                       /* 0xD5D */
    u8 row;                        /* 0xD5E */
    u8 row2;                       /* 0xD5F */
    u8 digit[3];                   /* 0xD60 */
    u8 hold;                       /* 0xD63 */
    struct {
        s8 dir1 : 2;               /* bits 0-1 */
        s8 dir2 : 2;               /* bits 2-3 */
        s8 b4 : 1;
        s8 b5 : 1;
        s8 b6 : 1;
        s8 b7 : 1;
    } dirs;                        /* 0xD64 */
    struct {
        s8 b0 : 1;
        s8 b1 : 1;
        s8 b2 : 1;
        s8 b3 : 1;
        s8 b4 : 1;
        s8 b5 : 1;
        s8 b6 : 1;
        s8 b7 : 1;
    } f65;                         /* 0xD65: one-bit flags (bit 0 first) */
    struct {
        s8 b0 : 1;
        u8 b1 : 1;                 /* read unsigned (lbu/dsrl32) */
        s8 b2 : 1;
        s8 b3 : 1;
        s8 b4 : 1;
        s8 b5 : 1;
        s8 b6 : 1;
        s8 b7 : 1;                 /* SzDuel_GetWindowDisp returns it */
    } f66;                         /* 0xD66 */
    struct {
        s8 b0 : 1;
        s8 b1 : 1;
        s8 b2 : 1;
        s8 b3 : 1;
        s8 b4 : 1;
        s8 b5 : 1;
        s8 b6 : 1;
        s8 b7 : 1;
    } f67;                         /* 0xD67 */
    u8 padD68[0xD70 - 0xD68];
} Duel_00317490_s;

/* The duel state: some functions read it as bytes (.b), others by field (.s). */
typedef union Duel_00317490 {
    u8 b[0xD70];
    Duel_00317490_s s;
} Duel_00317490;

extern Duel_00317490 data_00317490;


typedef __int128 Q128_0020F290;
extern u8 data_0032a5e0[];
extern Q128_0020F290 data_0032a600;

extern u8 data_003506c8[];





extern u8 data_00325e80[];
extern u8 data_00325f50[];
extern s32 data_003261bc;


extern char data_002b90da[];
extern char data_002b9102[];
extern char data_002b9112[];
extern char data_002b9127[];
extern char data_002b9135[];

extern s32 gSzGame_gl_word;

extern s32 data_00317254;

extern s32 data_00317b68;

extern s32 data_00317b6c;


extern s32 data_0031be98;

extern u8 data_003259c0[];

extern s32 data_0032618c;
extern s32 data_00326190;

extern s32 data_003261b0;
extern s32 data_003261b4;


extern s32 isCountVblank_word;
extern s32 frd_word;




extern s32 data_0031be9c;


extern u8 data_002d0655[];

extern volatile s32 data_00351170;

extern u8 EnchantId[];

extern s16 data_00322990;
extern u16 data_00322992;
extern s16 data_00322994;

extern u8 data_00325cc0[];

extern u8 data_00325d38[];

extern s32 gRhView_nStackIndex_word;

extern s32 gRhZSort_zsl_word;
extern s32 data_0032f784;
extern s32 data_0032f788;
extern s32 data_0032f78c;

extern char data_0030f8e9;



extern u8 data_0029d586[];


extern u8 gSzDuel_aCardControl_bytes[];
extern u16 gSzDuel_CardDefaultCamDist[];





extern s64 gExPrim_aqwAlpha_qwords[];


extern s8 Move_END_flg_byte;
extern s32 isWithAudio_word;
extern u8 data_00350620[];



extern u8 gSzDuel_aCardRequest_bytes[];


extern s32 g_SDBuffer_word;


extern s32 gSzView_hView_words[];








typedef void (*Callback_00286910)(void);











extern u8 data_00350600[];
extern volatile s32 data_0035060c;


extern u16 AlternateCardIds[];
extern u8 data_00318e60[];







extern u8 EnchMatch_bytes[];








extern char data_0030f354[];
extern s32 data_003503b8;
extern s32 controller_val_word;
extern u8 data_00350680[];
extern u8 data_00350740[];
extern s32 readBuf_word;

extern u16 gAnmPat_Bakura[];


extern u16 gSzData_ConstDeck[51][41];

extern u8 data_00299f90[];


extern u8 data_002b90b0[];



extern char data_002bf002[];


extern char data_002bf040;


extern u32 FusionDat_words[];







extern u64 g_aqwExDitherMatrix_qwords[];




extern s32 data_00292468;

extern u8 DeckBuildingSide;

extern s8 data_00299e10;
extern s16 SelectedBattleArena;














extern char data_002b90c0[];
extern s32 data_0030ff58;
typedef struct { s32 src; s32 offset; s32 size; s32 position; s32 left; } cdstream_t;
extern cdstream_t data_0030ffa0;




extern s32 data_00325dd0;
extern s32 data_00325e2c;



extern s32 voBufData_word;
extern s32 voBufTag_word;
extern s32 viBufTag_word;
extern s32 mpegWork_word;
extern s32 defStack_word;
extern s32 audioBuff_word;
extern s32 viBufData_word;
extern s32 videoDecStack_word;
extern s32 timeStamp_word;
extern s32 isWithInter_word;
extern u8 data_003503c0[];



extern f32 data_00294050[];


extern u8 data_00318280[];









extern s8 data_0034f700;
extern s8 data_0034f708;



extern s32 data_003180e8;

extern u8 data_0031747a;
extern s32 data_003180e4;
extern s32 data_0031810c;
extern s8 data_0031747b;


extern u8 turnOwner;
extern u8 turnFirst;

extern u8 turnCount;
extern s8 HandCursorState;
extern s8 MsgPopUpID;
extern s32 data_0031ab90;
extern s8 MenuVisualToDisplay;

extern s32 data_0031bb10;




extern s8 data_0031bea2;

extern u8 data_0031bea0;
extern s32 data_0031c2e4;
extern s32 data_003261d0;


extern s8 data_0031bea1;
extern s32 data_0032a220;

extern s8 data_00299eda;

extern s16 winCount;
extern char data_0030f3ae[];


extern s32 gSzView_pCamera_word;


extern u8 ScreenFadeAmount;


extern char data_002bf00b;

extern s32 data_0028ff10[];
extern u8 data_002be809[];
extern u8 data_002be80f[];





extern char data_002bef67[];















extern char data_002bef98[];
extern char data_002bef6f[];
extern char data_002bef73[];
extern char data_002be944[];
extern u8 data_003181c6;






extern u8 data_00325f00[];
extern Q128_0020F290 data_00325ef0;
extern u64 data_00326180;











extern f32 data_00291000;
extern f32 data_0029100c;
extern f32 data_00291020;
extern f32 data_002910b0;
extern f32 data_002910b8;




extern s32 (*data_00291330[])(s32, s32);

extern u8 gSzDuel_NameIndex0[];
extern u8 gSzDuel_NameIndex1[];

extern u8 data_002909b8[];
extern u8 WhoGoesFirst;





extern f32 data_00298ba4;


extern u8 InvertView_bytes[];


extern u8 data_0032a784[];


extern f32 data_00325d50;
extern f32 data_00325d54;
extern f32 data_00325d58;
extern f32 data_00325d60;
extern f32 data_00325d64;
extern f32 data_00325d68;
extern f32 data_00325d70;
extern f32 data_00325d74;
extern f32 data_00325d78;
extern f32 data_00325d90;
extern f32 data_00325d94;
extern f32 data_00325d98;
extern f32 data_00325da0;
extern f32 data_00325da4;
extern f32 data_00325da8;
extern f32 data_00325db0;
extern f32 data_00325db4;
extern f32 data_00325db8;

extern s16 data_00298260[];
extern s16 data_002982f8[];
extern s16 data_00298b50[];
extern s16 data_00298b76[];
extern s8 data_0030f102[];
extern s8 data_0030f107[];
extern s8 data_0030f10c[];
extern s8 data_0030f113[];
extern s8 data_0030f118[];




extern char data_0028c230[];



extern s32 data_0028fa98;

extern s32 data_0028d958;

extern s8 data_002bca98;

extern s32 data_0028dc90;

extern s32 data_00310e98;

extern s32 data_0028d8f0;

extern s32 data_002bd230;




extern s32 data_0028caa4;

extern u8 data_002bcb08[];

extern u8 data_002bcb38[];


extern u8 data_002bcb50[];


extern u8 data_002bcb88[];





















extern s32 data_0028f790;
extern s32 data_0028f794;
extern s32 data_00315c40;
extern s32 data_00317200;

extern s32 data_0028ca10;
extern s32 data_00310e94;

extern s32 data_0028caa0;
extern s32 data_00313ba8;


extern s32 data_0028cac0;
extern s32 data_00314028;


extern s32 data_0028d108;








extern s32 *data_00315c68;
extern s32 *data_00315c6c;
extern s32 *data_00315c70;



extern s32 data_0028cdf0;
extern s32 func_00184080;


extern s32 data_00315500;

extern s32 data_002bc900;
extern s32 data_002bc920;

extern s32 data_0028ca18;

extern s8 data_002bd0a0;
extern s32 data_00315cc0;



extern s32 data_0028dce8;
extern s32 data_0028dd40;
extern s32 data_0028ee90;
extern s32 data_0028dcec;
extern s32 data_0028eec0;
extern s32 data_0028f708;


extern s32 data_002bc8a8;
extern s32 data_002bc8d0;

extern PadSlot_00192A88 data_003159d0[2][4];
extern s32 data_00315bc0;

extern s32 data_00310fc0;
extern s32 data_0028da50;



typedef union { f32 value; u32 word; } fshape;

/* libgcc fp-bit.c: Cygnus built it for the r5900 with US_SOFTWARE_GOFAST and
   FLOAT_BIT_ORDER_MISMATCH, once for float (NO_NANS) and once for double. */
typedef enum { CLASS_SNAN, CLASS_QNAN, CLASS_ZERO, CLASS_NUMBER, CLASS_INFINITY } fp_class_type;
typedef struct { fp_class_type class; u32 sign; s32 normal_exp; u32 fraction; } fp_number_f;
typedef struct { fp_class_type class; u32 sign; s32 normal_exp; u64 fraction; } fp_number_d;
typedef union {
    f32 value;
    u32 value_raw;
#if defined(__GNUC__) && !defined(__MWERKS__)
    struct {
        u32 fraction : 23 __attribute__((packed));
        u32 exp : 8 __attribute__((packed));
        u32 sign : 1 __attribute__((packed));
    } bits;
#endif
} fp_union_f;
typedef union {
    double value;
    u64 value_raw;
#if defined(__GNUC__) && !defined(__MWERKS__)
    struct {
        u64 fraction : 52 __attribute__((packed));
        u32 exp : 11 __attribute__((packed));
        u32 sign : 1 __attribute__((packed));
    } bits;
#endif
} fp_union_d;
extern fp_number_d data_002bdf30; /* __thenan_df */
#if defined(__GNUC__) && !defined(__MWERKS__)
/* fp-bit's INLINE predicates: a test of their int result compiles to xori/beqz */
static __inline__ s32 fp_isnan_f(fp_number_f *x) { return x->class == CLASS_SNAN || x->class == CLASS_QNAN; }
static __inline__ s32 fp_isinf_f(fp_number_f *x) { return x->class == CLASS_INFINITY; }
static __inline__ s32 fp_iszero_f(fp_number_f *x) { return x->class == CLASS_ZERO; }
static __inline__ s32 fp_isnan_d(fp_number_d *x) { return x->class == CLASS_SNAN || x->class == CLASS_QNAN; }
static __inline__ s32 fp_isinf_d(fp_number_d *x) { return x->class == CLASS_INFINITY; }
static __inline__ s32 fp_iszero_d(fp_number_d *x) { return x->class == CLASS_ZERO; }
#endif


extern s32 data_00310980;


extern s32 data_0028caa8;
extern s32 data_0028caac;

extern s32 data_00310920;
extern s32 data_002bc1e8;
extern s32 data_0028f730;
extern s32 data_00315958;
extern s32 data_0028dcd0;
extern s32 data_002bce58;
extern s32 data_002bce88;

extern s32 data_002bce70;

extern s32 data_00313e00;
extern s32 data_00314000;
extern s32 data_002bc9f8;
extern s32 data_0028f748;


extern char data_002bca18[];

extern s32 data_001842c0;
extern s32 data_0028cdf8;

extern s32 data_0028d930;

extern s32 data_0028f750;
extern u8 data_002bd000;
extern s32 data_0028f770;

extern s32 data_0028f300;

extern s32 data_0028c24c;
extern s32 *data_0028cac4;



typedef struct { void (*fn)(s32); s32 arg; } hcall_t;
extern s32 *data_0028d11c;

extern void (*data_0028ca0c)(s32);

extern s32 data_00316200;


typedef struct { u8 a; u8 b; } ringent_t;
typedef struct { s32 x0; u32 count; ringent_t e[0x200]; } ring_t;
extern s32 data_0028ca00;
extern s32 data_00310510;
extern s32 data_00310518;


extern s32 *data_0028cab0;
extern s32 data_0028dcfc;
extern s32 data_003156c4;
extern s32 data_003156c8;
extern s32 data_0028dce4;

extern s32 data_0028c254;

extern s32 data_003512f8;

typedef struct { u8 pad0[0xC]; s16 _flags; u8 pad1[0x58 - 0xE]; } nl_FILE;
typedef struct nl_glue { struct nl_glue *_next; s32 _niobs; nl_FILE *_iobs; } nl_glue;
typedef struct { u8 pad0[0x58]; } nl_mFILE;
typedef struct nl_mglue { struct nl_mglue *_next; s32 _niobs; nl_mFILE *_iobs; } nl_mglue;

extern s8 data_002bd0c1[];


typedef union { double value; struct { u32 lsw; u32 msw; } parts; } nl_dshape;

extern s32 data_002bc828;

extern s32 data_002bc9d0;

extern f32 data_002be5d0[]; /* fdlibm kf_tan.c T[13] */

extern s32 data_002bc7c0;
extern s32 data_002bc7e8;

extern s32 data_002be4c4[];  /* fdlibm ef_rem_pio2.c npio2_hw[] - 1 */
extern s32 data_002be1b0[];  /* two_over_pi[] */

extern s32 data_00315ca0;
extern s32 data_00315cf0;


extern f32 data_002be120[]; /* fdlibm ef_fmod.c Zero[] = { 0.0, -0.0 } */

extern s32 data_00316140;

extern f32 data_002be608[]; /* fdlibm sf_atan.c atanhi[4] */
extern f32 data_002be618[]; /* atanlo[4] */
extern f32 data_002be628[]; /* aT[11] */

extern s32 data_0028dcd4;
extern s32 data_0028dce0;
extern volatile s32 data_0028dcf0;
extern s32 data_0028dd14;
extern volatile s32 data_0028dd18;
extern char data_002bcbe8[];
extern void (*data_003156c0)(s32);
extern s32 data_003156cc;




extern f32 data_002be128[]; /* fdlibm ef_pow.c bp[2] = { 1.0, 1.5 } */
extern f32 data_002be130[]; /* dp_h[2] */
extern f32 data_002be138[]; /* dp_l[2] */
extern f32 data_002be1ac;   /* huge * huge, folded (the compiler kept it in rodata) */

extern s32 data_002be570[]; /* fdlibm kf_rem_pio2.c init_jk[] = { 4, 7, 9 } */
extern f32 data_002be580[]; /* PIo2[11] */


typedef struct i2b_Bigint { struct i2b_Bigint *next; s32 k, maxwds, sign, wds; u32 x[1]; } i2b_Bigint;


extern hcall_t data_00313c00;

extern s32 data_0028ff04;
extern s32 data_0028ff00;
extern s32 data_0028d114;

extern s32 data_002bca78;

typedef struct mcmp_Bigint { struct mcmp_Bigint *next; s32 k, maxwds, sign, wds; u32 x[1]; } mcmp_Bigint;

extern double data_002bd288[]; /* mprec tens[] */

typedef struct vf_reent { u8 pad[0x38]; s32 __sdidinit; } vf_reent;
typedef struct vf_FILE { u8 pad[0x54]; vf_reent *_data; } vf_FILE;

typedef struct sread_FILE { u8 pad0[0xC]; s16 _flags; s16 _file; u8 pad1[0x40]; s32 _offset; s32 *_data; } sread_FILE;

typedef struct sseek_FILE { u8 pad0[0xC]; s16 _flags; s16 _file; u8 pad1[0x40]; s32 _offset; s32 *_data; } sseek_FILE;

typedef struct sprr_FILE { u8 *_p; s32 _r; s32 _w; s16 _flags; s16 _file; u8 *_bf_base; s32 _bf_size; s32 _lbfsize; void *_cookie; void *_read; void *_write; void *_seek; void *_close; u8 *_ub_base; s32 _ub_size; u8 *_up; s32 _ur; u8 _ubuf[3]; u8 _nbuf[1]; u8 *_lb_base; s32 _lb_size; s32 _blksize; s32 _offset; void *_data; } sprr_FILE;

typedef struct spr_FILE { u8 *_p; s32 _r; s32 _w; s16 _flags; s16 _file; u8 *_bf_base; s32 _bf_size; s32 _lbfsize; void *_cookie; void *_read; void *_write; void *_seek; void *_close; u8 *_ub_base; s32 _ub_size; u8 *_up; s32 _ur; u8 _ubuf[3]; u8 _nbuf[1]; u8 *_lb_base; s32 _lb_size; s32 _blksize; s32 _offset; void *_data; } spr_FILE;

typedef struct ssc_FILE { u8 *_p; s32 _r; s32 _w; s16 _flags; s16 _file; u8 *_bf_base; s32 _bf_size; s32 _lbfsize; void *_cookie; void *_read; void *_write; void *_seek; void *_close; u8 *_ub_base; s32 _ub_size; u8 *_up; s32 _ur; u8 _ubuf[3]; u8 _nbuf[1]; u8 *_lb_base; s32 _lb_size; s32 _blksize; s32 _offset; void *_data; } ssc_FILE;

extern char data_002bd270[]; /* "C" */
extern char data_002bd260[]; /* "" */


extern char ***data_0028ff08; /* p_environ */

typedef struct cal_chunk { u32 prev_size; u32 size; } cal_chunk;

extern s32 data_00312840;
extern s32 data_00313e08;
extern s32 data_00315d04;

typedef union { double d; u32 i[2]; } ratio_du;

typedef union { double d; u32 i[2]; } ulp_du;

extern s32 data_0028d970;

typedef struct fr_FILE { u8 *_p; s32 _r; } fr_FILE;



extern s32 data_0028db80;
extern s32 data_0028dbc0;
extern s32 data_002bcaa8;


typedef struct p5_reent { u8 pad0[0x48]; i2b_Bigint *_p5s; } p5_reent;
extern s32 data_002bd274[];


typedef union b2d_du { double d; u32 i[2]; } b2d_du;

extern s32 data_002bc7a0;

typedef union cvt_du { double d; u32 i[2]; } cvt_du;





extern s32 data_002bccd0;

extern s32 data_002bccc0;


extern s32 data_002bc940;


extern s32 data_002bcac0;
extern s32 data_002bcae8;

extern s32 data_002bc808;

extern s8 data_002bc2d8;



extern s32 data_0028d118;


extern s32 data_0028cab8;
extern s32 data_00313c40;
extern s32 data_00313c80;
extern s32 data_00313cc0;

extern s32 data_00313500;
extern s32 data_00313b80;




extern s32 data_0028ca08;
extern char data_00310d40[];

extern __int128 data_0028d9f0[];


extern s32 data_0028c250;
typedef struct { void *func; void *data; } SifCmdHandlerData;
extern SifCmdHandlerData *data_00310ea4;
extern SifCmdHandlerData *data_00310eac;

extern const char data_002bd0c0[];
typedef struct su_reent { s32 _errno; } su_reent;

typedef struct sl_reent { s32 _errno; } sl_reent;

extern u32 data_0028da78[];

extern s32 data_0028f74c;
extern char data_002bcee0[];

extern char data_002bc670[];

extern s32 data_00317240;
extern char **data_0028f79c;

extern char data_002b9141[];
extern char data_002b9168[];
extern char data_0030ff60[];

extern u8 data_003261f0;

extern u8 gSzDuel_dt_bytes[];

extern s32 isFrameEnd_word;

extern s32 data_00326a20;


extern s32 vecNormal;

extern f32 data_01e01a80_b505;
extern f32 data_01e01a88_b505;

extern f32 data_01e01e00_b350;
extern f32 data_01e01e08_b350;

extern f32 data_01e03d00_b267;
extern f32 data_01e03d08_b267;
extern f32 data_01e02400_b850;
extern f32 data_01e02408_b850;

extern f32 data_01e03c00_b378;
extern f32 data_01e03c08_b378;
extern f32 data_01e03780_b195;
extern f32 data_01e03788_b195;
extern f32 data_01e03580_b266;
extern f32 data_01e03588_b266;
extern f32 data_01e02780_b294;
extern f32 data_01e02788_b294;

extern f32 data_01e02c80_b484;
extern f32 data_01e02c88_b484;

extern f32 data_01e02d00_b477;
extern f32 data_01e02d08_b477;
extern f32 data_01e03180_b397;
extern f32 data_01e03188_b397;

extern f32 data_01e02e00_b284;
extern f32 data_01e02e08_b284;

extern f32 data_01e03c00_b192;
extern f32 data_01e03c08_b192;
extern f32 data_01e03000_b566;
extern f32 data_01e03008_b566;
extern f32 data_01e03380_b513;
extern f32 data_01e03388_b513;
extern f32 data_01e02e00_b552;
extern f32 data_01e02e08_b552;
extern f32 data_01e03180_b297;
extern f32 data_01e03188_b297;

extern f32 data_01e03b80_b052;
extern f32 data_01e03b88_b052;

extern f32 data_01e03d00_b567;
extern f32 data_01e03d08_b567;
extern f32 data_01e04000_b555;
extern f32 data_01e04008_b555;

extern f32 data_01e03e80_b018;
extern f32 data_01e03e88_b018;
extern f32 data_01e03e80_b019;
extern f32 data_01e03e88_b019;
extern f32 data_01e04000_b080;
extern f32 data_01e04008_b080;
extern f32 data_01e04480_b734;
extern f32 data_01e04488_b734;
extern f32 data_01e04f00_b430;
extern f32 data_01e04f08_b430;

extern f32 data_01e03f80_b393;
extern f32 data_01e03f88_b393;

extern f32 data_01e04300_b046;
extern f32 data_01e04308_b046;

extern f32 data_01e05100_b225;
extern f32 data_01e05108_b225;

extern f32 data_01e04680_b527;
extern f32 data_01e04688_b527;
extern f32 data_01e04580_b257;
extern f32 data_01e04588_b257;
extern f32 data_01e05780_b582;
extern f32 data_01e05788_b582;
extern f32 data_01e04800_b476;
extern f32 data_01e04808_b476;

extern f32 data_01e05b80_b258;
extern f32 data_01e05b88_b258;
extern f32 data_01e05480_b573;
extern f32 data_01e05488_b573;
extern f32 data_01e05c80_b218;
extern f32 data_01e05c88_b218;

extern f32 data_01e04b00_b207;
extern f32 data_01e04b08_b207;

extern f32 data_01e05600_b718;
extern f32 data_01e05608_b718;

extern f32 data_01e05600_b383;
extern f32 data_01e05608_b383;
extern f32 data_01e05600_b148;
extern f32 data_01e05608_b148;
extern f32 data_01e06100_b709;
extern f32 data_01e06108_b709;
extern f32 data_01e05a00_b417;
extern f32 data_01e05a08_b417;
extern f32 data_01e06a00_b357;
extern f32 data_01e06a08_b357;

extern u8 data_00912000[];
extern char data_002b8f00[], data_002b8f05[], data_002b8f07[], data_002b8f0d[], data_002b8f13[], data_002b8f18[];
extern char data_002b8f1f[], data_002b8f27[], data_002b8f2c[], data_002b8f32[];

extern s32 data_002bc768;

s32 Taku_Camp_Ivent_Window(s32);
s32 Taku_Camp_Ivent_Ga(s32);
s32 Taku_Camp_BUN_HYOUJI(s32);
s32 Taku_Camp_ST_WINDOW(s32);
s32 Taku_Camp_TEKI_WINDOW(s32);
s32 Taku_Camp_TIKEI_WINDOW(s32);
s32 Taku_Camp_BUN_DARE_b(s32, s8);
s32 Taku_Camp_BOTAN_MATI_b(s32);

extern s32 data_002bc858;

typedef struct { f32 x, y, z, w; } Vec4_00109830;

extern s32 data_00315640[];
extern s32 data_00315680;
extern s32 data_0028dca4;
extern s32 data_0028dca8;
extern s32 data_0028dcac;
extern s32 data_0028dcb0;
extern s32 data_0028dcb4;
extern s32 data_0028dcb8;

extern char gSzDuel_aicolMsg_bytes[];


#endif
