/* The DLC loader, and the executable's end of the widescreen mod.
 *
 * DLC.BIN is a file on the disc holding code and data built from mods/<name>/dlc/ (tools/mod.py),
 * loaded once at start-up to 0x2000000: the first byte past the PS2's 32 MB, in the extra memory of
 * PCSX2's "Enable 128MB RAM (Dev Console)" option, which the game never touches. There it has no
 * size limit to speak of (96 MB), unlike code squeezed into the executable.
 *
 * Dlc_Boot takes the place of the call to SzDebug_Init in Main_Init (a redirect in mod.json), so
 * it runs after the file system, graphics, sound and game state are set up and before the first
 * frame. tools/mod.py writes where DLC.BIN starts on the disc and its size into DLC_INFO in the
 * disc's copy of the executable. When the extra memory is missing (the option is off), DLC.BIN is
 * missing or its header is wrong, nothing is loaded and the game runs as without the DLC: the
 * forwarding functions below then do what the original code did.
 *
 * After loading, the DLC's init runs, then its hooks are installed: each names a function of the
 * game whose first instructions become a jump into the DLC, which then has a full replacement for
 * it (written from the decompiled source). Without the DLC no hook is installed and every game
 * function is the original.
 *
 * The code lives in the place of lSzDebug_ActEffect, the effect test page of the developers'
 * debug menu (reached only from SzDebug_Act with the menu's effect flag set), reduced to an
 * empty function. */

typedef unsigned int u32;

/* A function of the game the DLC takes over: its first two instructions become a jump to the
 * DLC's version (and the delay slot's nop). */
struct DlcHook {
    u32 address;
    void *target;
};

struct DlcHeader {
    u32 magic;              /* 'DLC1' */
    char *file_end;         /* end of the bytes in DLC.BIN */
    char *mem_end;          /* end of the memory the DLC uses; zeroed past file_end */
    void (*init)(void);     /* called once, after loading */
    void *exports[12];      /* the DLC's functions, in the order of mods/<name>/dlc/header.c */
    struct DlcHook *hooks;  /* ended by an address of 0 */
};

#define DLC_MAGIC 0x31434c44
#define DLC ((struct DlcHeader *)0x2000000)
/* [0] first sector of DLC.BIN, [1] its size in bytes (both written by tools/mod.py), [2] 1 once
 * it is loaded. These words held the screenshot routine's file name patterns
 * ("host0:%s/frame%03d.bmp", 0x2d0806), which nothing reads any more (tools/card_edit.py uses
 * the bytes after them). */
#define DLC_INFO ((volatile u32 *)0x2d0808)
#define DLC_LOADED (DLC_INFO[2] == 1)
/* Read in pieces of this size through a buffer in the game's own memory, then copied. */
#define CHUNK 0x8000

extern void SzDebug_Init(void);
extern void *SzMem_Alloc(int byte_count);
extern void SzMem_Free(void *block);
extern void *SzFile_ReadParam(int priority, char *name, void *destination, int byte_size,
                              int byte_offset, int range);
extern void FlushCache(int which);
extern int ExSortPrim_Pipe(int packet, int source, int table, int z);
extern int gRhView_nStackIndex[];
extern char gRhView_avl[];

void lSzDebug_ActEffect(void)
{
}

/* RhView_SetAspect jumps here (asm/ee/view_state/func_0020df90.s). */
int WsView_SetAspect(float a, float b)
{
    float *v;

    if (DLC_LOADED) {
        return ((int (*)(float, float))DLC->exports[2])(a, b);
    }
    v = (float *)(gRhView_avl + gRhView_nStackIndex[0] * 0x1a0 + 0x158);
    v[0] = a;
    v[1] = b;
    return 1;
}

int WsUi_SortPrim(int packet, int source, int table, int z)
{
    if (DLC_LOADED) {
        return ((int (*)(int, int, int, int))DLC->exports[0])(packet, source, table, z);
    }
    return ExSortPrim_Pipe(packet, source, table, z);
}

int WsUi_SortPrim3D(int packet, int source, int table, int z)
{
    if (DLC_LOADED) {
        return ((int (*)(int, int, int, int))DLC->exports[1])(packet, source, table, z);
    }
    return ExSortPrim_Pipe(packet, source, table, z);
}

/* 1 when writing at 0x2000000 + x neither fails nor lands on x: real memory past the first
 * 32 MB, not missing and not a mirror of the first 32 MB. DLC_INFO[2] is the probe. */
static int Dlc_HasMemory(void)
{
    volatile u32 *low = &DLC_INFO[2];
    volatile u32 *high = (volatile u32 *)((u32)low + 0x2000000);
    int ok;

    *low = 0x11111111;
    *high = 0x22222222;
    ok = *low == 0x11111111 && *high == 0x22222222;
    *low = 0;
    return ok;
}

static void Dlc_Load(void)
{
    u32 first = DLC_INFO[0];
    u32 size = DLC_INFO[1];
    u32 done, n, i;
    u32 *buffer;
    u32 *target = (u32 *)DLC;

    if (first == 0 || size < sizeof(struct DlcHeader) || size > 0x4000000 || !Dlc_HasMemory()) {
        return;
    }
    buffer = (u32 *)SzMem_Alloc(CHUNK);
    if (buffer == 0) {
        return;
    }
    for (done = 0; done < size; done += n) {
        n = size - done;
        if (n > CHUNK) n = CHUNK;
        n = (n + 2047) & ~2047;
        FlushCache(0);
        /* A name with bit 31 set is a sector number: the request reads sectors directly. */
        if (SzFile_ReadParam(0, (char *)(0x80000000 | (first + done / 2048)), buffer, n, 0, -1) == 0) {
            break;
        }
        FlushCache(0);
        for (i = 0; i < n / 4; i++) {
            target[(done >> 2) + i] = buffer[i];
        }
    }
    SzMem_Free(buffer);
    if (done < size || DLC->magic != DLC_MAGIC
        || DLC->file_end != (char *)DLC + size && DLC->file_end != (char *)DLC + ((size + 2047) & ~2047)
        || DLC->mem_end < DLC->file_end || DLC->mem_end > (char *)DLC + 0x6000000) {
        DLC->magic = 0;
        return;
    }
    for (i = (u32)DLC->file_end; i < (u32)DLC->mem_end; i += 4) {
        *(u32 *)i = 0;
    }
    FlushCache(0);
    FlushCache(2);
    DLC_INFO[2] = 1;
    DLC->init();
    if (DLC->hooks != 0) {
        struct DlcHook *h;
        for (h = DLC->hooks; h->address != 0; h++) {
            ((u32 *)h->address)[0] = 0x08000000 | (((u32)h->target >> 2) & 0x3ffffff);
            ((u32 *)h->address)[1] = 0;
        }
        FlushCache(0);
        FlushCache(2);
    }
}

/* Main_Init calls this in place of SzDebug_Init. */
void Dlc_Boot(void)
{
    SzDebug_Init();
    Dlc_Load();
}
