/* New cards' pictures (see dlc_cards.h): their numbers, and loading them where they are wanted.
 *
 * New card k (in cards.json order) has picture DLC_PICTURE_FIRST + k (+ 2 with the deck builder's
 * wallpaper, which comes first), a record of that number in DATA/PICTURE.MRG (tools/mod.py extends
 * it). A picture is a texture item (1, number) of the game's item table (gSzText_aItem, 1,024
 * entries).
 *
 * Duels load their decks' pictures by number from the file, any number (SzText_LoadMerge). The
 * deck builder (mode 0) loads all 871 of the game's pictures at once, in nine groups, into one
 * block from 0xb1b000 (0x4410 bytes a picture, so picture n at 0xb1b000 + n * 0x4410); there is no
 * room in that block or in the item table for all the new cards' pictures as well. So:
 *
 *   - the wallpaper's two pictures are one more group of that block (item 0x90: pictures 900 and
 *     901), loaded with the deck builder's;
 *   - every new card's picture is loaded when something asks for it and it is not there:
 *     SzText_GetReadyItem (entry_table/func_00200930.c) is replaced (mod.json hooks) by
 *     DlcPic_GetReadyItem, which does its lookup and, for a missing new card's picture, starts
 *     reading it (one record, 0x4800 bytes) into one of CACHE_SLOTS places in the upper memory
 *     (CACHE_BASE), giving back nothing until it is there. The deck builder shows "Loading" in
 *     the meantime. A place is taken again from the picture asked for least lately, whose item is
 *     freed first. A mode that frees all pictures (SzText_Free(1, -1)) frees these too; a place
 *     whose item is gone or no longer points at it is free. */

#include "dotr/ee/entry_table.h"
#include "dlc_cards.h"

extern int SzText_Free(int group, int id);
extern int func_0019b648(char *destination, const char *format, ...);
extern char *gSzText_apchMerge[10];
extern char data_002d074b[];
extern char data_002d0750[];
extern int DlcPort_SzData_ExchSpecial(int side, int cardNo);
extern const int dlc_wallpaper;
extern int SzGame_GetMode(int which);

#define PICTURE_GROUP_ITEM (DLC_PICTURE_FIRST / 100 * 16)
#define PICTURE_RECORD 0x4800
#define PICTURE_SLOT 0x4410
/* Upper memory (PCSX2's 128 MB): the monster model areas end at 0x5a00000 (card_models.c). */
#define CACHE_BASE 0x6000000
#define CACHE_SLOTS 24

int DlcCard_Picture(int id)
{
    int i;

    for (i = 0; i < dlc_card_count; i++) {
        if (dlc_cards[i].number == id && DlcCard_IsNew(id)) {
            return dlc_cards[i].picture;
        }
    }
    return -1;
}

/* SzData_ExchSpecial (side_table/func_002166d0.c): the picture number of a card; a new card's own
 * picture. */
int DlcSave_ExchSpecial(int side, int cardNo)
{
    int picture = DlcCard_Picture(cardNo);

    if (picture >= 0) {
        return picture;
    }
    return DlcPort_SzData_ExchSpecial(side, cardNo);
}

/* 1 for a new card's picture number (they follow one another in cards.json order). */
static int is_new_picture(int id)
{
    return dlc_card_count > 0 && id >= dlc_cards[0].picture && id < dlc_cards[0].picture + dlc_card_count;
}

static void picture_file(char *name)
{
    func_0019b648(name, data_002d0750, SzFile_GetEnv(data_002d074b), gSzText_apchMerge[0]);
}

/* ------------------------------------------------------------------ pictures on demand */

static short cache_id[CACHE_SLOTS];          /* the picture a place holds or is loading, 0 none */
static unsigned int cache_used[CACHE_SLOTS];  /* when it was last asked for */
static unsigned int tick;

static struct DotrEntryTableEntry *find_item(int group, int id)
{
    struct DotrEntryTableEntry *entry = gSzText_aItem;
    int i;

    if (group < 0 || group >= 10 || id < 0) {
        return 0;
    }
    for (i = 0; i < 1024; i++, entry++) {
        if (entry->pointerToTextureSource && entry->group == group && entry->id == id) {
            return entry;
        }
    }
    return 0;
}

static unsigned int place_address(int s)
{
    return CACHE_BASE + s * PICTURE_RECORD;
}

/* 1 when place s's picture item is still there and ready at the place. */
static int place_ready(int s)
{
    struct DotrEntryTableEntry *e = cache_id[s] ? find_item(1, cache_id[s]) : 0;

    return e != 0 && !(e->pointerToTextureSource & 0x80000000)
           && ((unsigned int)e->pointerToTextureSource & 0x0fffffff) == (place_address(s) & 0x0fffffff);
}

/* 1 when place s holds nothing of its own: never used, or its item freed or replaced. */
static int place_free(int s)
{
    struct DotrEntryTableEntry *e = cache_id[s] ? find_item(1, cache_id[s]) : 0;

    if (e == 0) {
        return 1;
    }
    if (e->pointerToTextureSource & 0x80000000) {
        return 0;                       /* still loading: maybe ours */
    }
    return !place_ready(s);
}

static void request(int id)
{
    char name[0x80];
    int s, best = -1;

    for (s = 0; s < CACHE_SLOTS; s++) {
        if (place_free(s)) {
            best = s;
            break;
        }
    }
    if (best < 0) {
        /* The ready picture asked for least lately (not one asked for this time round). */
        for (s = 0; s < CACHE_SLOTS; s++) {
            if (place_ready(s) && cache_used[s] != tick && (best < 0 || cache_used[s] < cache_used[best])) {
                best = s;
            }
        }
        if (best < 0) {
            return;                     /* all loading: ask again later */
        }
        SzText_Free(1, cache_id[best]);
    }
    picture_file(name);
    cache_id[best] = id;
    cache_used[best] = tick;
    SzText_LoadFileEx(1, id, name, place_address(best), PICTURE_RECORD, id * PICTURE_RECORD, PICTURE_RECORD);
}

/* SzText_GetReadyItem (entry_table/func_00200930.c), hooked: the item when it is loaded; a new
 * card's missing picture starts loading. */
struct DotrEntryTableEntry *DlcPic_GetReadyItem(int group, int id)
{
    struct DotrEntryTableEntry *entry = find_item(group, id);
    int s;

    if (group == 1 && is_new_picture(id)) {
        tick++;
        for (s = 0; s < CACHE_SLOTS; s++) {
            if (cache_id[s] == id) {
                cache_used[s] = tick;
            }
        }
        if (entry == 0) {
            request(id);
            return 0;
        }
    }
    if (entry && !(entry->pointerToTextureSource & 0x80000000)) {
        return entry;
    }
    return 0;
}

/* ------------------------------------------------------------------ the wallpaper */

typedef int (*LoadMerge)(int, int, int, int, int, int, int, int);
extern unsigned int DlcTramp_SzText_LoadMerge[];

/* The deck builder's 3D card viewer (SzDuel_Init3DView) loads its monster as model 29 at
 * 0x1b00000-0x1bfffff, past the wallpaper's two pictures; nothing is reloaded while it is open. */
#define VIEWER_MODE 7
#define WALLPAPER_PICTURES 2

static void load_group(void)
{
    char name[0x80];

    if (SzText_GetItem(0, PICTURE_GROUP_ITEM) != 0) {
        SzText_Free(0, PICTURE_GROUP_ITEM);
    }
    picture_file(name);
    SzText_LoadFileEx(0, PICTURE_GROUP_ITEM, name, 0xb1b000 + DLC_PICTURE_FIRST * PICTURE_SLOT,
                      WALLPAPER_PICTURES * PICTURE_RECORD, DLC_PICTURE_FIRST * PICTURE_RECORD,
                      WALLPAPER_PICTURES * PICTURE_RECORD);
}

/* The wallpaper's pictures: 900 and 901. */
int DlcPic_Wallpaper(void)
{
    return DLC_PICTURE_FIRST;
}

static int viewer_open;

void DlcPic_ViewerOpen(int open)
{
    viewer_open = open;
}

/* Called every frame the deck builder draws its background: loads the wallpaper when it is missing
 * (another mode freed it) or was overwritten (no "RH2" at its place). A request already queued is
 * left alone (SzText_LoadFileEx refuses an item that exists). */
void DlcPic_Ensure(void)
{
    int i;

    if (viewer_open || SzGame_GetMode(0) == VIEWER_MODE || !dlc_wallpaper) {
        return;
    }
    if (find_item(1, DLC_PICTURE_FIRST) == 0 || (find_item(1, DLC_PICTURE_FIRST)->pointerToTextureSource & 0x80000000)) {
        if (find_item(1, DLC_PICTURE_FIRST) == 0) {
            load_group();
        }
        return;
    }
    for (i = 0; i < WALLPAPER_PICTURES; i++) {
        if (*(unsigned int *)(0xb1b000 + (DLC_PICTURE_FIRST + i) * PICTURE_SLOT) != 0x00324852) {
            break;
        }
    }
    if (i < WALLPAPER_PICTURES) {
        for (i = 0; i < WALLPAPER_PICTURES; i++) {
            SzText_Free(1, DLC_PICTURE_FIRST + i);
        }
        load_group();
    }
}

int DlcPic_LoadMerge(int mode, int force, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int result = ((LoadMerge)DlcTramp_SzText_LoadMerge)(mode, force, a2, a3, a4, a5, a6, a7);

    if (mode == 0) {
        DlcPic_Ensure();
    }
    return result;
}
