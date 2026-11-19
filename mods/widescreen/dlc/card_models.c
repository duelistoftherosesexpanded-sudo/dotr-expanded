/* New cards' own 3D models.
 *
 * A monster's model is a 1 MiB slot of DATA/MONSTER.MRG (skeleton, animations, mesh and textures),
 * chosen by its card's place in AlternateCardIds ("xax"). A new card has its donor's place there,
 * so that the donor's battle code, sounds and effects go with it. A new card with a model of its own
 * (cards.json "model") has a 1 MiB slot of its own instead, built by tools/dlc_cards.py from the
 * donor's slot with the new mesh and texture, and stored in DLC.BIN after the DLC's code (the
 * loader reads only the code; dlc_header.models says where the slots start).
 *
 * SzDuel_AddUnit2 (ported, mod.json) tells DlcModel_Bind which card goes into which of the game's
 * 84 model objects before it presets the object; SzModel_Load (ported) then reads that card's slot
 * from DLC.BIN by sector (a file name with bit 31 set is a sector number for the file reader)
 * instead of the donor's from MONSTER.MRG, as long as the object is still loading the same place. */

#include "dlc_cards.h"

struct DlcHeaderView {
    unsigned int words[17];  /* magic ... hooks, as in header.c */
    unsigned int models;
    unsigned int live;
    unsigned int models_sector;
};
extern struct DlcHeaderView dlc_header;

/* DLC_INFO of the loader in the executable: [0] DLC.BIN's first sector on the disc. */
#define DLC_FIRST_SECTOR (*(volatile unsigned int *)0x2d0808)
#define OBJECTS 84
/* A new card's slot in DLC.BIN (tools/dlc_cards.py MODEL_SLOT): twice the game's, so a big mesh fits. */
#define SLOT_BYTES 0x200000
#define GAME_SLOT_BYTES 0x100000

/* Where the monster objects (20-32: the game's 0x1200000, 0x1c00000 and 0x10be800, 1 MiB each) keep
 * their model now: 2 MiB each from 0x4000000, in the console's upper 96 MiB, which only the DLC
 * uses (the loader requires it). Any monster's model, the game's (1 MiB) or a new card's (2 MiB),
 * fits there. */
#define AREA_FIRST_OBJECT 20
#define AREA_LAST_OBJECT 32
#define AREA_BASE 0x4000000
#define AREA_BYTES 0x200000

static short bound_place[OBJECTS];  /* the xax the object was preset with, + 1 (0: none) */
static short bound_model[OBJECTS];  /* the card's own model + 1 (0: none) */

void DlcModel_Bind(int object, int card, int place)
{
    int model;

    if (object < 0 || object >= OBJECTS) {
        return;
    }
    model = DlcCard_Model(card);
    bound_place[object] = place + 1;
    bound_model[object] = model >= 0 ? model + 1 : 0;
}

static int own_model(int object, int place)
{
    if (object < 0 || object >= OBJECTS || bound_model[object] == 0 || bound_place[object] != place + 1
        || dlc_header.models == 0) {
        return -1;
    }
    return bound_model[object] - 1;
}

/* The file to read the model from: DLC.BIN by sector for an object showing a card with its own
 * model, else the game's (name, the path of MONSTER.MRG). */
char *DlcModel_PackFile(char *name, int object, int place)
{
    if (own_model(object, place) < 0) {
        return name;
    }
    if (dlc_header.models_sector != 0) {
        return (char *)(0x80000000 | dlc_header.models_sector);
    }
    return (char *)(0x80000000 | (DLC_FIRST_SECTOR + dlc_header.models / 2048));
}

/* Where the slot starts in that file. */
int DlcModel_PackOffset(int object, int place, int card)
{
    int model = own_model(object, place);

    return model < 0 ? card << 20 : model * SLOT_BYTES;
}

/* The memory an object loads its monster model into (SzModel_Load, ported), and its size. */
int DlcModel_Area(int object, int original)
{
    if (object < AREA_FIRST_OBJECT || object > AREA_LAST_OBJECT) {
        return original;
    }
    return AREA_BASE + (object - AREA_FIRST_OBJECT) * AREA_BYTES;
}

int DlcModel_AreaBytes(int object)
{
    return object < AREA_FIRST_OBJECT || object > AREA_LAST_OBJECT ? GAME_SLOT_BYTES : AREA_BYTES;
}

/* How much to read: a new card's whole 2 MiB slot, or the game's 1 MiB. */
int DlcModel_ReadBytes(int object, int place)
{
    return own_model(object, place) < 0 || DlcModel_AreaBytes(object) < SLOT_BYTES ? GAME_SLOT_BYTES : SLOT_BYTES;
}
