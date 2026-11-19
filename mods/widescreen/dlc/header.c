/* The header at the start of DLC.BIN, which the loader in the executable
 * (src/ee/debug_menu/func_002047f0.c) checks before running anything. DLC.BIN is loaded at
 * 0x2000000, the first byte past the PS2's 32 MB, into the extra memory of PCSX2's "Enable 128MB
 * RAM (Dev Console)" option, which the game itself never touches.
 *
 * The exports are the DLC's functions in a fixed order; the executable's small forwarding
 * functions call them by index. The layout must agree with struct DlcHeader there. */

struct DlcHook {
    unsigned int address;
    void *target;
};

struct DlcHeader {
    unsigned int magic;     /* 'DLC1' */
    char *file_end;         /* end of the bytes in DLC.BIN */
    char *mem_end;          /* end of the memory the DLC uses; the loader zeroes the rest */
    void (*init)(void);     /* called once, after loading */
    void *exports[12];
    struct DlcHook *hooks;  /* installed after init; written by tools/mod.py from mod.json */
    unsigned int models;    /* where the new cards' model slots start in DLC.BIN (bytes), written by
                             * tools/mod.py; the loader reads only the code before them */
    unsigned int live;      /* +0x48: set to 1 by the launcher (tools/launcher.py, over PCSX2's
                             * PINE link) after it wrote new card, effect or boss tables into the
                             * running game; Dlc_LiveCheck then redoes what Dlc_Init derived from
                             * them */
    unsigned int models_sector; /* +0x4c: when not 0, the disc sector where the model slots start
                                 * (the launcher keeps them apart from DLC.BIN, as DLCMDL.BIN) */
};

extern char _dlc_file_end[];
extern char _dlc_mem_end[];
extern struct DlcHook dlc_hooks[];
extern void Dlc_Init(void);
extern int DlcWs_SortPrim(int packet, int source, int table, int z);
extern int DlcWs_SortPrim3D(int packet, int source, int table, int z);
extern int DlcWs_SetAspect(float a, float b);
extern void DlcCard_Init(void);
extern void DlcText_Init(void);

struct DlcHeader dlc_header = {
    0x31434c44,
    _dlc_file_end,
    _dlc_mem_end,
    Dlc_Init,
    {
        (void *)DlcWs_SortPrim,    /* 0 */
        (void *)DlcWs_SortPrim3D,  /* 1 */
        (void *)DlcWs_SetAspect,   /* 2 */
    },
    dlc_hooks,
    0,
    0,
    0,
};

extern void DlcStack_Reset(void);

/* Called often (from DlcWs_SortPrim, every frame something 2D is drawn): after a live edit, the
 * card records and names are made again from the new tables, and cached counts forgotten. */
void Dlc_LiveCheck(void)
{
    if (dlc_header.live) {
        dlc_header.live = 0;
        DlcText_Init();
        DlcCard_Init();
        DlcStack_Reset();
    }
}

/* Runs once when the DLC has been loaded, during the game's start-up (after its file system,
 * graphics, sound and game state are initialised, before the first frame and before the save
 * data is set up), before the hooks are installed. */
void Dlc_Init(void)
{
    extern void DlcMods_Init(void);

    DlcText_Init();
    DlcCard_Init();
    /* Duelists of the Roses Expanded is built on SPEX: it is always applied (modoptions.c), and
     * the card records and names are taken again from the patched game. */
    DlcMods_Init();
}
