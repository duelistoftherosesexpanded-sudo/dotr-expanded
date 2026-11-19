/* Stacked effects: more than one effect in any of a monster's five slots (effects.json gives a
 * list; the first is the slot's own, the rest are stacked, see dlc_effects.h).
 *
 * A unit keeps one effect per slot, and the game's rules read it through SzDuel_GetUnitEffect
 * (Ta_GetUnitEffect calls it). That function is wrapped here with layers: while layer k is
 * active, the passive slots (0-2) of a unit whose card has a k-th stacked effect answer that
 * effect, and the other units' answer none; LAYER_NONE answers none for every unit. The rules
 * that read the passive slots are run once per layer and their answers combined:
 *
 *   sums       Ta_GetParamOfs, Ta_GetSummonCalcRevision, Ta_GetEnchantOfs: the plain answer, plus
 *              for each layer what it adds over LAYER_NONE
 *   in turn    Ta_GetLPRevByNaturalEffect (a value changed by effects): each layer changes the
 *              value the one before gave
 *   yes / no   Ta_IsResistAll(Master), Ta_IsUnitHide(2), TaPerm_IsPermCurseActive: yes when any
 *              layer says yes
 *   searches   Ta_SearchUncTypeNaturalEffectUser(Num, NumWithSide): the units with the type in any
 *              layer (counts add up; the n-th is counted through the layers in turn)
 *   battle     Manage_AtkEfc (ported) runs Do_AtkEfc once per ATTACK effect (DlcStack_AtkEfc)
 *   movement   GetMoveRev (ported, wrapped): labyrinth if any layer allows it, the best extra
 *              movement, no movement if any layer forbids it; Chk_TrapA's "ignores limited traps"
 *              (MOVEMENT type 7) in any layer (DlcStack_HasType)
 *
 * Rules called from inside another while a layer is active run as they are (no nesting).
 *
 * Effects that play (an effect record the duel then shows) come one at a time from functions
 * the duel calls step by step; each answers the next step. After the slot's own effect, extra
 * steps (numbers the game does not use) play the stacked ones, then the duel goes on with the
 * step the game chose:
 *
 *   FLIP         Ta_Open (flipped by the player or at the start of a turn) and Ta_FinishMove
 *                steps 2, 3 and 17 (flipped in battle; the stacked ones play when the first left
 *                both fighters in place, and one that removes a fighter ends the battle as the
 *                game's own would)
 *   DESTRUCTION  Ta_FinishMove steps 8-10 (destroyed in battle); the stacked ones play while the
 *                card fades, and the battle ends as the first effect decided
 *   MOVEMENT     Ta_Do_MoveEfc (LP per move, terrain round the card ...): each stacked one acts;
 *                only one can leave an effect to show (the first that has one)
 *   NATURE       Ta_Do_StandbyPhase (each turn: LP, weaken or strengthen monsters ...)
 *
 * Those read the stacked effect through SzDuel_GetUnitEffect with a pick (that unit's slot
 * answers the k-th stacked effect, everything else as it is), or are handed it directly. */

#include "dlc_effects.h"

extern int SzDuel_GetUnitCardNo(int side, int pos);
extern void *SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern unsigned int Do_AtkEfc(unsigned int eID, unsigned int eParam, void *pDat, int nSide_Do, int ID_Do,
                              int nSide_Done, int ID_Done, unsigned int fAtkSide);
const struct DlcEffectCard *DlcEffect_Find(int card);

#define LAYER_OFF -1
#define LAYER_NONE -2
#define PASSIVE 3     /* slots 0-2 */

static int layer = LAYER_OFF;
static const unsigned int none_pair[2] = {0xffffffff, 0xffffffff};

/* The pick: unit pick_side/pick_pos's slot pick_slot answers its stacked effect pick_k (a copy,
 * which the game may write to). pick_side -1: none. */
static int pick_side = -1, pick_pos, pick_slot, pick_k;
static unsigned int pick_pair[2];

typedef int (*Fn)(int, int, int, int, int, int, int, int);

static const struct DlcEffectCard *card_of(int side, int pos)
{
    return DlcEffect_Find(SzDuel_GetUnitCardNo(side, pos));
}

static int known = -1;

/* After a live edit (header.c Dlc_LiveCheck): count the layers again. */
void DlcStack_Reset(void)
{
    known = -1;
}

/* How many layers any card on the field could use (the most stacked effects of any card). */
static int layers(void)
{
    extern const int dlc_effect_count;
    extern const struct DlcEffectCard dlc_effect_cards[];
    int k, s, most = 0;

    if (known >= 0) {
        return known;
    }
    for (k = 0; k < dlc_effect_count; k++) {
        for (s = 0; s < PASSIVE; s++) {
            if (dlc_effect_cards[k].stacked[s] > most) {
                most = dlc_effect_cards[k].stacked[s];
            }
        }
    }
    known = most;
    return most;
}

/* ---- SzDuel_GetUnitEffect: the layers ---- */

extern unsigned int DlcTramp_SzDuel_GetUnitEffect[];

void *DlcStack_GetUnitEffect(int side, int pos, int slot, int a3, int a4, int a5, int a6, int a7)
{
    void *own = (void *)((Fn)DlcTramp_SzDuel_GetUnitEffect)(side, pos, slot, a3, a4, a5, a6, a7);
    const struct DlcEffectCard *c;

    if (pick_side >= 0 && own != 0 && side == pick_side && pos == pick_pos && slot == pick_slot) {
        c = card_of(side, pos);
        if (c != 0 && pick_k < c->stacked[slot]) {
            pick_pair[0] = c->extra[slot][pick_k][0];
            pick_pair[1] = c->extra[slot][pick_k][1];
        } else {
            pick_pair[0] = pick_pair[1] = 0xffffffff;
        }
        return pick_pair;
    }
    if (layer == LAYER_OFF || own == 0 || slot < 0 || slot >= PASSIVE) {
        return own;
    }
    if (layer == LAYER_NONE) {
        return (void *)none_pair;
    }
    c = card_of(side, pos);
    if (c == 0 || layer >= c->stacked[slot]) {
        return (void *)none_pair;
    }
    return (void *)c->extra[slot][layer];
}

/* Whether the unit has an effect of `type` in `slot`, its own or stacked. */
int DlcStack_HasType(int side, int pos, int slot, int type)
{
    unsigned int *own = (unsigned int *)((Fn)DlcTramp_SzDuel_GetUnitEffect)(side, pos, slot, 0, 0, 0, 0, 0);
    const struct DlcEffectCard *c;
    int k;

    if (own == 0) {
        return 0;
    }
    if (own[0] != 0xffffffff && (int)(own[0] >> 8) == type) {
        return 1;
    }
    c = card_of(side, pos);
    for (k = 0; c != 0 && slot < 5 && k < c->stacked[slot]; k++) {
        if ((int)(c->extra[slot][k][0] >> 8) == type) {
            return 1;
        }
    }
    return 0;
}

/* The unit's pair in `slot`, read from the unit itself (GetMoveRev reads it there), with the
 * layers: the card number at +0x3c, the five pairs from +8. */
void *DlcStack_UnitPair(void *unit, int slot)
{
    unsigned char *u = (unsigned char *)unit;
    const struct DlcEffectCard *c;

    if (layer == LAYER_OFF || slot >= PASSIVE) {
        return u + 8 + 8 * slot;
    }
    if (layer == LAYER_NONE) {
        return (void *)none_pair;
    }
    c = DlcEffect_Find(*(unsigned short *)(u + 0x3c));
    if (c == 0 || layer >= c->stacked[slot]) {
        return (void *)none_pair;
    }
    return (void *)c->extra[slot][layer];
}

/* ---- combining ---- */

static int run(unsigned int *tramp, int at, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int keep = layer, r;

    layer = at;
    r = ((Fn)tramp)(a0, a1, a2, a3, a4, a5, a6, a7);
    layer = keep;
    return r;
}

#define ARGS a0, a1, a2, a3, a4, a5, a6, a7
#define PARAMS int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7

static int sum(unsigned int *tramp, PARAMS)
{
    int total, base, k, n = layers();

    total = run(tramp, LAYER_OFF, ARGS);
    if (layer != LAYER_OFF || n == 0) {
        return total;
    }
    base = run(tramp, LAYER_NONE, ARGS);
    for (k = 0; k < n; k++) {
        total += run(tramp, k, ARGS) - base;
    }
    return total;
}

static int any(unsigned int *tramp, PARAMS)
{
    int r, k, n = layers();

    r = run(tramp, LAYER_OFF, ARGS);
    if (layer != LAYER_OFF) {
        return r;
    }
    for (k = 0; r == 0 && k < n; k++) {
        r = run(tramp, k, ARGS);
    }
    return r;
}

#define SUM(name) \
    extern unsigned int DlcTramp_##name[]; \
    int DlcStack_##name(PARAMS) { return sum(DlcTramp_##name, ARGS); }
#define ANY(name) \
    extern unsigned int DlcTramp_##name[]; \
    int DlcStack_##name(PARAMS) { return any(DlcTramp_##name, ARGS); }

SUM(Ta_GetParamOfs)
SUM(Ta_GetSummonCalcRevision)
SUM(Ta_GetEnchantOfs)
SUM(Ta_SearchUncTypeNaturalEffectUserNum)
SUM(Ta_SearchUncTypeNaturalEffectUserNumWithSide)
ANY(Ta_IsResistAll)
ANY(Ta_IsResistAllMaster)
ANY(Ta_IsUnitHide)
ANY(Ta_IsUnitHide2)
ANY(TaPerm_IsPermCurseActive)

/* Ta_GetLPRevByNaturalEffect(type, value): each layer changes the value the one before gave. */
extern unsigned int DlcTramp_Ta_GetLPRevByNaturalEffect[];

int DlcStack_Ta_GetLPRevByNaturalEffect(int type, int value, int a0, int a1, int a2, int a3, int a4, int a5)
{
    int k, n = layers();

    value = run(DlcTramp_Ta_GetLPRevByNaturalEffect, LAYER_OFF, type, value, a0, a1, a2, a3, a4, a5);
    if (layer != LAYER_OFF) {
        return value;
    }
    for (k = 0; k < n; k++) {
        value = run(DlcTramp_Ta_GetLPRevByNaturalEffect, k, type, value, a0, a1, a2, a3, a4, a5);
    }
    return value;
}

/* Ta_SearchUncTypeNaturalEffectUser(type, n): the n-th unit with the type, counting the units of
 * each layer after those of the one before. */
extern unsigned int DlcTramp_Ta_SearchUncTypeNaturalEffectUser[];

int DlcStack_Ta_SearchUncTypeNaturalEffectUser(int type, int n, int a0, int a1, int a2, int a3, int a4, int a5)
{
    int k, j, r, seen = 0, count = layers();

    r = run(DlcTramp_Ta_SearchUncTypeNaturalEffectUser, LAYER_OFF, type, n, a0, a1, a2, a3, a4, a5);
    if (r >= 0 || layer != LAYER_OFF || count == 0) {
        return r;
    }
    /* Not that many among the own effects: count them, then go on through the layers. */
    while (run(DlcTramp_Ta_SearchUncTypeNaturalEffectUser, LAYER_OFF, type, seen, a0, a1, a2, a3, a4, a5) >= 0) {
        seen++;
    }
    for (k = 0; k < count; k++) {
        for (j = 0; ; j++) {
            r = run(DlcTramp_Ta_SearchUncTypeNaturalEffectUser, k, type, j, a0, a1, a2, a3, a4, a5);
            if (r < 0) {
                break;
            }
            if (seen++ == n) {
                return r;
            }
        }
    }
    return -1;
}

/* GetMoveRev(unit, x, z), ported (its reads of the unit's MOVEMENT go through DlcStack_UnitPair):
 * bit 4 labyrinth, the low four bits the movement; 0 cannot move. */
extern unsigned int DlcPort_GetMoveRev(void *unit, int x, int z);

unsigned int DlcStack_GetMoveRev(void *unit, int x, int z)
{
    unsigned int r = DlcPort_GetMoveRev(unit, x, z), v;
    int k, n = layers(), keep = layer;

    if (layer != LAYER_OFF || r == 0) {
        return r;
    }
    for (k = 0; k < n; k++) {
        layer = k;
        v = DlcPort_GetMoveRev(unit, x, z);
        layer = keep;
        if (v == 0) {
            return 0;
        }
        r = (r & 0x10) | (v & 0x10) | ((v & 0xf) > (r & 0xf) ? v & 0xf : r & 0xf) | (r & ~0x1fu);
    }
    return r;
}

/* Manage_AtkEfc (ported) calls this for each fighter: its ATTACK effect, then each stacked one.
 * The answers combine: the low byte 1 when any is (destroyed), bit 16 and bit 24 when any has
 * them (bit 24 terrain changed). */
unsigned int DlcStack_AtkEfc(unsigned int eID, unsigned int eParam, void *pDat, int side, int id,
                             int side2, int id2, unsigned int fAtkSide)
{
    unsigned int r = Do_AtkEfc(eID, eParam, pDat, side, id, side2, id2, fAtkSide), v;
    const struct DlcEffectCard *c = card_of(side, id);
    int k;

    for (k = 0; c != 0 && k < c->stacked[0]; k++) {
        v = Do_AtkEfc(c->extra[0][k][0], c->extra[0][k][1], pDat, side, id, side2, id2, fAtkSide);
        r = ((r & 0xff) == 1 || (v & 0xff) == 1 ? 1 : (r & 0xff)) | ((r | v) & 0x1010000);
    }
    return r;
}

/* ---- effects that play: FLIP, DESTRUCTION, MOVEMENT, NATURE ---- */

#define FLIP 3
#define DEST 4

/* An effect record: the sequence number and its three operands. */
struct Record {
    int eNO;
    unsigned int dwFrom, dwTo, dwPar;
};

static int stacked(int side, int pos, int slot)
{
    const struct DlcEffectCard *c = card_of(side, pos);

    return c == 0 ? 0 : c->stacked[slot];
}

/* Whether the unit is still the card it was (not removed or replaced by an effect before). */
static int still(int side, int pos, int card)
{
    return SzDuel_GetUnit(side, pos) != 0 && SzDuel_GetUnitCardNo(side, pos) == card;
}

static void pick(int side, int pos, int slot, int k)
{
    pick_pos = pos;
    pick_slot = slot;
    pick_k = k;
    pick_side = side;
}

/* Ta_Manage_ActEfc (the FLIP effect *e of unit nSide/nID, met by eSide/eID): the wrap notes each
 * call, for Ta_Open and Ta_FinishMove to know which unit was flipped. */
typedef int (*ActFn)(void *, int, int, int, int, int, int, void *, void *, unsigned char);
extern unsigned int DlcTramp_Ta_Manage_ActEfc[];
static struct {
    int hit, side, id, eSide, eID, x, z;
    unsigned char heap;
} act;

int DlcStack_Ta_Manage_ActEfc(void *e, int nSide, int nID, int eSide, int eID, int nX, int nZ, void *Effect,
                              void *pInfo, unsigned char bHeap)
{
    act.hit = 1;
    act.side = nSide;
    act.id = nID;
    act.eSide = eSide;
    act.eID = eID;
    act.x = nX;
    act.z = nZ;
    act.heap = bHeap;
    return ((ActFn)DlcTramp_Ta_Manage_ActEfc)(e, nSide, nID, eSide, eID, nX, nZ, Effect, pInfo, bHeap);
}

static int act_stacked(int side, int id, int k, int eSide, int eID, int x, int z, void *rec, void *kind,
                       unsigned char heap)
{
    const struct DlcEffectCard *c = card_of(side, id);
    unsigned int pair[2];

    pair[0] = c->extra[FLIP][k][0];
    pair[1] = c->extra[FLIP][k][1];
    return ((ActFn)DlcTramp_Ta_Manage_ActEfc)(pair, side, id, eSide, eID, x, z, rec, kind, heap);
}

/* Ta_Open(nSide, nID, pDat, PrNo): the duel plays the record in pDat when the low byte is 1 (2 and
 * 4 first ask for a place or spring a trap), then calls again with step = the high half, done at
 * 0 (bit 8 set: the unit stays selected). Steps OPEN_STEP + k play stacked FLIP effect k. */
typedef int (*OpenFn)(int, int, void *, int);
extern unsigned int DlcTramp_Ta_Open[];
#define OPEN_STEP 0x40
static int open_card, open_k;

static int open_more(int nSide, int nID, unsigned char *pDat)
{
    int n = stacked(nSide, nID, FLIP), v, loc;
    struct Record *rec = (struct Record *)pDat;

    while (open_k < n && still(nSide, nID, open_card)) {
        rec->eNO = 0;
        rec->dwFrom = rec->dwTo = rec->dwPar = 0;
        pDat[16] = pDat[17] = 0;
        loc = SzDuel_GetUnitLoc(nSide, nID);
        v = act_stacked(nSide, nID, open_k++, -1, -1, loc & 0xff, loc >> 8, pDat, pDat + 16, 0);
        if (v & 0xff) {
            return (v & 0xffff) | (open_k < n ? (OPEN_STEP + open_k) << 16 : 0x100);
        }
    }
    return 0x100;
}

int DlcStack_Ta_Open(int nSide, int nID, void *pDat, int PrNo)
{
    int r;

    if (PrNo >= OPEN_STEP && PrNo < OPEN_STEP + DLC_STACK_MAX) {
        return open_more(nSide, nID, (unsigned char *)pDat);
    }
    act.hit = 0;
    r = ((OpenFn)DlcTramp_Ta_Open)(nSide, nID, pDat, PrNo);
    if (PrNo == 0 && act.hit && (r >> 16) == 0 && stacked(nSide, nID, FLIP) > 0) {
        open_card = SzDuel_GetUnitCardNo(nSide, nID);
        open_k = 0;
        return (r & 0xffff) | (OPEN_STEP << 16);
    }
    return r;
}

/* Ta_Manage_DestEfc (the DESTRUCTION effect of nSide_1/nID_1, destroyed by nSide_2/nID_2): the
 * wrap notes each call. */
typedef int (*DestFn)(int, int, int, int, int, int, void *, unsigned int);
extern unsigned int DlcTramp_Ta_Manage_DestEfc[];
static struct {
    int hit, side, id, eSide, eID, x, z;
    unsigned int flag;
} dest;

int DlcStack_Ta_Manage_DestEfc(int nSide_1, int nID_1, int nSide_2, int nID_2, int nX, int nZ, void *eInfo,
                               unsigned int fFlag)
{
    dest.hit = 1;
    dest.side = nSide_1;
    dest.id = nID_1;
    dest.eSide = nSide_2;
    dest.eID = nID_2;
    dest.x = nX;
    dest.z = nZ;
    dest.flag = fFlag;
    return ((DestFn)DlcTramp_Ta_Manage_DestEfc)(nSide_1, nID_1, nSide_2, nID_2, nX, nZ, eInfo, fFlag);
}

/* Ta_FinishMove(nSide, nID, nX, nZ, eInfo, PrNo): one step of a move onto nX/nZ, battle included.
 * The answer: the next step in the low byte with flags in bits 8-12, and from bit 16 what the
 * duel does first (1 play the record, 2 ask for a place, 3 and 4 a trap). Steps FM_FLIP and FM_DEST
 * play the stacked FLIP or DESTRUCTION effects. */
typedef int (*MoveFn)(int, int, int, int, void *, int);
extern unsigned int DlcTramp_Ta_FinishMove[];
#define FM_FLIP 0x40
#define FM_DEST 0x48
#define HI(v) (((v) & 0xff) << 16)

static struct {
    int step, side, id, eSide, eID, x, z, card, k;
    unsigned char heap;
} fflip;
static struct {
    int side, id, eSide, eID, x, z, card, k, next;
    unsigned int flag;
} fdest;
static int fm_open;   /* the record's "opened" bit, as step 0 sets it */

/* The step the game goes on with after a FLIP effect of step 3, 2 or 17 that removes no fighter. */
static int flip_rest(int step)
{
    return step == 3 ? 2 : step == 2 ? 6 : 5;
}

/* What step 3, 2 or 17 answers for the result v of the flipped unit's Ta_Manage_ActEfc (as
 * Ta_FinishMove has it): bits 8-11 say which fighters the effect removes. */
static int flip_map(int step, int v)
{
    int b8 = (v >> 8) & 1, b9 = (v >> 9) & 1, b10 = (v >> 10) & 1, b11 = (v >> 11) & 1;

    if ((v >> 8) <= 0) {
        return HI(v) | flip_rest(step);
    }
    if (step == 3) {
        if (b11 || b8) {
            return HI(v) | 14;
        }
        if (b9) {
            return HI(v) | (b10 << 9) | (b9 << 8) | 15;
        }
        return b10 ? HI(v) | (b10 << 9) | (b9 << 8) | 11 : 14;
    }
    if (step == 2) {
        if (b11) {
            return HI(v) | 14;
        }
        if (b8) {
            return HI(v) | (fm_open == 0 ? 0x20e : 14);
        }
    } else if (b11) {
        return HI(v) | 0x10e;
    }
    if (b10) {
        return HI(v) | (b9 << 9) | (b10 << 8) | 15;
    }
    if (b9) {
        return HI(v) | (b9 << 9) | (b10 << 8) | 11;
    }
    return flip_rest(step);
}

static void clear(struct Record *rec)
{
    rec->eNO = 999;
    rec->dwFrom = rec->dwTo = rec->dwPar = 0;
}

static int fm_flip(unsigned char *eInfo)
{
    int n = stacked(fflip.side, fflip.id, FLIP), v, val;

    while (fflip.k < n && still(fflip.side, fflip.id, fflip.card)) {
        clear((struct Record *)eInfo);
        v = act_stacked(fflip.side, fflip.id, fflip.k++, fflip.eSide, fflip.eID, fflip.x, fflip.z, eInfo,
                        eInfo + 16, fflip.heap);
        val = flip_map(fflip.step, v);
        if ((val & 0xffff) != flip_rest(fflip.step)) {
            return val;   /* a fighter goes: the battle goes on as the game has it */
        }
        if (val >> 16) {
            return fflip.k < n ? (val & 0xffff0000) | FM_FLIP : val;
        }
    }
    return flip_rest(fflip.step);
}

static int fm_dest(void *eInfo)
{
    int n = stacked(fdest.side, fdest.id, DEST), v;

    while (fdest.k < n && still(fdest.side, fdest.id, fdest.card)) {
        clear((struct Record *)eInfo);
        pick(fdest.side, fdest.id, DEST, fdest.k++);
        v = ((DestFn)DlcTramp_Ta_Manage_DestEfc)(fdest.side, fdest.id, fdest.eSide, fdest.eID, fdest.x, fdest.z,
                                                eInfo, fdest.flag);
        pick_side = -1;
        if (v & 0xff) {
            return HI(v) | (fdest.k < n ? FM_DEST : fdest.next);
        }
    }
    return fdest.next;
}

int DlcStack_Ta_FinishMove(int nSide, int nID, int nX, int nZ, void *eInfo, int PrNo)
{
    int no = PrNo & 0xff, r;

    if (no == FM_FLIP) {
        return fm_flip((unsigned char *)eInfo);
    }
    if (no == FM_DEST) {
        return fm_dest(eInfo);
    }
    if (no == 0) {
        fm_open = !SzDuel_GetUnitStatus(nSide, nID, 4) && SzDuel_GetUnitStatus(nSide, nID, 2);
    }
    act.hit = dest.hit = 0;
    r = ((MoveFn)DlcTramp_Ta_FinishMove)(nSide, nID, nX, nZ, eInfo, PrNo);
    if ((no == 2 || no == 3 || no == 17) && act.hit && (r & 0xffff) == flip_rest(no) &&
        stacked(act.side, act.id, FLIP) > 0) {
        fflip.step = no;
        fflip.side = act.side;
        fflip.id = act.id;
        fflip.eSide = act.eSide;
        fflip.eID = act.eID;
        fflip.x = act.x;
        fflip.z = act.z;
        fflip.heap = act.heap;
        fflip.card = SzDuel_GetUnitCardNo(act.side, act.id);
        fflip.k = 0;
        return (r & 0xffff0000) | FM_FLIP;
    }
    if (no >= 8 && no <= 10 && dest.hit && stacked(dest.side, dest.id, DEST) > 0) {
        fdest.side = dest.side;
        fdest.id = dest.id;
        fdest.eSide = dest.eSide;
        fdest.eID = dest.eID;
        fdest.x = dest.x;
        fdest.z = dest.z;
        fdest.flag = dest.flag;
        fdest.card = SzDuel_GetUnitCardNo(dest.side, dest.id);
        fdest.k = 0;
        fdest.next = r & 0xffff;
        return (r & 0xffff0000) | FM_DEST;
    }
    return r;
}

/* Ta_Do_MoveEfc(nSide, nID, nX, nZ, eInfo): the MOVEMENT effect acting as the unit moves. Each
 * stacked one runs too (most act at once: LP, terrain); the record of the first that leaves one
 * is shown. */
typedef int (*MoveEfcFn)(int, int, int, int, void *);
extern unsigned int DlcTramp_Ta_Do_MoveEfc[];

int DlcStack_Ta_Do_MoveEfc(int nSide, int nID, int nX, int nZ, void *eInfo)
{
    struct Record scratch[2];
    int r = ((MoveEfcFn)DlcTramp_Ta_Do_MoveEfc)(nSide, nID, nX, nZ, eInfo), v, k, n, card;

    if (nID == 0 || pick_side >= 0) {
        return r;
    }
    n = stacked(nSide, nID, 1);
    card = SzDuel_GetUnitCardNo(nSide, nID);
    for (k = 0; k < n && still(nSide, nID, card); k++) {
        pick(nSide, nID, 1, k);
        v = ((MoveEfcFn)DlcTramp_Ta_Do_MoveEfc)(nSide, nID, nX, nZ, r ? (void *)scratch : eInfo);
        pick_side = -1;
        if (r == 0) {
            r = v;
        }
    }
    return r;
}

/* Ta_Do_StandbyPhase(PrNo, eInfo): the start of a turn, one step at a time; steps 32-63 hand one
 * unit each to Ta_ManageNaturalEffectInStandbyPhase (or ...2), whose record the duel plays
 * (low byte 1) before it calls again with step = the high half (0xff done). Steps NAT_STEP + k
 * play that unit's stacked NATURE effects. */
typedef int (*NatFn)(int, int, void *);
typedef int (*StandbyFn)(int, void *);
extern unsigned int DlcTramp_Ta_ManageNaturalEffectInStandbyPhase[];
extern unsigned int DlcTramp_Ta_ManageNaturalEffectInStandbyPhase2[];
extern unsigned int DlcTramp_Ta_Do_StandbyPhase[];
#define NAT_STEP 0x100
static struct {
    int hit, side, id, second, card, k, next;
} nat;

int DlcStack_Ta_ManageNaturalEffectInStandbyPhase(int nSide, int nID, void *eInfo)
{
    nat.hit = 1;
    nat.side = nSide;
    nat.id = nID;
    nat.second = 0;
    return ((NatFn)DlcTramp_Ta_ManageNaturalEffectInStandbyPhase)(nSide, nID, eInfo);
}

int DlcStack_Ta_ManageNaturalEffectInStandbyPhase2(int nSide, int nID, void *eInfo)
{
    nat.hit = 1;
    nat.side = nSide;
    nat.id = nID;
    nat.second = 1;
    return ((NatFn)DlcTramp_Ta_ManageNaturalEffectInStandbyPhase2)(nSide, nID, eInfo);
}

static int nat_more(void *eInfo)
{
    int n = stacked(nat.side, nat.id, 2), v;

    while (nat.k < n && still(nat.side, nat.id, nat.card)) {
        pick(nat.side, nat.id, 2, nat.k++);
        v = ((NatFn)(nat.second ? DlcTramp_Ta_ManageNaturalEffectInStandbyPhase2
                                : DlcTramp_Ta_ManageNaturalEffectInStandbyPhase))(nat.side, nat.id, eInfo);
        pick_side = -1;
        if (v & 0xff) {
            return (v & 0xffff) | ((nat.k < n ? NAT_STEP + nat.k : nat.next) << 16);
        }
    }
    return nat.next << 16;
}

int DlcStack_Ta_Do_StandbyPhase(int PrNo, void *eInfo)
{
    int r;

    if (PrNo >= NAT_STEP && PrNo < NAT_STEP + DLC_STACK_MAX) {
        return nat_more(eInfo);
    }
    nat.hit = 0;
    r = ((StandbyFn)DlcTramp_Ta_Do_StandbyPhase)(PrNo, eInfo);
    if (nat.hit && (r >> 16) != 0xff && stacked(nat.side, nat.id, 2) > 0) {
        nat.card = SzDuel_GetUnitCardNo(nat.side, nat.id);
        nat.k = 0;
        nat.next = r >> 16;
        return (r & 0xffff) | (NAT_STEP << 16);
    }
    return r;
}
