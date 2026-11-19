/* Synthetic host tests for the finish_move group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/finish_move.h"

/* The moving unit, the cell, and the unit standing on the cell (map < 0: none). */
static int m_side, m_id, m_x, m_z, t_side, t_id, map;
#define ENEMY (m_side == 0 ? 1 : 0)
#define PACK(s, i) (((s) << 8) | (i))

/* Fixtures the stubs answer from, by (side, id). */
static int status[2][16], kind[2][16], control[2][16], curse[2][16], spell[2][16], unit[2][16];
static int ratec[2][16], goalc[2][16], ratem[2][16], goalm[2][16];
static unsigned eff3[2][16], eff4[2][16];
static int has3[2][16], has4[2][16];
static int m_loc, leader_loc, cur_side, trap_b[64], range_abl, ld_abl, resist;
static int act_v, call_v, call_no, agree;
static unsigned call_par;
static int call_sets;

/* What was called: one word per call, in order. */
static char seen[1024];
static int act_mode, dest_args[8], open_step, graves[4][2], grave_n, exp_args[4], visual[7];
static struct DotrFinishMoveInfo info;

static void said(const char *format, ...)
{
    va_list list;
    size_t used = strlen(seen);

    va_start(list, format);
    vsnprintf(seen + used, sizeof seen - used, format, list);
    va_end(list);
    used = strlen(seen);
    assert(used + 2 < sizeof seen);
    seen[used] = ' ';
    seen[used + 1] = 0;
}

/* 'n' for the moving unit, 't' for the unit on the cell. */
static char who(int a, int b)
{
    assert((a == 0 || a == 1) && b >= 0 && b < 16);
    if (a == m_side && b == m_id) {
        return 'n';
    }
    assert(map >= 0 && a == t_side && b == t_id);
    return 't';
}

int SzDuel_GetMapUnitID(int x, int y)
{
    assert(x == m_x && y == m_z);
    said("map");
    return map;
}
int SzDuel_GetUnitLoc(int a, int b)
{
    if (b == 0) {
        assert(a == cur_side);
        said("locL");
        return leader_loc;
    }
    said("loc%c", who(a, b));
    return m_loc;
}
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    said("st%d%c", bit, who(a, b));
    return (status[a][b] >> bit) & 1;
}
int Ta_IsUnitControl(int a, int b)
{
    said("ctl%c", who(a, b));
    return control[a][b];
}
int Ta_GetCurrentSide(int a, int b)
{
    said("cur%c", who(a, b));
    return cur_side;
}
int Ta_IsActiveTrapB(int side, int id)
{
    assert(id == 4 || id == 48 || id == 49 || id == 46);
    /* 46 is asked of the mover's opponent, the others of the mover's current side. */
    assert(side == (id == 46 ? ENEMY : cur_side));
    said("trap%d", id);
    return trap_b[id];
}
int Ta_IsLDRangeAvilityWithLocation(int a, int b, int c, int d, int e)
{
    assert(a == m_side && b == m_id && c == m_x && d == m_z && e == 11);
    said("range");
    return range_abl;
}
int Ta_IsLDAbl(int a, int b)
{
    assert(a == 9 && b == cur_side);
    said("abl");
    return ld_abl;
}
void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int k)
{
    said("visual");
    visual[0] = x;
    visual[1] = y;
    visual[2] = c;
    visual[3] = reach;
    visual[4] = e;
    visual[5] = mode;
    visual[6] = k;
}
int Ta_IsResistAll(int a, int b)
{
    assert(a == m_side && b == m_id);
    said("resist");
    return resist;
}
int SzDuel_GetUnitKind(int a, int b)
{
    said("kind%c", who(a, b));
    return kind[a][b];
}
unsigned *func_00250740_finishmove(int a, int b, int n)
{
    assert(n == 3 || n == 4);
    said("eff%d%c", n, who(a, b));
    if (n == 3) {
        return has3[a][b] ? &eff3[a][b] : NULL;
    }
    return has4[a][b] ? &eff4[a][b] : NULL;
}
void TaUtil_DoOpen(int a, int b, int mode)
{
    assert(mode == 1);
    said("open%c", who(a, b));
}
int SzSound_PlaySE(int index, int value)
{
    assert(index == 0 && value == 38);
    said("se");
    return 0;
}
int func_00263930_finishmove(unsigned *e, int a, int b, int c, int d, int x, int z,
                             struct DotrFinishMoveInfo *in, struct DotrFinishMoveSub *sub, int mode)
{
    /* The effect words of the first unit; the other unit; the cell; the record twice. */
    assert(e == &eff3[a][b] && has3[a][b]);
    assert(x == m_x && z == m_z && in == &info && sub == &info.sub);
    said("act%c%c", who(a, b), who(c, d));
    act_mode = mode;
    return act_v;
}
int SzDuel_GetUnitCurseRest(int a, int b)
{
    said("curse%c", who(a, b));
    return curse[a][b];
}
int SzDuel_GetUnitSpellRest(int a, int b)
{
    said("spell%c", who(a, b));
    return spell[a][b];
}
/* Battle, SelfHeap, Ta_IsVirusAll, Ta_Do_MoveEfc and Ta_Open answer call_v and, when
 * call_sets is on, write call_no and call_par into the record. */
static int answer(struct DotrFinishMoveInfo *in)
{
    assert(in == &info);
    if (call_sets) {
        in->eNO = call_no;
        in->dwPar = call_par;
    }
    return call_v;
}
int func_00246810_finishmove(int a, int b, int c, int d, int x, int z, struct DotrFinishMoveInfo *in)
{
    assert(who(a, b) == 't' && who(c, d) == 'n' && x == m_x && z == m_z);
    said("battle");
    return answer(in);
}
int func_00247f40_finishmove(int a, int b, int c, int d, struct DotrFinishMoveInfo *in)
{
    assert(who(a, b) == 't' && who(c, d) == 'n');
    said("heap");
    return answer(in);
}
int func_002517d0_finishmove(int a, int b, int c, int d, int x, int z, struct DotrFinishMoveInfo *in, int flag)
{
    assert(in == &info);
    said("dest");
    dest_args[0] = a;
    dest_args[1] = b;
    dest_args[2] = c;
    dest_args[3] = d;
    dest_args[4] = x;
    dest_args[5] = z;
    dest_args[6] = flag;
    return call_v;
}
void Ta_UnitFade(int side, int pos, int dir, int value, int flags)
{
    assert(dir == -1 && value == 1 && flags == 3);
    said("fade%c", who(side, pos));
}
int SzDuel_SetUnitCtlGoalC(int a, int b, int value)
{
    assert(value == 0);
    said("goal%c", who(a, b));
    return 0;
}
int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value)
{
    assert(pair == -1 && value == 0);
    said("rate%c", who(a, b));
    return 0;
}
int SzDuel_GetUnitCtlRateC(int a, int b) { said("rc%c", who(a, b)); return ratec[a][b]; }
int SzDuel_GetUnitCtlGoalC(int a, int b) { said("gc%c", who(a, b)); return goalc[a][b]; }
int SzDuel_GetUnitCtlRateM(int a, int b) { said("rm%c", who(a, b)); return ratem[a][b]; }
int SzDuel_GetUnitCtlGoalM(int a, int b) { said("gm%c", who(a, b)); return goalm[a][b]; }
int SzDuel_GetUnitCardNo(int a, int b)
{
    said("card%c", who(a, b));
    return 700 + a * 20 + b;
}
int SzDuel_AddGrave(int index, int value)
{
    assert(grave_n < 4);
    said("grave");
    graves[grave_n][0] = index;
    graves[grave_n][1] = value;
    grave_n++;
    return 0;
}
int SzDuel_DelUnit(int a, int b)
{
    said("del%c", who(a, b));
    return 0;
}
int SzData_CalcExp(int side, int id, int operation, int value)
{
    said("exp");
    exp_args[0] = side;
    exp_args[1] = id;
    exp_args[2] = operation;
    exp_args[3] = value;
    return 0;
}
int SzDuel_GetUnit(int a, int b)
{
    said("unit%c", who(a, b));
    return unit[a][b];
}
int SzEffect_SetBusyFlag2(int packed, int flag)
{
    assert(packed == PACK(m_side, m_id) && flag == 0);
    said("busy");
    return 0;
}
int func_0024feb0_finishmove(struct DotrFinishMoveInfo *in)
{
    said("virus");
    in->dwTo = 0xabc;
    return answer(in);
}
int Ta_GetAgreeOwnerIDon22bitFlagLst(int packed, unsigned word)
{
    assert(packed == PACK(m_side, m_id) && word == 0xabc);
    said("agree");
    return agree;
}
int func_0024f830_finishmove(int a, int b, int x, int y, struct DotrFinishMoveInfo *in)
{
    /* The mover's own location, not the target cell. */
    assert(who(a, b) == 'n' && x == (m_loc & 0xff) && y == m_loc >> 8);
    said("move");
    return answer(in);
}
int func_00252440_finishmove(int a, int b, struct DotrFinishMoveInfo *in, int n)
{
    assert(who(a, b) == 'n');
    said("topen");
    open_step = n;
    return answer(in);
}

/* Unit (side, 3) moves to (2, 5), where unit 4 of the other side stands. Nothing answers. */
static void reset(int side)
{
    m_side = side;
    m_id = 3;
    m_x = 2;
    m_z = 5;
    t_side = ENEMY;
    t_id = 4;
    map = PACK(t_side, t_id);
    memset(status, 0, sizeof status);
    memset(kind, 0, sizeof kind);
    memset(control, 0, sizeof control);
    memset(curse, 0, sizeof curse);
    memset(spell, 0, sizeof spell);
    memset(unit, 0, sizeof unit);
    memset(ratec, 0, sizeof ratec);
    memset(goalc, 0, sizeof goalc);
    memset(ratem, 0, sizeof ratem);
    memset(goalm, 0, sizeof goalm);
    memset(eff3, 0, sizeof eff3);
    memset(eff4, 0, sizeof eff4);
    memset(has3, 0, sizeof has3);
    memset(has4, 0, sizeof has4);
    memset(trap_b, 0xff, sizeof trap_b);
    control[m_side][m_id] = 1;
    control[t_side][t_id] = 2;
    kind[m_side][m_id] = 0x05;
    kind[t_side][t_id] = 0x07;
    m_loc = 0x0604;
    leader_loc = 0x0206;
    cur_side = side;
    range_abl = ld_abl = resist = 0;
    act_v = call_v = call_no = agree = call_sets = 0;
    call_par = 0;
    info.sub.h = 0x1234;
    info.sub.open = 0;
}

static int run(int pr)
{
    seen[0] = 0;
    act_mode = open_step = -99;
    grave_n = 0;
    memset(dest_args, 0x55, sizeof dest_args);
    memset(visual, 0x55, sizeof visual);
    info.eNO = -77;
    info.dwFrom = info.dwTo = info.dwPar = 0x77777777u;
    return Ta_FinishMove(m_side, m_id, m_x, m_z, &info, pr);
}

static int is(int no, unsigned from, unsigned to, unsigned par)
{
    return info.eNO == no && info.dwFrom == from && info.dwTo == to && info.dwPar == par && info.sub.h == 0x1234;
}
#define CLEARED is(999, 0, 0, 0)
#define SAW(text) (strcmp(seen, "map locn " text) == 0)

static void check_step0(void)
{
    int side;

    for (side = 0; side < 2; side++) {
        reset(side);
        /* Face down (bit 4 clear) with bit 2: the bit is set. */
        status[side][3] = 1 << 2;
        assert(run(0) == 1 && CLEARED && info.sub.open != 0 && SAW("st4n st2n "));
        /* Upper bits of PrNo do not change the step. */
        info.sub.open = 0;
        assert(run(0x1f00) == 1 && info.sub.open != 0);
        /* Bit 4 set: cleared without asking about bit 2. */
        status[side][3] = (1 << 4) | (1 << 2);
        assert(run(0) == 1 && CLEARED && info.sub.open == 0 && SAW("st4n "));
        /* Neither. */
        info.sub.open = -1;
        status[side][3] = 0;
        assert(run(0) == 1 && info.sub.open == 0 && SAW("st4n st2n "));
    }
    /* No unit on the cell: step 0 still runs. */
    reset(0);
    map = -1;
    status[0][3] = 1 << 2;
    assert(run(0) == 1 && info.sub.open != 0);
}

static void check_step1(void)
{
    static const struct {
        int id;
        unsigned par;
        int fires;
    } traps[3] = { { 4, 0x40000, 0x1010f }, { 48, 0x300000, 0x1000f }, { 49, 0x310000, 0x10003 } };
    int side, i;

    for (side = 0; side < 2; side++) {
        unsigned from, to;

        reset(side);
        from = (unsigned)PACK(t_side, t_id);
        to = (unsigned)PACK(m_side, m_id);
        /* Same controller: straight to step 17. */
        control[t_side][t_id] = 1;
        assert(run(1) == 17 && CLEARED && SAW("ctln ctlt "));
        control[t_side][t_id] = 2;
        /* No trap: step 3 after asking for 4, 48 and 49 in that order. */
        assert(run(1) == 3 && CLEARED && SAW("ctln ctlt curn locL curn trap4 curn trap48 curn trap49 "));
        /* The side asked is the mover's current side, whatever it is. */
        cur_side = ENEMY;
        assert(run(1) == 3 && CLEARED);
        cur_side = side;

        for (i = 0; i < 3; i++) {
            reset(side);
            trap_b[traps[i].id] = 5 + i;
            /* The trap fires. */
            assert(run(1) == traps[i].fires && is(54, from, to, (unsigned)(5 + i) | traps[i].par));
            assert(strstr(seen, "range resist ") || strstr(seen, "range trap46 resist "));
            assert(!strstr(seen, "visual"));
            /* Leader ability 11 in range: the range visual and effect 254 instead. */
            range_abl = 1;
            ld_abl = 6;
            cur_side = ENEMY;
            assert(run(1) == 0x10003 && is(254, 0, (unsigned)ENEMY << 8, 16));
            assert(strstr(seen, "range abl visual ") && !strstr(seen, "resist") && !strstr(seen, "trap46"));
            assert(visual[0] == 0x06 && visual[1] == 0x02 && visual[2] == 0 && visual[3] == 6);
            assert(visual[4] == 1 && visual[5] == 1 && visual[6] == 11);
            cur_side = side;
            range_abl = 0;
            /* Trap B 46 of the opponent answers with effect 102. */
            trap_b[46] = 9;
            assert(run(1) == 0x10003 && is(102, 1, 9, 0));
            if (i == 0) {
                /* Trap 4 asks 46 before the resistance. */
                assert(strstr(seen, "trap4 range trap46 ") && !strstr(seen, "resist"));
            } else {
                assert(strstr(seen, "range resist trap46 "));
            }
            /* The mover resists everything. */
            resist = 1;
            if (i == 0) {
                assert(run(1) == 0x10003 && is(102, 1, 9, 0));
                trap_b[46] = -1;
                /* Trap 4 still fills the record but returns to step 3. */
                assert(run(1) == 0x10003 && is(54, from, to, 5 | 0x40000));
            } else {
                assert(run(1) == 3 && CLEARED && !strstr(seen, "trap46"));
                trap_b[46] = -1;
                assert(run(1) == 3 && CLEARED);
            }
            /* Index 0 counts as an active trap. */
            resist = 0;
            trap_b[traps[i].id] = 0;
            assert(run(1) == traps[i].fires && is(54, from, to, traps[i].par));
        }
        /* The first active trap wins. */
        reset(side);
        trap_b[4] = 1;
        trap_b[48] = 2;
        trap_b[49] = 3;
        assert(run(1) == 0x1010f && is(54, from, to, 1 | 0x40000) && !strstr(seen, "trap48"));
        trap_b[4] = -1;
        assert(run(1) == 0x1000f && is(54, from, to, 2 | 0x300000) && !strstr(seen, "trap49"));
    }
}

struct word_case {
    int v;
    int want;
};

static void check_step3(void)
{
    static const struct word_case words[] = {
        { 0x007, 0x70002 }, { (int)0x80000007u, 0x70002 }, { 0x0ff, 0xff0002 },
        { 0x807, 0x7000e }, { 0xf07, 0x7000e }, { 0x107, 0x7000e }, { 0x307, 0x7000e },
        { 0x207, 0x7010f }, { 0x607, 0x7030f }, { 0x407, 0x7020b }, { 0x1007, 14 },
    };
    int side;
    size_t i;

    for (side = 0; side < 2; side++) {
        reset(side);
        /* No unit on the cell. */
        map = -1;
        assert(run(3) == 18 && CLEARED && SAW(""));
        map = PACK(t_side, t_id);
        /* Same controller. */
        control[t_side][t_id] = 1;
        assert(run(3) == 5 && CLEARED && SAW("ctln ctlt "));
        control[t_side][t_id] = 2;
        /* The target's kind class is not 0. */
        kind[t_side][t_id] = 0x27;
        assert(run(3) == 2 && SAW("ctln ctlt kindt "));
        kind[t_side][t_id] = 0x1f;
        /* The mover's is not 0. */
        kind[m_side][m_id] = 0x65;
        assert(run(3) == 6 && SAW("ctln ctlt kindt kindn "));
        kind[m_side][m_id] = 0x05;
        /* The target has no effect: it is opened with the sound, when face down. */
        assert(run(3) == 2 && CLEARED && SAW("ctln ctlt kindt kindn eff3t st4t opent se "));
        status[t_side][t_id] = 1 << 4;
        assert(run(3) == 2 && SAW("ctln ctlt kindt kindn eff3t st4t "));
        /* It has one and is face up: opened again without the sound, no effect run. */
        has3[t_side][t_id] = 1;
        assert(run(3) == 2 && CLEARED && SAW("ctln ctlt kindt kindn eff3t st4t opent "));
        /* Face down with an effect: opened, then Ta_Manage_ActEfc with mode 1. */
        status[t_side][t_id] = 0;
        for (i = 0; i < sizeof words / sizeof words[0]; i++) {
            act_v = words[i].v;
            assert(run(3) == words[i].want && CLEARED && act_mode == 1);
            assert(SAW("ctln ctlt kindt kindn eff3t st4t st4t opent se acttn "));
        }
    }
    /* The leader (id 0) is never opened. */
    reset(0);
    t_id = 0;
    map = PACK(t_side, 0);
    control[t_side][0] = 2;
    assert(run(3) == 2 && SAW("ctln ctlt kindt kindn eff3t "));
    status[t_side][0] = 1 << 4;
    has3[t_side][0] = 1;
    assert(run(3) == 2 && SAW("ctln ctlt kindt kindn eff3t st4t "));
    status[t_side][0] = 0;
    act_v = 0x207;
    assert(run(3) == 0x7010f && SAW("ctln ctlt kindt kindn eff3t st4t acttn "));
}

static void check_step2(void)
{
    static const struct word_case closed[] = {
        { 0x007, 0x70006 }, { (int)0x80000007u, 0x70006 }, { 0x807, 0x7000e }, { 0x107, 0x7020e },
        { 0x407, 0x7010f }, { 0x607, 0x7030f }, { 0x207, 0x7020b }, { 0x1007, 6 },
    };
    static const struct word_case opened[] = {
        { 0x007, 0x70006 }, { (int)0x80000007u, 0x70006 }, { 0x807, 0x7000e }, { 0x107, 0x7000e },
        { 0x407, 0x7010f }, { 0x607, 0x7030f }, { 0x207, 0x7020b }, { 0x1007, 6 },
    };
    int side;
    size_t i;

    for (side = 0; side < 2; side++) {
        reset(side);
        /* A curse or a spell running, or status bits 5 and 3 together. */
        curse[side][3] = 1;
        assert(run(2) == 14 && CLEARED && SAW("cursen "));
        curse[side][3] = 0;
        spell[side][3] = 2;
        assert(run(2) == 14 && SAW("cursen spelln "));
        spell[side][3] = 0;
        status[side][3] = (1 << 5) | (1 << 3);
        assert(run(2) == 14 && SAW("cursen spelln st5n st3n "));
        /* One of the two bits is not enough. */
        status[side][3] = 1 << 5;
        kind[side][3] = 0x25;
        assert(run(2) == 6 && SAW("cursen spelln st5n st3n kindn "));
        status[side][3] = 1 << 3;
        assert(run(2) == 6 && SAW("cursen spelln st5n kindn "));
        status[side][3] = 0;
        kind[side][3] = 0x05;

        /* Face up: nothing is opened. */
        status[side][3] = 1 << 4;
        has3[side][3] = 1;
        assert(run(2) == 6 && CLEARED && !strstr(seen, "open") && !strstr(seen, "eff3"));
        status[side][3] = 0;
        /* No effect: opened with the sound unless the target's kind class is not 0. */
        has3[side][3] = 0;
        assert(run(2) == 6 && SAW("cursen spelln st5n kindn st4n eff3n kindt st4n openn se "));
        kind[t_side][t_id] = 0x47;
        assert(run(2) == 6 && SAW("cursen spelln st5n kindn st4n eff3n kindt "));
        kind[t_side][t_id] = 0x07;

        /* An effect, the record's bit clear. */
        has3[side][3] = 1;
        for (i = 0; i < sizeof closed / sizeof closed[0]; i++) {
            act_v = closed[i].v;
            assert(run(2) == closed[i].want && CLEARED && act_mode == 1);
            assert(SAW("cursen spelln st5n kindn st4n eff3n kindt st4n openn se actnt "));
        }
        kind[t_side][t_id] = 0x47;
        assert(run(2) == 6 && !strstr(seen, "act") && !strstr(seen, "open"));

        /* The record's bit set: the target's kind is not asked. */
        info.sub.open = -1;
        for (i = 0; i < sizeof opened / sizeof opened[0]; i++) {
            act_v = opened[i].v;
            assert(run(2) == opened[i].want && CLEARED && act_mode == 1 && info.sub.open != 0);
            assert(SAW("cursen spelln st5n kindn st4n eff3n st4n openn se actnt "));
        }
    }
}

static void check_step17(void)
{
    static const struct word_case words[] = {
        { 0x007, 0x70005 }, { (int)0x80000007u, 0x70005 }, { 0x807, 0x7010e }, { 0x107, 5 },
        { 0x407, 0x7010f }, { 0x607, 0x7030f }, { 0x207, 0x7020b }, { 0x1007, 5 },
    };
    int side;
    size_t i;

    for (side = 0; side < 2; side++) {
        reset(side);
        has3[side][3] = 1;
        act_v = 0x807;
        /* The record's bit clear, or a kind class other than 0: nothing. */
        assert(run(17) == 5 && CLEARED && SAW("kindn "));
        info.sub.open = -1;
        kind[side][3] = 0x25;
        assert(run(17) == 5 && SAW("kindn "));
        kind[side][3] = 0x05;
        /* Opened; no effect. */
        has3[side][3] = 0;
        assert(run(17) == 5 && CLEARED && SAW("kindn st4n openn se eff3n "));
        has3[side][3] = 1;
        for (i = 0; i < sizeof words / sizeof words[0]; i++) {
            act_v = words[i].v;
            assert(run(17) == words[i].want && CLEARED && act_mode == 2);
            assert(SAW("kindn st4n openn se eff3n actnt "));
        }
        /* Already face up: no open, the effect still runs. */
        status[side][3] = 1 << 4;
        act_v = 0x407;
        assert(run(17) == 0x7010f && SAW("kindn st4n eff3n actnt "));
    }
}

static void check_battle(void)
{
    static const struct {
        int no;
        unsigned par;
        int v;
        int want;
    } battle[] = {
        { 93, 0, 5, 0x50010 },
        { 94, 1u << 18, 5, 0x70208 },
        { 94, (1u << 18) | (1u << 19) | (1u << 27) | (2u << 25), 5, 0x71708 },
        { 94, (1u << 19) | (3u << 25), 5, 0x7190a },
        { 94, (1u << 19) | (1u << 27), 5, 0x7050a },
        { 94, (1u << 27) | (3u << 25) | 0xffff, 5, 0x70010 },
        { 95, 1, 5, 0x50110 }, { 95, 3, 5, 0x50110 }, { 95, 2, 5, 0x5020b },
        { 36, 0, 0x10005, 0x50110 }, { 36, 0, 5, 0x5020b },
        { 37, 0, 0x10005, 0x50110 }, { 37, 0, 5, 0x5020b },
        { 43, 0, 0x30005, 0x50110 }, { 43, 0, 5, 0x5020b },
        { 999, 0, 5, 15 }, { 35, 0, 5, 15 }, { 44, 1, 0x10005, 15 }, { 96, 1, 5, 15 },
    }, heap[] = {
        { 95, 1, 5, 0x5010f }, { 95, 3, 5, 0x5020b }, { 95, 0, 5, 0x5020b },
        { 100, 999, 5, 0x5000b }, { 100, 0, 5, 0x5000b },
        { 999, 0, 0x10005, 0x50110 }, { 999, 0, 0x20005, 0x5020b }, { 999, 0, 5, 0x5020b },
        { 94, 1, 0x10005, 0x50110 },
    };
    int side;
    size_t i;

    for (side = 0; side < 2; side++) {
        reset(side);
        call_sets = 1;
        for (i = 0; i < sizeof battle / sizeof battle[0]; i++) {
            call_no = battle[i].no;
            call_par = battle[i].par;
            call_v = battle[i].v;
            assert(run(6) == battle[i].want && SAW("battle "));
            assert(is(battle[i].no, 0, 0, battle[i].par));
            /* The flag bits of PrNo are not used by this step. */
            assert(run(6 | 0x1f00) == battle[i].want);
        }
        for (i = 0; i < sizeof heap / sizeof heap[0]; i++) {
            call_no = heap[i].no;
            call_par = heap[i].par;
            call_v = heap[i].v;
            assert(run(5) == heap[i].want && SAW("heap "));
            assert(is(heap[i].no, 0, 0, heap[i].par));
        }
    }
}

static int dest_is(int a, int b, int c, int d, int flag)
{
    return dest_args[0] == a && dest_args[1] == b && dest_args[2] == c && dest_args[3] == d
        && dest_args[4] == m_x && dest_args[5] == m_z && dest_args[6] == flag;
}

static void check_dest(void)
{
    /* Step 8: the result word, PrNo's flag bits, the result and whether the target fades. */
    static const struct {
        int v;
        int flags;
        int want;
        int fades;
    } eight[] = {
        { 0x407, 0, 0x7020f, 1 }, { 0x407, 0x100, 0x70309, 1 }, { 0x407, 0x1c00, 0x71e0f, 1 },
        { 0x707, 0x100, 0x70309, 1 },
        { 0x207, 0, 0x7030f, 1 }, { 0x207, 0x100, 0x7030f, 1 }, { 0x307, 0x400, 0x7070f, 1 },
        { 0x107, 0, 0x7000b, 0 }, { 0x107, 0x100, 0x70109, 0 }, { 0x107, 0x800, 0x7080b, 0 },
        { 0x007, 0x100, 0x70309, 1 }, { 0x007, 0, 0x7020b, 1 }, { 0x007, 0x1000, 0x71210, 1 },
        { 0x807, 0x200, 0x7020b, 1 }, { 0x007, 0x1c00, 0x71e10, 1 },
    };
    int side;
    size_t i;

    for (side = 0; side < 2; side++) {
        reset(side);
        for (i = 0; i < sizeof eight / sizeof eight[0]; i++) {
            call_v = eight[i].v;
            assert(run(8 | eight[i].flags) == eight[i].want && CLEARED);
            assert(dest_is(t_side, t_id, m_side, m_id, (eight[i].flags >> 8) & 1));
            assert(eight[i].fades ? SAW("dest fadet goalt ratet ") : SAW("dest "));
        }

        /* Step 9: the mover alone; it fades unless bit 8 of the result is set. */
        call_v = 0x007;
        assert(run(9) == 0x7010f && CLEARED && dest_is(m_side, m_id, -1, -1, 0) && SAW("dest faden goaln raten "));
        assert(run(9 | 0x200) == 0x7030f && dest_is(m_side, m_id, -1, -1, 1));
        assert(run(9 | 0x1d00) == 0x71d0f && dest_is(m_side, m_id, -1, -1, 0));
        call_v = 0x107;
        assert(run(9) == 0x7000f && SAW("dest "));
        assert(run(9 | 0x1e00) == 0x71e0f && dest_is(m_side, m_id, -1, -1, 1) && SAW("dest "));
        call_v = 0x607;
        assert(run(9) == 0x7010f && SAW("dest faden goaln raten "));

        /* Step 10: the mover against the target. */
        call_v = 0x107;
        assert(run(10) == 0x70010 && CLEARED && dest_is(m_side, m_id, t_side, t_id, 0) && SAW("dest "));
        assert(run(10 | 0x200) == 0x70210 && dest_is(m_side, m_id, t_side, t_id, 1));
        assert(run(10 | 0x1d00) == 0x71c10);
        call_v = 0x007;
        assert(run(10) == 0x70110 && SAW("dest faden goaln raten "));
        assert(run(10 | 0x1e00) == 0x71f10 && dest_is(m_side, m_id, t_side, t_id, 1));
    }
}

#define KILL_T "cardt grave eff4t delt cardt exp "
#define KILL_N "cardn grave eff4n deln cardn exp "
#define STILL_T "rct gct rmt gmt "
#define STILL_N "rcn gcn rmn gmn "

static void check_removal(void)
{
    int side, step;

    for (side = 0; side < 2; side++) {
        int tcard, ncard;

        reset(side);
        tcard = 700 + t_side * 20 + t_id;
        ncard = 700 + m_side * 20 + m_id;

        /* Step 11 with neither flag: only the mover's state is asked. */
        assert(run(11) == 14 && CLEARED && SAW("unitn "));
        unit[side][3] = 1;
        assert(run(11) == 0x6000c && SAW("unitn cursen busy "));
        assert(run(11 | 0x1c00) == 0x61c0c);
        curse[side][3] = 1;
        assert(run(11) == 14 && SAW("unitn cursen "));
        assert(run(11 | 0x1c00) == 0x1c0e);
        curse[side][3] = 0;

        for (step = 11; step <= 15; step += 4) {
            const char *busy = step == 15 ? "busy " : "";
            char want[256];

            reset(side);
            /* Bit 9: the target is still moving (either pair differs). */
            ratec[t_side][t_id] = 3;
            assert(run(step | 0x200) == (0x200 | step) && CLEARED && SAW("rct gct "));
            assert(run(step | 0x300) == (0x300 | step));
            assert(run(step | 0x1e00) == (0x1e00 | step));
            ratec[t_side][t_id] = 0;
            goalm[t_side][t_id] = 3;
            assert(run(step | 0x200) == (0x200 | step) && SAW(STILL_T));
            goalm[t_side][t_id] = 0;
            /* Bit 8: the mover is. */
            goalc[side][3] = 1;
            assert(run(step | 0x100) == (0x100 | step) && CLEARED);
            snprintf(want, sizeof want, "map locn %srcn gcn ", busy);
            assert(strcmp(seen, want) == 0);
            assert(run(step | 0x1f00) == (0x1f00 | step));
            goalc[side][3] = 0;
            ratem[side][3] = 1;
            assert(run(step | 0x300) == (0x300 | step));
            snprintf(want, sizeof want, "map locn " STILL_T "%s" STILL_N, busy);
            assert(strcmp(seen, want) == 0);
            ratem[side][3] = 0;

            /* Bit 9 at rest: the target goes to the graveyard and is deleted. */
            assert(run(step | 0x200) == (step == 11 ? 0x20e : 15) && CLEARED);
            snprintf(want, sizeof want, "map locn " STILL_T "%s" KILL_T "%s", busy, step == 11 ? "unitn " : "");
            assert(strcmp(seen, want) == 0);
            assert(grave_n == 1 && graves[0][0] == t_side && graves[0][1] == tcard);
            assert(exp_args[0] == t_side && exp_args[1] == tcard && exp_args[2] == 1 && exp_args[3] == -4);
            /* Bit 8 at rest: the mover. */
            assert(run(step | 0x100) == (step == 11 ? 0x10e : 15) && CLEARED);
            snprintf(want, sizeof want, "map locn %s" STILL_N KILL_N "%s", busy, step == 11 ? "unitn " : "");
            assert(strcmp(seen, want) == 0);
            assert(grave_n == 1 && graves[0][0] == m_side && graves[0][1] == ncard);
            assert(exp_args[0] == m_side && exp_args[1] == ncard && exp_args[2] == 1 && exp_args[3] == -4);
            /* Both: the target first. */
            assert(run(step | 0x300) == (step == 11 ? 0x30e : 15));
            snprintf(want, sizeof want, "map locn " STILL_T "%s" STILL_N KILL_T KILL_N "%s", busy,
                     step == 11 ? "unitn " : "");
            assert(strcmp(seen, want) == 0);
            assert(grave_n == 2 && graves[0][0] == t_side && graves[1][0] == m_side);

            /* Bit 10: no graveyard entry for the card itself. */
            assert(run(step | 0x600) == (step == 11 ? 0x60e : 15));
            assert(grave_n == 0 && strstr(seen, "eff4t delt cardt exp ") && !strstr(seen, "grave"));
            /* Effect word 81 above the low byte: card 672 to the current side's graveyard. */
            has4[t_side][t_id] = 1;
            eff4[t_side][t_id] = (81u << 8) | 0x33;
            cur_side = 1;
            assert(run(step | 0x200) == (step == 11 ? 0x20e : 15));
            assert(strstr(seen, "cardt grave eff4t curt grave delt cardt exp "));
            assert(grave_n == 2 && graves[0][1] == tcard && graves[1][0] == 1 && graves[1][1] == 672);
            assert(run(step | 0x600) == (step == 11 ? 0x60e : 15));
            assert(grave_n == 1 && graves[0][0] == 1 && graves[0][1] == 672);
            /* Another word: nothing more. */
            eff4[t_side][t_id] = (80u << 8) | 81;
            assert(run(step | 0x200) == (step == 11 ? 0x20e : 15) && grave_n == 1 && !strstr(seen, "curt"));
            eff4[t_side][t_id] = (81u << 8) | (1u << 16);
            assert(run(step | 0x200) == (step == 11 ? 0x20e : 15) && grave_n == 1);
            cur_side = side;
            has4[side][3] = 1;
            eff4[side][3] = 81u << 8;
            assert(run(step | 0x100) == (step == 11 ? 0x10e : 15));
            assert(grave_n == 2 && graves[1][0] == side && graves[1][1] == 672);
        }

        /* Step 11 after the removals: a mover that still exists without a curse goes to 12. */
        reset(side);
        unit[side][3] = 1;
        assert(run(11 | 0x200) == 0x6020c && CLEARED);
        assert(SAW(STILL_T KILL_T "unitn cursen busy "));
        assert(run(11 | 0x300) == 0x6020c);
        assert(run(11 | 0x1f00) == 0x61e0c);
        curse[side][3] = 4;
        assert(run(11 | 0x300) == 0x30e);

        /* Step 15 with neither flag. */
        reset(side);
        assert(run(15) == 14 && CLEARED && SAW("busy "));
        assert(run(15 | 0x1c00) == 14);
    }
}

static void check_tail(void)
{
    static const int others[] = { 4, 7, 18, 19, 200, 255 };
    int side;
    size_t i;

    for (side = 0; side < 2; side++) {
        reset(side);
        call_v = 7;
        /* Step 12: the virus test, then whether the mover is in the list it left in dwTo. */
        agree = 1;
        assert(run(12) == 0x7000e && is(999, 0, 0xabc, 0) && SAW("virus agree "));
        assert(run(12 | 0x200) == 0x7020e);
        assert(run(12 | 0x1d00) == 0x71c0e);
        agree = 0;
        assert(run(12) == 0x7000d && SAW("virus agree "));
        assert(run(12 | 0x1f00) == 0x71e0d);
        /* Step 16: the virus test alone. */
        assert(run(16) == 0x7000f && SAW("virus "));
        assert(run(16 | 0x300) == 0x7030f);
        assert(run(16 | 0x1f00) == 0x71f0f);
        call_v = 0x1ff;
        assert(run(16) == 0xff000f);
        /* Step 13: the move effect at the mover's own location. */
        call_v = 7;
        assert(run(13) == 0x70012 && CLEARED && SAW("move "));
        assert(run(13 | 0x200) == 0x70212);
        assert(run(13 | 0x1d00) == 0x71c12);
        m_loc = 0x0305;
        assert(run(13) == 0x70012);
        /* Step 14: no call at all. */
        assert(run(14) == 14 && CLEARED && SAW(""));
        assert(run(14 | 0x300) == 0x30e);
        assert(run(14 | 0x1c00) == 14);
        assert(run(14 | 0x1e00) == 0x20e);

        /* Steps 4, 7, 18 and everything past 18. */
        for (i = 0; i < sizeof others / sizeof others[0]; i++) {
            int step = others[i];

            reset(side);
            call_v = 5;
            /* Kind class 0: nothing. */
            status[side][3] = (1 << 4) | (1 << 2);
            assert(run(step) == 14 && CLEARED && SAW("kindn "));
            assert(run(step | 0x300) == 0x30e);
            assert(run(step | 0x1c00) == 14);
            kind[side][3] = 0x45;
            assert(run(step) == 14 && SAW("kindn "));
            /* Class 1 needs status bit 4 or bit 2. */
            kind[side][3] = 0x25;
            status[side][3] = 0;
            assert(run(step) == 14 && SAW("kindn st4n st2n "));
            status[side][3] = 1 << 4;
            assert(run(step) == 0x5010e && CLEARED && open_step == step - 17 && SAW("kindn st4n topen "));
            assert(run(step | 0x200) == 0x5030e);
            assert(run(step | 0x1d00) == 0x5010e);
            /* Class 5 with bit 2 alone. */
            kind[side][3] = 0xa5;
            status[side][3] = 1 << 2;
            assert(run(step) == 0x5010e && open_step == step - 17 && SAW("kindn st4n st2n topen "));
            /* A result with a high half: 17 is added to it for the next step. */
            call_v = 0x20005;
            assert(run(step) == 0x50113 && CLEARED);
            assert(run(step | 0x200) == 0x50313);
            call_v = 0x10000;
            assert(run(step) == 0x112);
        }
    }
}

int main(void)
{
    assert(offsetof(struct DotrFinishMoveInfo, dwPar) == 12 && offsetof(struct DotrFinishMoveInfo, sub) == 16);
    assert(sizeof(struct DotrFinishMoveSub) == 4 && sizeof(struct DotrFinishMoveInfo) == 20);
    check_step0();
    check_step1();
    check_step3();
    check_step2();
    check_step17();
    check_battle();
    check_dest();
    check_removal();
    check_tail();
    puts("finish_move: ok");
    return 0;
}
