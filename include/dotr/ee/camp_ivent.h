#ifndef DOTR_EE_CAMP_IVENT_H
#define DOTR_EE_CAMP_IVENT_H

/* The campaign map's event-sequence state machine Taku_Camp_Ivent, which runs the scripted camp
 * events by event number and step in the camp work area at 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Fields of the 52-byte location records at 0x298ba0 (camp_system.h has a struct view), each
 * declared under its own address: 0x298c32 (record 2 +42), 0x298cd0 (record 5 +44) and 0x298d6a
 * (record 8 +42) as halfwords copied into the route records; 0x298d0c/0x298d14 (record 7) and
 * 0x298ddc/0x298de4 (record 11) as the map position floats copied to the focus position at
 * 0x1ed0040/0x1ed0048. Assumed: these types, from this use. */
extern short data_00298c32_campivent;
extern short data_00298cd0_campivent;
extern float data_00298d0c_campivent;
extern float data_00298d14_campivent;
extern short data_00298d6a_campivent;
extern float data_00298ddc_campivent;
extern float data_00298de4_campivent;
/* 0x34f710: a byte set to 1 before leaving to the movie (assumed signed char). */
extern signed char Move_END_flg_campivent;

/* The camp state at 0x299e10 (camp_system.h: struct DotrCampSystemBoard). The function takes its
 * address as a byte pointer and reaches +0 (side), +1, +2 (bytes), +0x3c (halfword), +0xc2 (the
 * event number, which it also reads as the separate symbol 0x299ed2) and +0xc9 (byte) by offset.
 * The work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated symbol) is
 * reached by its absolute addresses: halfword 0x1ed536c is the step of the running event.
 * MATCHING DEVICE: under the target compiler the two symbols are separate externs and AT(a) is
 * the literal address itself; on the host the two symbols name the same fixture (so writing
 * +0xc2 changes the event number, as in the game) and AT(a) indexes a 0x5500-byte fixture of
 * the work area. */
#ifdef __MWERKS__
extern signed char data_00299e10_campivent;
extern short CutSceneNumber_campivent;
#define DOTR_CAMP_IVENT_AT(a) (a)
#else
union DotrCampIventBoard {
    signed char bytes[0x100];
    short align;
};
extern union DotrCampIventBoard dotr_camp_ivent_board;
#define data_00299e10_campivent (dotr_camp_ivent_board.bytes[0])
#define CutSceneNumber_campivent (*(short *)(dotr_camp_ivent_board.bytes + 0xc2))
union DotrCampIventWork {
    unsigned char bytes[0x5500];
    double align;
};
extern union DotrCampIventWork dotr_camp_ivent_work;
#define DOTR_CAMP_IVENT_AT(a) (dotr_camp_ivent_work.bytes + ((a) - 0x01ED0000))
#endif

/* Callees, under their own names where the prototypes in this tree agree. Views of this caller
 * (they link by address): 0x217980 SzSound_PlayBGM with two ints (small_state.h declares it without
 * parameters; camp_system.h has the same view) and 0x1f93f0 SzView_SetStraightLightColor with an
 * untyped fourth argument (passed 0 here; block_table.h types it). SzModel_LoadMerge (0x1e7f70,
 * object_table) is declared as object_table.h does. Assumed: the parameter names and the result
 * types. */
extern int func_00217980_campivent(int n, int b);
extern int func_001f93f0_campivent(int index, int position, int part, void *in);
extern int SzEffect_Entry(int a, int b, int c, int d, int e);
extern void Taku_Camp_SINEMA_Init(signed char presetNum);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern int SzModel_LoadMerge(int modelType, int no, int force);
extern void SzEffect_SetIntensityOnly(int kind, float value);
extern int SzFile_InqRequestEx(void);
extern int SzData_SetMapEditFlag(int side, int flag);
extern void SzGame_InitMemCardProc(int kind);
extern int SzGame_ActMemCardProc(void);
extern void SzGame_PlayMovieNo(int movie, int a, int b);
extern int SzGame_SetMenuNum(int index, int value);
extern void SzGame_Pop(void);
extern int My_LedarModel_a_set(int dir);

/* 0x21e330 Taku_Camp_Ivent (name from the Japanese build's symbol table). One frame of the
 * running camp event: the event number (board +0xc2) selects the event, the step halfword at
 * 0x1ed536c its step. Assumed: no parameters and no result. */
void Taku_Camp_Ivent(void);

#endif
