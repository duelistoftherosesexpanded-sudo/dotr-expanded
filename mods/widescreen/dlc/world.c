/* The second world: the campaign map again, green land and blue sea, with opponents of its own
 * (bosses.json "world": 2), reached through a gate podium (bosses.c).
 *
 * The map is model pack type 3 of DATA/MODEL.MRG (SzModel_LoadMerge(3, n, ...), file entry 33 + n).
 * tools/dlc_world.py appends recoloured copies of entries 33 and 35 (n = 0 and 2) to the file, in
 * that order, from its old end; SzModel_LoadMerge is ported (mod.json) to read the copy instead
 * while the second world is shown. Switching worlds reloads the pack the map last asked for.
 *
 * In the second world the game's own map drawing (Taku_Camp_3D: its locations, routes, ship and
 * leader) is replaced by the map models alone and the second world's podiums (bosses.c), and the
 * game's own locations cannot be hovered or picked (TkCamp_Act2 is wrapped in bosses.c). The
 * world is saved (card_save.c); the map pack in memory is reloaded whenever it is not the one of
 * the current world (DlcWorld_Check, each map frame), e.g. after loading a save. */

#include "dlc_bosses.h"

extern int SzModel_LoadMerge(int type, int entry, int force);

#define MODEL_OFFSETS ((unsigned int *)0x292a50)   /* anOfs_MODEL: 82 offsets, the last the size */
#define OFFSET_COUNT 82
#define MAP_TYPE 3
#define MAP_FIRST_ENTRY 33

int dlc_world;
static int last_entry = 2;
static int pack_world;   /* the world of the map pack last read */

/* SzModel_LoadMerge (ported): the file offset to read pack `fileIndex` from. */
int DlcWorld_PackOffset(int type, int fileIndex, int original)
{
    unsigned int *ofs = MODEL_OFFSETS;
    unsigned int end = ofs[OFFSET_COUNT - 1];

    if (type != MAP_TYPE) {
        return original;
    }
    last_entry = fileIndex - MAP_FIRST_ENTRY;
    pack_world = dlc_world;
    if (!dlc_world) {
        return original;
    }
    if (fileIndex == 33) {
        return (int)end;
    }
    if (fileIndex == 35) {
        return (int)(end + (ofs[34] - ofs[33]));
    }
    return original;
}

void DlcWorld_Switch(void)
{
    dlc_world = !dlc_world;
    SzModel_LoadMerge(MAP_TYPE, last_entry, 1);
}

/* After a load or a new game: the world only; the map follows on its next frame. */
void DlcWorld_Set(int world)
{
    dlc_world = world != 0;
}

/* Each map frame: reload the map pack if it belongs to the other world. */
void DlcWorld_Check(void)
{
    if (pack_world != dlc_world) {
        SzModel_LoadMerge(MAP_TYPE, last_entry, 1);
        pack_world = dlc_world;
    }
}
