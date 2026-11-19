/* Community patches chosen in the title screen's MOD OPTIONS (modoptions.c; the data written by
 * tools/dlc_community.py from community.json). */

#ifndef DLC_MODS_H
#define DLC_MODS_H

struct DlcModRun {
    unsigned int address;   /* in the game's memory */
    unsigned int length;
    unsigned int at;        /* in dlc_mod_bytes: the original bytes, then the patched ones */
};

struct DlcModPatch {
    const char *name;
    int kind;               /* 0 main patch; -1 extra for the game; -1 - k extra for main patch k */
    int first, count;       /* its runs */
};

extern const unsigned char dlc_mod_bytes[];
extern const struct DlcModRun dlc_mod_runs[];
extern const struct DlcModPatch dlc_mod_patches[];
extern const int dlc_mod_patch_count;

/* The choice: main 0 the game, k the k-th main patch; extras a bit per extra (in order). */
int DlcMods_Main(void);
int DlcMods_Extras(void);
/* Applies SPEX (any choice given, as from a loaded save, is taken as SPEX): the memory is changed
 * only where it differs. DlcMods_Init applies it at start-up. */
void DlcMods_Set(int main, int extras);
void DlcMods_Init(void);

/* MOD OPTIONS' BUTTONS row (buttons.c): 0 the PlayStation pictures, 1 Xbox ones. */
int DlcButtons_Xbox(void);
void DlcButtons_Set(int xbox);
/* Keeps the loaded window texture showing the chosen pictures; called often (DlcWs_SortPrim). */
void DlcButtons_Update(void);

/* MOD OPTIONS' SLEEVE row (sleeves.c): 0 the game's rose shield, k the k-th sleeve added in the
 * launcher. */
int DlcSleeves_Count(void);
const char *DlcSleeves_Name(int k);
int DlcSleeves_Chosen(void);
void DlcSleeves_Set(int k);
void DlcSleeves_Update(void);

#endif
