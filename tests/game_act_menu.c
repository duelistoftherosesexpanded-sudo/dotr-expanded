/* Synthetic host tests for the game_act_menu group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/game_act_menu.h"

struct DotrActMenuState gSzGame_gl_actmenu;
unsigned char data_00317473_actmenu;
unsigned char data_0031747d_actmenu;
unsigned char DebugMenuOptions_actmenu[64];
char data_002be7d0_actmenu[] = "ENV";
char data_002be7d5_actmenu[] = "fmt%s%d";

enum {
    F_SETMODE = 1, F_KILL, F_DESTROY, F_ENTRY, F_PAUSE, F_SE, F_CONSTDECK, F_PUSH, F_ENV, F_FORMAT, F_READ,
    F_BIND, F_BGM, F_STOPBGM, F_TRANSFER, F_STOPSE, F_SUSPEND, F_INIT, F_STOPALL, F_TERM, F_OPTION,
    F_LEVEL, F_RANK, F_MOVIE, F_MERGE, F_MEMCARD
};

struct call {
    int fn;
    long a[6];
};

static struct call trace[2048];
static int ntrace;
static int memcard, fade_rest, inq_busy, read_ok[2], nread;
static int pressed[8][3];
static int npressed;
static struct DotrActMenuFile files[2];
static unsigned char env_dir[4];

static void add(int fn, long a, long b, long c, long d, long e, long f)
{
    assert(ntrace < 2048);
    trace[ntrace].fn = fn;
    trace[ntrace].a[0] = a;
    trace[ntrace].a[1] = b;
    trace[ntrace].a[2] = c;
    trace[ntrace].a[3] = d;
    trace[ntrace].a[4] = e;
    trace[ntrace].a[5] = f;
    ntrace++;
}

static int count(int fn)
{
    int i, n = 0;

    for (i = 0; i < ntrace; i++) {
        n += trace[i].fn == fn;
    }
    return n;
}

static struct call *nth(int fn, int k)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == fn && k-- == 0) {
            return &trace[i];
        }
    }
    assert(0);
    return 0;
}

static void press(int port, int kind, int mask)
{
    assert(npressed < 8);
    pressed[npressed][0] = port;
    pressed[npressed][1] = kind;
    pressed[npressed][2] = mask;
    npressed++;
}

int SzGame_ActMemCardProc(void) { return memcard; }
int SzFade_GetRestCount(int index) { assert(index == 0); return fade_rest; }
void SzGame_SetMode(int x, int y) { add(F_SETMODE, x, y, 0, 0, 0, 0); }
int SzFade_SetKillFlag(int index) { add(F_KILL, index, 0, 0, 0, 0, 0); return 0; }
int SzFade_Destroy(int index) { add(F_DESTROY, index, 0, 0, 0, 0, 0); return 0; }
int SzFade_Entry(int index, int kind, unsigned char a, unsigned char b, unsigned char frames, void *arg)
{
    add(F_ENTRY, index, kind, a, b, frames, arg != 0);
    return 0;
}
int SzFade_SetPauseFlag(int index, long long flag) { add(F_PAUSE, index, (long)flag, 0, 0, 0, 0); return 0; }

/* Non-zero only for an exact (port, kind, mask) the test pressed. */
int SzPad_Check(int port, int kind, int mask)
{
    int i;

    for (i = 0; i < npressed; i++) {
        if (pressed[i][0] == port && pressed[i][1] == kind && pressed[i][2] == mask) {
            return 1;
        }
    }
    return 0;
}

int SzSound_PlaySE(int index, int value) { add(F_SE, index, value, 0, 0, 0, 0); return 0; }
void SzData_SetupFromConstDeck(int deck, int kind) { add(F_CONSTDECK, deck, kind, 0, 0, 0, 0); }
void SzGame_Push(int mode, int arg) { add(F_PUSH, mode, arg, 0, 0, 0, 0); }

unsigned char *SzFile_GetEnv(DotrActMenuWord label)
{
    assert((char *)label == data_002be7d0_actmenu);
    add(F_ENV, 0, 0, 0, 0, 0, 0);
    return env_dir;
}

int func_0019b648(char *str, const char *fmt, ...)
{
    va_list ap;
    long dir;
    int n;

    va_start(ap, fmt);
    dir = va_arg(ap, long);
    n = va_arg(ap, int);
    va_end(ap);
    assert(fmt == data_002be7d5_actmenu && (unsigned char *)dir == env_dir);
    str[0] = (char)('0' + n % 10);
    str[1] = 0;
    add(F_FORMAT, n, 0, 0, 0, 0, 0);
    return 1;
}

DotrActMenuWord SzFile_Read(DotrActMenuWord name, DotrActMenuWord address, int size)
{
    int k = nread++;

    assert(size == -1 && name != 0);
    add(F_READ, ((char *)name)[0] - '0', address, 0, 0, 0, 0);
    return read_ok[k] ? (DotrActMenuWord)&files[k] : 0;
}

void RhText_BindAddress(int address) { add(F_BIND, address, 0, 0, 0, 0, 0); }
int SzSound_PlayBGM(int index, int flag) { add(F_BGM, index, flag, 0, 0, 0, 0); return 0; }
int SzSound_StopBGM(long long flag) { add(F_STOPBGM, (long)flag, 0, 0, 0, 0, 0); return 0; }
int SzSound_TransferSEGrp(int channel, int group, int flag) { add(F_TRANSFER, channel, group, flag, 0, 0, 0); return 0; }
int SzSound_StopSE(int channel, int index) { add(F_STOPSE, channel, index, 0, 0, 0, 0); return 0; }
void SD_SuspendSoundSystem(int on) { add(F_SUSPEND, on, 0, 0, 0, 0, 0); }
void SzSound_Init(void) { add(F_INIT, 0, 0, 0, 0, 0, 0); }
void SD_StopAll(void) { add(F_STOPALL, 0, 0, 0, 0, 0, 0); }
void SzSound_Term(void) { add(F_TERM, 0, 0, 0, 0, 0, 0); }
int SzData_SetOption(int side, int which, int value) { add(F_OPTION, side, which, value, 0, 0, 0); return 0; }
void SD_BGMSetSourceLevel(int level, int value) { add(F_LEVEL, level, value, 0, 0, 0, 0); }
int SzData_SetRankDirect(int side, int index, int value) { add(F_RANK, side, index, value, 0, 0, 0); return 0; }
void SzGame_PlayMovieNo(int movie, int a, int b) { add(F_MOVIE, movie, a, b, 0, 0, 0); }
int SzFile_InqRequestEx(void) { return inq_busy; }
int SzText_LoadMerge(int index, int flag) { add(F_MERGE, index, flag, 0, 0, 0, 0); return 0; }
void SzGame_InitMemCardProc(int kind) { add(F_MEMCARD, kind, 0, 0, 0, 0, 0); }

#define M gSzGame_gl_actmenu

/* Row `row` of page `page` shows `item`; nothing pressed. */
static void reset(int page, int row, int item)
{
    memset(&M, 0, sizeof(M));
    memset(DebugMenuOptions_actmenu, 0, sizeof(DebugMenuOptions_actmenu));
    memset(files, 0, sizeof(files));
    M.page = (unsigned char)page;
    M.selected = (unsigned char)row;
    data_0031747d_actmenu = (unsigned char)page;
    data_00317473_actmenu = (unsigned char)row;
    DebugMenuOptions_actmenu[(page & 7) * 20 + row] = (unsigned char)item;
    memcard = -1;
    fade_rest = 1;
    inq_busy = 0;
    read_ok[0] = read_ok[1] = 0;
    nread = 0;
    npressed = 0;
    ntrace = 0;
}

static void test_gates(void)
{
    reset(0, 0, 7);
    memcard = 0;
    press(0, 0, 0xc0);
    SzGame_ActMenu();
    assert(ntrace == 0);

    /* Fade finished: back to the saved mode with a new fade. */
    reset(0, 0, 7);
    fade_rest = 0;
    M.x = 3;
    M.y = -2;
    SzGame_ActMenu();
    assert(ntrace == 6);
    assert(trace[0].fn == F_SETMODE && trace[0].a[0] == 3 && trace[0].a[1] == -2);
    assert(trace[1].fn == F_KILL && trace[1].a[0] == 0);
    assert(trace[2].fn == F_DESTROY && trace[2].a[0] == 1);
    assert(trace[3].fn == F_ENTRY && trace[3].a[0] == 1 && trace[3].a[1] == 0 && trace[3].a[2] == 1);
    assert(trace[3].a[3] == 0 && trace[3].a[4] == 60 && trace[3].a[5] == 0);
    assert(trace[4].fn == F_KILL && trace[4].a[0] == 1);
    assert(trace[5].fn == F_PAUSE && trace[5].a[0] == 1 && trace[5].a[1] == 1);

    /* A row index of 29 or more falls back to the page's first row. */
    reset(1, 9, 0);
    DebugMenuOptions_actmenu[20] = 7;
    press(0, 0, 0xc0);
    SzGame_ActMenu();
    assert(M.selected == 0);
    assert(count(F_PUSH) == 1 && nth(F_PUSH, 0)->a[0] == 7);
}

static void test_navigation(void)
{
    /* Page 0 has 20 rows: up from row 0 wraps to 19. */
    reset(0, 0, 7);
    press(0, 2, 0x5000);
    press(0, 2, 0x1000);
    SzGame_ActMenu();
    assert(M.selected == 19 && count(F_SE) == 1 && nth(F_SE, 0)->a[1] == 3);
    assert(count(F_PUSH) == 0);

    /* Page 1 has 9 rows: down from row 8 wraps to 0. */
    reset(1, 8, 7);
    press(0, 2, 0x5000);
    SzGame_ActMenu();
    assert(M.selected == 0 && count(F_SE) == 1);

    /* Page toggles keep the upper bits. */
    reset(0xf8, 0, 7);
    press(0, 2, 0xa000);
    press(0, 2, 0x8000);
    SzGame_ActMenu();
    assert(M.page == 0xf9 && count(F_SE) == 1);
    reset(1, 0, 7);
    press(0, 2, 0xa000);
    SzGame_ActMenu();
    assert(M.page == 0 && count(F_SE) == 1);
}

static void edit(int item, unsigned int limit, unsigned int value, int mask)
{
    reset(0, 0, item);
    M.limits[item] = limit;
    M.values[item] = value;
    press(0, 2, mask);
}

static void test_values(void)
{
    /* Default: step within the limit. */
    edit(7, 5, 0, 4);
    SzGame_ActMenu();
    assert(M.values[7] == 4 && count(F_SE) == 1);
    edit(7, 5, 4, 8);
    SzGame_ActMenu();
    assert(M.values[7] == 0);
    /* No limit: nothing edited and no sound. */
    edit(7, 0, 3, 8);
    SzGame_ActMenu();
    assert(M.values[7] == 3 && count(F_SE) == 0);

    /* Items 3 and 4: low part modulo the limit, thousands toggled 0/1. */
    edit(3, 10, 1009, 8);
    press(0, 2, 1);
    SzGame_ActMenu();
    assert(M.values[3] == 0);
    edit(4, 10, 0, 4);
    press(0, 2, 2);
    SzGame_ActMenu();
    assert(M.values[4] == 1009);

    /* Items 17, 19 and 22: two decimal digits pairs. */
    edit(19, 1, 9900, 4);
    press(0, 2, 2);
    SzGame_ActMenu();
    assert(M.values[19] == 99);

    /* Item 23: thousands 0..99 and units 0..999. */
    edit(23, 1, 0, 4);
    press(0, 2, 1);
    SzGame_ActMenu();
    assert(M.values[23] == 99999);

    /* Item 24: two bytes. */
    edit(24, 1, 0xff00, 8);
    press(0, 2, 2);
    SzGame_ActMenu();
    assert(M.values[24] == 0x0001);

    /* Item 26 skips 0 (to limit - 1 down, 1 up); item 27 skips 0 to 12 down. */
    edit(26, 8, 1, 4);
    SzGame_ActMenu();
    assert(M.values[26] == 7);
    edit(26, 8, 7, 8);
    SzGame_ActMenu();
    assert(M.values[26] == 1);
    edit(27, 13, 1, 4);
    SzGame_ActMenu();
    assert(M.values[27] == 12);
    edit(27, 13, 12, 8);
    SzGame_ActMenu();
    assert(M.values[27] == 1);
}

static void act(int item, int port, int kind, int mask)
{
    reset(0, 0, item);
    press(port, kind, mask);
}

static void test_actions(void)
{
    int i;

    /* Items that only edit. */
    static const int quiet[] = { 2, 3, 4, 29, 30 };
    for (i = 0; i < 5; i++) {
        act(quiet[i], 0, 0, 0xc0);
        SzGame_ActMenu();
        assert(ntrace == 0);
    }

    /* Default and item 10: push the item. */
    act(9, 0, 0, 0xc0);
    SzGame_ActMenu();
    assert(nth(F_SE, 0)->a[1] == 4 && nth(F_PUSH, 0)->a[0] == 9 && nth(F_PUSH, 0)->a[1] == 0);
    act(10, 0, 0, 0xc0);
    SzGame_ActMenu();
    assert(nth(F_PUSH, 0)->a[0] == 10);
    act(10, 0, 0, 0x10);
    SzGame_ActMenu();
    assert(ntrace == 0);

    /* Item 6: 0x10 sets up a constant deck for item 1's opponent (0 -> deck 17, else + 26). */
    act(6, 0, 0, 0x10);
    M.values[6] = 5;
    SzGame_ActMenu();
    assert(nth(F_CONSTDECK, 0)->a[0] == 5 && nth(F_CONSTDECK, 0)->a[1] == 17 && nth(F_SE, 0)->a[1] == 4);
    act(6, 0, 0, 0x10);
    M.values[1] = 21;
    SzGame_ActMenu();
    assert(nth(F_CONSTDECK, 0)->a[1] == 47);
    act(6, 0, 0, 0x10);
    M.values[1] = 22;
    SzGame_ActMenu();
    assert(count(F_CONSTDECK) == 0 && nth(F_SE, 0)->a[1] == 6);
    act(6, 0, 0, 0xc0);
    SzGame_ActMenu();
    assert(nth(F_PUSH, 0)->a[0] == 6);

    /* Items 13 and 15 set their value to 1 first. */
    act(13, 0, 0, 0xc0);
    SzGame_ActMenu();
    assert(M.values[13] == 1 && nth(F_PUSH, 0)->a[0] == 13);
    act(15, 0, 0, 0xc0);
    SzGame_ActMenu();
    assert(M.values[15] == 1 && nth(F_PUSH, 0)->a[0] == 15);

    /* Item 16 plays its movie. */
    act(16, 0, 0, 0xc0);
    M.values[16] = 42;
    SzGame_ActMenu();
    assert(nth(F_MOVIE, 0)->a[0] == 42 && nth(F_MOVIE, 0)->a[1] == 1 && nth(F_MOVIE, 0)->a[2] == 1);

    /* Items 20 and 21 copy the value's first byte. */
    act(20, 0, 0, 0);
    M.values[20] = 0x1fe;
    SzGame_ActMenu();
    assert(M.mode == -2);
    act(21, 0, 0, 0);
    M.values[21] = 1;
    SzGame_ActMenu();
    assert(M.flags.enabled != 0);
    assert((((unsigned char *)&M)[0x22e] & 0x20) != 0);

    /* Item 22: play or stop the BGM. */
    act(22, 0, 0, 0xc0);
    M.values[22] = 1234;
    SzGame_ActMenu();
    assert(nth(F_BGM, 0)->a[0] == 1234 && nth(F_BGM, 0)->a[1] == 1);
    act(22, 0, 0, 0x20);
    SzGame_ActMenu();
    assert(nth(F_STOPBGM, 0)->a[0] == 0x100);

    /* Items 23 and 24: sound channel = item 23's thousands. */
    act(23, 0, 0, 0xc0);
    M.values[23] = 3045;
    SzGame_ActMenu();
    assert(nth(F_TRANSFER, 0)->a[0] == 3 && nth(F_TRANSFER, 0)->a[1] == 45 && nth(F_TRANSFER, 0)->a[2] == 1);
    act(24, 0, 0, 0xc0);
    M.values[23] = 3045;
    M.values[24] = 0x102;
    SzGame_ActMenu();
    assert(nth(F_SE, 0)->a[0] == 3 && nth(F_SE, 0)->a[1] == 0x102);
    act(24, 0, 0, 0x20);
    M.values[23] = 2000;
    M.values[24] = 7;
    SzGame_ActMenu();
    assert(nth(F_STOPSE, 0)->a[0] == 2 && nth(F_STOPSE, 0)->a[1] == 7);

    /* Item 25: sound system on or off, then option 2 always. */
    act(25, 0, 0, 0x10);
    M.values[25] = 1;
    SzGame_ActMenu();
    assert(trace[0].fn == F_SUSPEND && trace[0].a[0] == 0 && trace[1].fn == F_INIT);
    assert(trace[2].fn == F_OPTION && trace[2].a[1] == 2 && trace[2].a[2] == 1);
    act(25, 0, 0, 0x10);
    SzGame_ActMenu();
    assert(trace[0].fn == F_STOPALL && trace[1].fn == F_SUSPEND && trace[1].a[0] == 1 && trace[2].fn == F_TERM);
    act(25, 0, 0, 0);
    SzGame_ActMenu();
    assert(ntrace == 1 && trace[0].fn == F_OPTION);

    /* Item 26: BGM source level. */
    act(26, 0, 0, 0xc0);
    M.values[26] = 5;
    SzGame_ActMenu();
    assert(nth(F_LEVEL, 0)->a[0] == 5 && nth(F_LEVEL, 0)->a[1] == 0x80 && nth(F_SE, 0)->a[1] == 4);

    /* Item 27: every one of 854 ranks of both sides. */
    act(27, 0, 0, 0xc0);
    M.values[27] = 9;
    SzGame_ActMenu();
    assert(count(F_RANK) == 2 * 854);
    assert(nth(F_RANK, 0)->a[0] == 0 && nth(F_RANK, 1)->a[0] == 1 && nth(F_RANK, 1707)->a[1] == 853);
    assert(nth(F_RANK, 1707)->a[2] == 9);

    /* Items 31 and 32: options 1 and 0. */
    act(31, 0, 0, 0);
    M.values[31] = 1;
    SzGame_ActMenu();
    assert(nth(F_OPTION, 0)->a[1] == 1 && nth(F_OPTION, 0)->a[2] == 1);
    act(32, 0, 0, 0);
    SzGame_ActMenu();
    assert(nth(F_OPTION, 0)->a[1] == 0);

    /* Item 33: merge a text unless a file request is pending. */
    act(33, 0, 0, 0xc0);
    M.values[33] = 4;
    SzGame_ActMenu();
    assert(nth(F_MERGE, 0)->a[0] == 4 && nth(F_SE, 0)->a[1] == 4);
    act(33, 0, 0, 0xc0);
    inq_busy = 1;
    SzGame_ActMenu();
    assert(count(F_MERGE) == 0 && nth(F_SE, 0)->a[1] == 6);

    /* Items 34 and 35: memory-card process kinds 0 and 1. */
    act(34, 0, 0, 0xc0);
    SzGame_ActMenu();
    assert(nth(F_MEMCARD, 0)->a[0] == 0);
    act(35, 0, 0, 0xc0);
    SzGame_ActMenu();
    assert(nth(F_MEMCARD, 0)->a[0] == 1);
}

static void test_text_load(void)
{
    /* Both files: the second loads after the first, rounded up to 64 bytes. */
    act(17, 0, 0, 0xc0);
    M.values[17] = 304;
    read_ok[0] = read_ok[1] = 1;
    files[0].size = 100;
    SzGame_ActMenu();
    assert(count(F_ENV) == 1 && nth(F_FORMAT, 0)->a[0] == 4 && nth(F_FORMAT, 1)->a[0] == 3);
    assert(nth(F_READ, 0)->a[0] == 4 && nth(F_READ, 0)->a[1] == 0x4000000);
    assert(nth(F_READ, 1)->a[0] == 3 && nth(F_READ, 1)->a[1] == 0x4000000 + 128);
    assert(M.firstAddress == 0x4000000 && M.secondAddress == 0x4000000 + 128);
    assert(nth(F_BIND, 0)->a[0] == 0x4000000 && nth(F_BIND, 1)->a[0] == 0x4000000 + 128);
    assert(nth(F_SE, 0)->a[1] == 4 && nth(F_PUSH, 0)->a[0] == 17);

    /* Only the second (hundreds 0: no second file); nothing read: sound 6, addresses cleared. */
    act(17, 0, 0, 0xc0);
    M.values[17] = 5;
    M.firstAddress = 1;
    M.secondAddress = 1;
    SzGame_ActMenu();
    assert(count(F_READ) == 1 && M.firstAddress == 0 && M.secondAddress == 0);
    assert(count(F_PUSH) == 0 && nth(F_SE, 0)->a[1] == 6);

    /* First missing, second present: loaded at the base address. */
    act(17, 0, 0, 0xc0);
    M.values[17] = 105;
    read_ok[1] = 1;
    SzGame_ActMenu();
    assert(M.firstAddress == 0 && M.secondAddress == 0x4000000 && count(F_PUSH) == 1);
}

int main(void)
{
    test_gates();
    test_navigation();
    test_values();
    test_actions();
    test_text_load();
    return 0;
}
