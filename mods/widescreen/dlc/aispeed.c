/* AI SPEED (the duel's settings screen, L1, in the place of SOUND): FAST runs the duel twice a
 * frame while the computer plays its turn, so its moves, cursor and choices go twice as fast.
 *
 * The setting is the game's option 2, which was SOUND (stereo / mono): its left choice, set,
 * reads NORMAL (it was STEREO, the default, so a save from before keeps the AI's own pace), the
 * right one, clear, FAST. The sound is always stereo now (SzSound_SetOutput, ported in mod.json).
 * The words are the game's strings 116, 121 and 122 (and its help text 3030), given new text by
 * cards.c.
 *
 * SzDuel_Act (wrapped, mod.json) runs a second time in the same frame only when: FAST is chosen,
 * the turn's side is the computer's (its controller below 100) and the other side a pad's, the
 * duel is in its main phases (19-83: not set-up, not an ending), and the first run left the game
 * in the same mode (a duel that ends or leaves is never run past that). */

typedef int (*DuelAct)(int, int, int, int, int, int, int, int);
extern unsigned int DlcTramp_SzDuel_Act[];
extern int SzDuel_GetTurnOwner(void);
extern int SzData_GetOption(int side, int which);
extern int SzGame_GetMode(int which);

#define DUEL ((unsigned char *)0x317490)
#define CONTROLLER(side) (DUEL[(side) * 0x350 + 0x33c])
#define PHASE (DUEL[0xd57])
#define PAD_FIRST 100
#define PHASE_MAIN_FIRST 19
#define PHASE_ENDINGS 84
#define OPTION_AI_SPEED 2     /* 1 (set) NORMAL, 0 FAST */

static int computer_turn(void)
{
    int owner = SzDuel_GetTurnOwner();

    if (owner < 0 || owner > 1) {
        return 0;
    }
    return CONTROLLER(owner) < PAD_FIRST && CONTROLLER(owner ^ 1) >= PAD_FIRST
           && PHASE >= PHASE_MAIN_FIRST && PHASE < PHASE_ENDINGS;
}

int DlcAi_Act(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int mode = SzGame_GetMode(0);
    int fast = SzData_GetOption(0, OPTION_AI_SPEED) == 0 && computer_turn();
    int r = ((DuelAct)DlcTramp_SzDuel_Act)(a0, a1, a2, a3, a4, a5, a6, a7);

    if (fast && SzGame_GetMode(0) == mode && computer_turn()) {
        r = ((DuelAct)DlcTramp_SzDuel_Act)(a0, a1, a2, a3, a4, a5, a6, a7);
    }
    return r;
}
