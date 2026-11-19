/* Synthetic host tests for the link_step group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/link_step.h"

struct DotrMiscLink data_0034fb00;

static int poll_calls, status_calls, status_result, status_20, status_24;
static int poll_10, poll_14, poll_18;
static int reset_mode, reset_status[2];
int func_00194380(int a, int b, int *c, int *d, int *e)
{
    if (reset_mode) {
        assert(a == poll_calls && b == data_0034fb00.unknown_0c);
        poll_calls++;
        return 77;
    }
    assert(a == data_0034fb00.unknown_08 && b == data_0034fb00.unknown_0c);
    assert(c == &data_0034fb00.unknown_10 && d == &data_0034fb00.unknown_14);
    assert(e == &data_0034fb00.unknown_18);
    *c = poll_10;
    *d = poll_14;
    *e = poll_18;
    poll_calls++;
    return 77;
}
int func_00194248(int a, int *b, int *c)
{
    if (reset_mode) {
        assert(a == 0 && b == &data_0034fb00.unknown_20 && status_calls == poll_calls - 1);
        *c = reset_status[status_calls++];
        return 0;
    }
    assert(a == 1 && b == &data_0034fb00.unknown_20 && c == &data_0034fb00.unknown_24);
    *b = status_20;
    *c = status_24;
    status_calls++;
    return status_result;
}

static int (*link_step)(int) = TakuCustom_card_kansi;
static int link_mode = 5;

static int link_run(int w08, int w10, int w18, int w24, int state, int count)
{
    memset(&data_0034fb00, 0, sizeof(data_0034fb00));
    data_0034fb00.unknown_08 = w08;
    data_0034fb00.unknown_30 = state;
    data_0034fb00.unknown_34 = 9;
    data_0034fb00.unknown_40 = count;
    poll_10 = w10;
    poll_18 = w18;
    status_24 = w24;
    return link_step(link_mode);
}

static int link_is(int state, int count)
{
    return data_0034fb00.unknown_30 == state && data_0034fb00.unknown_34 == 0
        && data_0034fb00.unknown_40 == count;
}

/* TakuCustom_card_kansi (branch coverage) with the two SDK-block callees stubbed. */
static void check_link_poll(void)
{
    /* both results are stored; a status result other than 1 returns 0 */
    status_result = 0;
    status_20 = 1;
    poll_calls = status_calls = 0;
    assert(link_run(0, 0, 0, 0, 7, 1) == 0 && poll_calls == 1 && status_calls == 1);
    assert(data_0034fb00.unknown_00 == 77 && data_0034fb00.unknown_04 == 0);
    assert(data_0034fb00.unknown_30 == 7 && data_0034fb00.unknown_34 == 9);
    /* status word not 1 */
    status_result = 1;
    status_20 = 0;
    assert(link_run(0, 0, 0, 0, 7, 1) == 0 && data_0034fb00.unknown_30 == 7);
    assert(data_0034fb00.unknown_04 == 1);
    status_20 = 1;
    /* second status word: -1 and -2 give 0x700, other non-zero values go on */
    assert(link_run(0, 2, 5, -1, 7, 1) == 0x700 && link_is(3, 0));
    assert(link_run(0, 2, 5, -2, 7, 1) == 0x700 && link_is(3, 0));
    assert(link_run(0, 4, 0, 6, 7, 1) == 2 && link_is(3, 0));
    /* first polled word */
    assert(link_run(0, 0, 0, 0, 7, 1) == 2 && link_is(3, 0));
    assert(link_run(0, 1, 0, 0, 7, 1) == 4 && link_is(3, 0));
    assert(link_run(1, 1, 0, 0, 7, 1) == 0x400 && link_is(3, 0));
    assert(link_run(0, 3, 0, 0, 7, 1) == 4 && link_is(3, 0));
    assert(link_run(1, 3, 0, 0, 7, 1) == 0x500 && link_is(3, 0));
    assert(link_run(0, 4, 0, 0, 7, 1) == 2 && link_is(3, 0));
    /* first word 2, third word 0: the count reaches 3 */
    assert(link_run(0, 2, 0, 0, 7, 1) == 0 && data_0034fb00.unknown_40 == 2);
    assert(data_0034fb00.unknown_30 == 7 && data_0034fb00.unknown_34 == 9);
    assert(link_run(0, 2, 0, 0, 7, 2) == 3 && link_is(3, 3));
    /* third word 1: only from state 0 */
    assert(link_run(0, 2, 1, 0, 7, 1) == 0 && data_0034fb00.unknown_30 == 7);
    assert(link_run(0, 2, 1, 0, 0, 1) == 16 && link_is(1, 0));
    /* other third words */
    assert(link_run(0, 2, 2, 0, 7, 1) == 2 && link_is(3, 0));
}

/* TakuLoad_card_kansi (branch coverage), same stubs: the 1 and 3 cases give 4 whatever
 * the word at +8 holds. */
static void check_link_poll_plain(void)
{
    link_step = TakuLoad_card_kansi;
    status_result = 0;
    status_20 = 1;
    poll_calls = status_calls = 0;
    assert(link_run(0, 0, 0, 0, 7, 1) == 0 && poll_calls == 1 && status_calls == 1);
    assert(data_0034fb00.unknown_00 == 77 && data_0034fb00.unknown_04 == 0);
    assert(data_0034fb00.unknown_30 == 7 && data_0034fb00.unknown_34 == 9);
    status_result = 1;
    status_20 = 0;
    assert(link_run(0, 0, 0, 0, 7, 1) == 0 && data_0034fb00.unknown_30 == 7);
    status_20 = 1;
    assert(link_run(0, 2, 5, -1, 7, 1) == 0x700 && link_is(3, 0));
    assert(link_run(0, 2, 5, -2, 7, 1) == 0x700 && link_is(3, 0));
    assert(link_run(0, 4, 0, 6, 7, 1) == 2 && link_is(3, 0));
    assert(link_run(0, 0, 0, 0, 7, 1) == 2 && link_is(3, 0));
    assert(link_run(0, 1, 0, 0, 7, 1) == 4 && link_is(3, 0));
    assert(link_run(1, 1, 0, 0, 7, 1) == 4 && link_is(3, 0));
    assert(link_run(0, 3, 0, 0, 7, 1) == 4 && link_is(3, 0));
    assert(link_run(1, 3, 0, 0, 7, 1) == 4 && link_is(3, 0));
    assert(link_run(0, 4, 0, 0, 7, 1) == 2 && link_is(3, 0));
    assert(link_run(0, 2, 0, 0, 7, 1) == 0 && data_0034fb00.unknown_40 == 2);
    assert(data_0034fb00.unknown_30 == 7 && data_0034fb00.unknown_34 == 9);
    assert(link_run(0, 2, 0, 0, 7, 2) == 3 && link_is(3, 3));
    assert(link_run(0, 2, 1, 0, 7, 1) == 0 && data_0034fb00.unknown_30 == 7);
    assert(link_run(0, 2, 1, 0, 0, 1) == 16 && link_is(1, 0));
    assert(link_run(0, 2, 2, 0, 7, 1) == 2 && link_is(3, 0));
}

char data_0029b100[4], data_0029b1be[4];
static int open_calls, send_calls;
static int open_staged;
static int stage_calls;
int func_00194758(int a, int b, char *c, unsigned char *d)
{
    if (open_staged) {
        assert(a == data_0034fb00.unknown_08 && b == 6);
        stage_calls++;
    } else {
        assert(a == 21 && b == 22);
    }
    assert(c == data_0029b100 && d == data_0034fb00.unknown_2c0);
    open_calls++;
    return 55;
}
int func_00194578(int a, int b, char *c, unsigned d, int e, unsigned char *f)
{
    assert(a == 21 && b == 22 && c == data_0029b1be && d == 0x1234 && e == 1);
    assert(f == data_0034fb00.unknown_80);
    send_calls++;
    return 66;
}

static int request_mode = -1;
static int request_run(int stage, int w20, int w24, int count)
{
    memset(&data_0034fb00, 0, sizeof(data_0034fb00));
    data_0034fb00.unknown_08 = 21;
    data_0034fb00.unknown_0c = 22;
    data_0034fb00.unknown_60 = 0x1234;
    data_0034fb00.unknown_30 = 7;
    data_0034fb00.unknown_34 = stage;
    data_0034fb00.unknown_40 = count;
    status_20 = w20;
    status_24 = w24;
    open_calls = send_calls = 0;
    return request_mode < 0 ? TakuLoad_card_filename_check() : TakuSave_card_filename_check(request_mode);
}

static int request_is(int state, int stage, int count)
{
    return data_0034fb00.unknown_30 == state && data_0034fb00.unknown_34 == stage
        && data_0034fb00.unknown_40 == count;
}

/* TakuLoad_card_filename_check (branch coverage) with the three SDK-block callees stubbed. */
static void check_link_request(void)
{
    status_result = 1;
    /* a stage other than 0 or 1 calls nothing */
    assert(request_run(2, 12, 0, 1) == 0 && open_calls == 0 && send_calls == 0 && status_calls >= 0);
    assert(request_is(7, 2, 1));
    /* stage 0: results stored; status result or first word not matching returns 0 */
    status_result = 0;
    assert(request_run(0, 12, 0, 1) == 0 && open_calls == 1 && send_calls == 0);
    assert(data_0034fb00.unknown_00 == 55 && data_0034fb00.unknown_04 == 0 && request_is(7, 0, 1));
    status_result = 1;
    assert(request_run(0, 13, 0, 1) == 0 && data_0034fb00.unknown_04 == 1 && request_is(7, 0, 1));
    /* stage 0, second word 0: on to stage 1 */
    assert(request_run(0, 12, 0, 2) == 0 && request_is(7, 1, 0));
    /* -2: retry count, 3 at the third */
    assert(request_run(0, 12, -2, 0) == 0 && request_is(7, 0, 1));
    assert(request_run(0, 12, -2, 1) == 0 && request_is(7, 0, 2));
    assert(request_run(0, 12, -2, 2) == 3 && request_is(3, 0, 3));
    /* -4 and any other value */
    assert(request_run(0, 12, -4, 2) == 7 && request_is(3, 0, 0));
    assert(request_run(0, 12, 5, 2) == 2 && request_is(3, 0, 0));
    assert(request_run(0, 12, -1, 2) == 2 && request_is(3, 0, 0));
    /* stage 1 */
    status_result = 0;
    assert(request_run(1, 13, 0, 1) == 0 && open_calls == 0 && send_calls == 1);
    assert(data_0034fb00.unknown_00 == 66 && data_0034fb00.unknown_04 == 0 && request_is(7, 1, 1));
    status_result = 1;
    assert(request_run(1, 12, 0, 1) == 0 && request_is(7, 1, 1));
    /* non-negative second word: state 2, stage 0, count kept */
    assert(request_run(1, 13, 0, 1) == 9 && request_is(2, 0, 1));
    assert(request_run(1, 13, 40, 1) == 9 && request_is(2, 0, 1));
    assert(request_run(1, 13, -2, 1) == 0 && request_is(7, 1, 2));
    assert(request_run(1, 13, -2, 2) == 3 && request_is(3, 0, 3));
    assert(request_run(1, 13, -4, 2) == 10 && request_is(3, 0, 0));
    assert(request_run(1, 13, -1, 2) == 2 && request_is(3, 0, 0));
}

struct DotrMiscLargeEntry *p_kaban_data, *data_0034faec;
union DotrLinkHeadStore Taku_card_info;
static int entry_calls, clear_calls;
struct DotrMiscLargeEntry *SzData_GetData(int value)
{
    assert(value == entry_calls);
    entry_calls++;
    return (struct DotrMiscLargeEntry *)(reset_status + value);
}
void func_00195108(void *a, int b)
{
    assert(a == &data_0034fb00 && b == 0x840 && sizeof(data_0034fb00) == 0x840);
    assert(Taku_card_info.first_byte == 1);
    clear_calls++;
}

static int reset_run(int mode, int first, int second)
{
    memset(&data_0034fb00, 0x55, sizeof(data_0034fb00));
    p_kaban_data = data_0034faec = NULL;
    Taku_card_info.first_byte = 0;
    entry_calls = clear_calls = poll_calls = status_calls = 0;
    reset_status[0] = first;
    reset_status[1] = second;
    reset_mode = 1;
    mode = TakuCard_inti(mode);
    reset_mode = 0;
    assert(entry_calls == 2 && clear_calls == 1 && poll_calls == 2 && status_calls == 2);
    return mode;
}

/* TakuCard_inti (loop and branch coverage) with all four callees stubbed; the
 * 0x195108 stub leaves the 0x55 fill so that each cleared member is seen. */
static void check_link_reset(void)
{
    struct DotrMiscLink *l = &data_0034fb00;
    assert(reset_run(0, 0, 0) == 0);
    assert(p_kaban_data == (void *)reset_status && data_0034faec == (void *)(reset_status + 1));
    assert(Taku_card_info.first_byte == 1);
    assert(l->unknown_08 == 0 && l->unknown_0c == 0 && l->unknown_10 == 0 && l->unknown_14 == 0);
    assert(l->unknown_18 == 0 && l->unknown_1c == 0 && l->unknown_20 == 0 && l->unknown_24 == 0);
    assert(l->unknown_2c == 0 && l->unknown_2e == 0 && l->unknown_30 == 0 && l->unknown_34 == 0);
    assert(l->unknown_36 == 0 && l->unknown_38 == 0 && l->unknown_3a == 0 && l->unknown_3c == 0);
    assert(l->unknown_3e == 0 && l->unknown_40 == 0 && l->unknown_42 == 0 && l->unknown_48 == 0);
    assert(l->unknown_4c == 0 && l->unknown_50 == 0 && l->unknown_52 == 0 && l->unknown_54 == 0);
    assert(l->unknown_56 == 0 && l->unknown_5c == 0 && l->unknown_60 == 0 && l->unknown_64 == 0);
    assert(l->unknown_68 == 6 && l->unknown_280 == 0 && l->unknown_284 == 0 && l->unknown_288 == 0);
    assert(l->unknown_28c == 0 && l->unknown_290 == 0 && l->unknown_294 == 0 && l->unknown_296 == 0);
    assert(l->unknown_2a8 == 0 && l->unknown_2aa == 0 && l->unknown_2ac == 0 && l->unknown_2ae == 0);
    assert(l->unknown_2b0 == 0 && l->unknown_2b2 == 0 && l->unknown_2b4 == 0 && l->unknown_2b6 == 0);
    assert(l->unknown_2b8 == 0);
    /* members the routine does not store keep the fill */
    assert(l->unknown_00 == 0x55555555 && l->unknown_32 == 0x5555 && l->unknown_2be == 0x5555);
    /* mode 1 reports a non-zero status from either pass; other modes do not */
    assert(reset_run(1, 0, 0) == 0);
    assert(reset_run(1, 4, 0) == 1);
    assert(reset_run(1, 0, -3) == 1);
    assert(reset_run(0, 4, 4) == 0);
    assert(reset_run(2, 4, 4) == 0);
}

/* TakuSave_card_filename_check (branch coverage), same stubs as 0x23f340. */
static void check_link_request_mode(void)
{
    request_mode = 0;
    status_result = 1;
    assert(request_run(2, 12, 0, 1) == 0 && open_calls == 0 && send_calls == 0 && request_is(7, 2, 1));
    status_result = 0;
    assert(request_run(0, 12, 0, 1) == 0 && open_calls == 1 && send_calls == 0);
    assert(data_0034fb00.unknown_00 == 55 && data_0034fb00.unknown_04 == 0 && request_is(7, 0, 1));
    status_result = 1;
    assert(request_run(0, 13, 0, 1) == 0 && data_0034fb00.unknown_04 == 1 && request_is(7, 0, 1));
    assert(request_run(0, 12, 0, 2) == 0 && request_is(7, 1, 0));
    assert(request_run(0, 12, -2, 0) == 0 && request_is(7, 0, 1));
    assert(request_run(0, 12, -2, 1) == 0 && request_is(7, 0, 2));
    assert(request_run(0, 12, -2, 2) == 3 && request_is(11, 0, 3));
    assert(request_run(0, 12, -4, 2) == 17 && request_is(3, 0, 0));
    assert(request_run(0, 12, 5, 2) == 2 && request_is(14, 0, 0));
    assert(request_run(0, 12, -1, 2) == 2 && request_is(14, 0, 0));
    /* stage 1 */
    status_result = 0;
    assert(request_run(1, 13, 0, 1) == 0 && open_calls == 0 && send_calls == 1);
    assert(data_0034fb00.unknown_00 == 66 && data_0034fb00.unknown_04 == 0 && request_is(7, 1, 1));
    status_result = 1;
    assert(request_run(1, 12, 0, 1) == 0 && request_is(7, 1, 1));
    assert(request_run(1, 13, -2, 1) == 0 && request_is(7, 1, 2));
    assert(request_run(1, 13, -2, 2) == 3 && request_is(11, 0, 3));
    assert(request_run(1, 13, -4, 2) == 10 && request_is(14, 0, 0));
    assert(request_run(1, 13, -1, 2) == 2 && request_is(14, 0, 0));
    /* non-negative second word: the argument chooses the exit */
    assert(request_run(1, 13, 0, 2) == 16 && request_is(2, 0, 0));
    assert(request_run(1, 13, 40, 2) == 16 && request_is(2, 0, 0));
    request_mode = 1;
    assert(request_run(1, 13, 0, 2) == 0x100 && request_is(0, 0, 0));
    request_mode = 2;
    assert(request_run(1, 13, 0, 2) == 16 && request_is(2, 0, 0));
    request_mode = -1;
}

/* TakuSave_card_kansi (branch coverage), same stubs. The `3` result after the count
 * reaches 3 needs the state to differ from the 11 just stored: not reachable
 * on the host. */
static void check_link_poll_mapped(void)
{
    link_step = TakuSave_card_kansi;
    /* status result not 1, or status word not 1: only the final mapping */
    status_result = 0;
    status_20 = 1;
    poll_calls = status_calls = 0;
    assert(link_run(0, 0, 0, 0, 13, 1) == 1 && poll_calls == 1 && status_calls == 1);
    assert(data_0034fb00.unknown_00 == 77 && data_0034fb00.unknown_04 == 0);
    assert(data_0034fb00.unknown_30 == 13 && data_0034fb00.unknown_34 == 9);
    assert(link_run(0, 0, 0, 0, 11, 1) == 14);
    assert(link_run(0, 0, 0, 0, 3, 1) == 17);
    assert(link_run(0, 0, 0, 0, 7, 1) == 0);
    status_result = 1;
    status_20 = 0;
    assert(link_run(0, 0, 0, 0, 3, 1) == 17 && data_0034fb00.unknown_04 == 1);
    status_20 = 1;
    /* second status word */
    assert(link_run(0, 2, 5, -1, 7, 1) == 0x700 && link_is(14, 0));
    assert(link_run(0, 2, 5, -2, 7, 1) == 0x700 && link_is(14, 0));
    assert(link_run(0, 4, 0, 6, 7, 1) == 2 && link_is(14, 0));
    link_mode = 1;
    assert(link_run(0, 2, 0, 6, 3, 0) == 17 && data_0034fb00.unknown_34 == 9);
    assert(data_0034fb00.unknown_30 == 3 && data_0034fb00.unknown_40 == 1);
    /* first polled word 0: argument 1 with state 14 gives 0 */
    assert(link_run(0, 0, 0, 0, 14, 1) == 0 && link_is(14, 0));
    assert(link_run(0, 0, 0, 0, 7, 1) == 2 && link_is(14, 0));
    link_mode = 5;
    assert(link_run(0, 0, 0, 0, 14, 1) == 2 && link_is(14, 0));
    assert(link_run(1, 1, 0, 0, 7, 1) == 4 && link_is(14, 0));
    assert(link_run(1, 3, 0, 0, 7, 1) == 4 && link_is(14, 0));
    assert(link_run(0, 4, 0, 0, 7, 1) == 2 && link_is(14, 0));
    /* first word 2, third word 0: the count reaches 3 */
    assert(link_run(0, 2, 0, 0, 13, 1) == 1 && data_0034fb00.unknown_40 == 2);
    assert(data_0034fb00.unknown_30 == 13 && data_0034fb00.unknown_34 == 9);
    assert(link_run(0, 2, 0, 0, 7, 2) == 14 && link_is(11, 3));
    /* third word 1: the count is cleared; state 0 becomes 1 */
    assert(link_run(0, 2, 1, 0, 0, 2) == 0 && data_0034fb00.unknown_30 == 1);
    assert(data_0034fb00.unknown_34 == 9 && data_0034fb00.unknown_40 == 0);
    assert(link_run(0, 2, 1, 0, 13, 2) == 1 && data_0034fb00.unknown_40 == 0);
    /* state 3: second polled word against 250, then the argument */
    poll_14 = 249;
    assert(link_run(0, 2, 1, 0, 3, 2) == 5 && link_is(14, 0));
    poll_14 = 250;
    assert(link_run(0, 2, 1, 0, 3, 2) == 9 && data_0034fb00.unknown_30 == 4);
    assert(data_0034fb00.unknown_34 == 9 && data_0034fb00.unknown_40 == 0);
    link_mode = 1;
    assert(link_run(0, 2, 1, 0, 3, 2) == 0x100 && link_is(0, 0));
    link_mode = 5;
    poll_14 = 0;
    /* other third words */
    assert(link_run(0, 2, 2, 0, 7, 1) == 2 && link_is(14, 0));
    link_step = TakuCustom_card_kansi;
}

struct DotrLinkImage dotr_link_images_01e10000[2];
struct DotrLinkTail data_00299e10_link;
static struct DotrMiscLargeEntry stage_body;
static int stage_check, stage_done, stage_idle;

static int stage_d_want = 1;

int func_00193ba8(int a, int b, char *c)
{
    assert(a == data_0034fb00.unknown_08 && b == 6 && c == data_0029b1be);
    stage_calls++;
    return 61;
}

int func_00193dd0(int a, int b, int c, int d)
{
    assert(a == 31 + data_0034fb00.unknown_08 && b == 41 && c == 42 && d == stage_d_want);
    stage_calls++;
    return 62;
}

void func_0017bb60(int unknown)
{
    assert(unknown == 0);
    stage_idle++;
}

int func_00193f38(int a, struct DotrLinkImage *b, int c)
{
    assert(a == 31 + data_0034fb00.unknown_08 && c == 0x100f0 && stage_idle == 1);
    assert(b == &dotr_link_images_01e10000[data_0034fb00.unknown_08]);
    stage_calls++;
    return 63;
}

int func_00193d18(int a)
{
    assert(a == 31 + data_0034fb00.unknown_08);
    stage_calls++;
    return 64;
}

int LkCheckData(unsigned short *words)
{
    assert(words == (unsigned short *)&dotr_link_images_01e10000[data_0034fb00.unknown_08]);
    return stage_check;
}

static int stage_done_want = 6;

void TakuLoad_Name_Set(int a)
{
    assert(a == stage_done_want);
    stage_done++;
}

static int (*stage_step)(void) = TakuLoad_card_file_open;

static int stage_run(int stage, int index, int w20, int w24, int count)
{
    memset(&data_0034fb00, 0, sizeof(data_0034fb00));
    data_0034fb00.unknown_08 = index;
    data_0034fb00.unknown_0c = 6;
    data_0034fb00.unknown_30 = 7;
    data_0034fb00.unknown_34 = stage;
    data_0034fb00.unknown_40 = count;
    data_0034fb00.unknown_48 = 31;
    data_0034fb00.unknown_4c = 32;
    data_0034fb00.unknown_2b6 = 41;
    data_0034fb00.unknown_2b8 = 42;
    status_20 = w20;
    status_24 = w24;
    stage_calls = stage_idle = 0;
    return stage_step();
}

static int stage_is(int state, int stage, int count)
{
    return data_0034fb00.unknown_30 == state && data_0034fb00.unknown_34 == stage
        && data_0034fb00.unknown_40 == count;
}

/* TakuLoad_card_file_open (branch coverage) with its SDK-block callees, 0x17bb60, 0x241190
 * and 0x23fbe0 stubbed. */
static void check_link_transfer(void)
{
    int stage;
    static const int reply[4] = { 2, 4, 5, 3 };
    static const int result[4] = { 61, 62, 63, 64 };
    data_0034faec = NULL;
    p_kaban_data = &stage_body;
    /* an unlisted stage calls nothing */
    status_result = 1;
    assert(stage_run(4, 0, 2, 0, 1) == 0 && stage_calls == 0 && stage_is(7, 4, 1));
    for (stage = 0; stage < 4; stage++) {
        /* both results are stored; status result or reply word mismatch gives 0 */
        status_result = 0;
        assert(stage_run(stage, stage & 1, reply[stage], -4, 1) == 0 && stage_calls == 1);
        assert(data_0034fb00.unknown_00 == result[stage] && data_0034fb00.unknown_04 == 0);
        assert(stage_is(7, stage, 1) && stage_idle == (stage == 2));
        status_result = 1;
        assert(stage_run(stage, 0, 9, -4, 1) == 0 && stage_is(7, stage, 1));
        assert(data_0034fb00.unknown_04 == 1);
        /* -2 counts to 3; -4 gives 10; unlisted negative words give 2 */
        assert(stage_run(stage, 1, reply[stage], -2, 1) == 0 && stage_is(7, stage, 2));
        assert(stage_run(stage, 1, reply[stage], -2, 2) == 3 && stage_is(3, 0, 3));
        assert(stage_run(stage, 0, reply[stage], -4, 1) == 10 && stage_is(3, 0, 0));
        assert(stage_run(stage, 0, reply[stage], -6, 1) == 2 && stage_is(3, 0, 0));
    }
    /* stage 0: the other negative words; a word >= 0 is stored by index */
    assert(stage_run(0, 0, 2, -3, 1) == 5 && stage_is(3, 0, 0));
    assert(stage_run(0, 0, 2, -5, 1) == 10 && stage_is(3, 0, 0));
    assert(stage_run(0, 0, 2, -7, 1) == 10 && stage_is(3, 0, 0));
    assert(stage_run(0, 0, 2, 0, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 0 && data_0034fb00.unknown_4c == 32);
    assert(stage_run(0, 1, 2, 9, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 31 && data_0034fb00.unknown_4c == 9);
    /* stage 1: only 0 advances; positive words give 2 */
    assert(stage_run(1, 0, 4, 0, 1) == 0 && stage_is(7, 2, 0));
    assert(stage_run(1, 0, 4, 5, 1) == 2 && stage_is(3, 0, 0));
    /* stage 2: any word >= 0 advances */
    assert(stage_run(2, 1, 5, 0, 1) == 0 && stage_is(7, 3, 0));
    assert(stage_run(2, 1, 5, 8, 1) == 0 && stage_is(7, 3, 0));
    /* stage 3, word 0: the three copies, then the check decides */
    assert(stage_run(3, 0, 3, 5, 1) == 2 && stage_is(3, 0, 0));
    dotr_link_images_01e10000[1].head.unknown_00 = 0x1234;
    dotr_link_images_01e10000[1].head.unknown_02 = -2;
    dotr_link_images_01e10000[1].body.unknown_words[0] = 11;
    dotr_link_images_01e10000[1].body.unknown_words[16359] = 12;
    dotr_link_images_01e10000[1].tail.unknown_words[0] = 13;
    dotr_link_images_01e10000[1].tail.unknown_words[82] = 14;
    stage_check = 1;
    stage_done = 0;
    assert(stage_run(3, 1, 3, 0, 1) == 0x100 && stage_is(0, 0, 0) && stage_done == 1);
    assert(memcmp(&Taku_card_info, &dotr_link_images_01e10000[1].head, 4) == 0);
    assert(Taku_card_info.head.unknown_00 == 0x1234);
    assert(memcmp(&stage_body, &dotr_link_images_01e10000[1].body, sizeof(stage_body)) == 0);
    assert(stage_body.unknown_words[16359] == 12);
    assert(memcmp(&data_00299e10_link, &dotr_link_images_01e10000[1].tail, 332) == 0);
    assert(data_00299e10_link.unknown_words[82] == 14);
    stage_check = 0;
    assert(stage_run(3, 0, 3, 0, 1) == 10 && stage_is(3, 0, 0) && stage_done == 1);
    assert(stage_body.unknown_words[16359] == 0 && Taku_card_info.first_byte == 0);
}

/* TakuSave_card_file_open (branch coverage), same stubs as 0x23f5a0. */
static void check_link_stages(void)
{
    int stage;
    static const int reply[4] = { 2, 4, 5, 3 };
    static const int result[4] = { 61, 62, 63, 64 };
    stage_step = TakuSave_card_file_open;
    /* an unlisted stage calls nothing */
    status_result = 1;
    assert(stage_run(4, 0, 2, 0, 1) == 0 && stage_calls == 0 && stage_is(7, 4, 1));
    for (stage = 0; stage < 4; stage++) {
        /* both results are stored; status result or reply word mismatch gives 0 */
        status_result = 0;
        assert(stage_run(stage, stage & 1, reply[stage], -4, 1) == 0 && stage_calls == 1);
        assert(data_0034fb00.unknown_00 == result[stage] && data_0034fb00.unknown_04 == 0);
        assert(stage_is(7, stage, 1) && stage_idle == (stage == 2));
        status_result = 1;
        assert(stage_run(stage, 0, 9, -4, 1) == 0 && stage_is(7, stage, 1));
        assert(data_0034fb00.unknown_04 == 1);
        /* -2 counts to 3; -4 gives 10; unlisted negative words give 2 */
        assert(stage_run(stage, 1, reply[stage], -2, 1) == 0 && stage_is(7, stage, 2));
        assert(stage_run(stage, 1, reply[stage], -2, 2) == 3 && stage_is(11, 0, 3));
        assert(stage_run(stage, 0, reply[stage], -4, 1) == 10 && stage_is(14, 0, 0));
        assert(stage_run(stage, 0, reply[stage], -6, 1) == 2 && stage_is(14, 0, 0));
    }
    /* stage 0: the other listed negative words */
    assert(stage_run(0, 0, 2, -3, 1) == 5 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 2, -5, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 2, -7, 1) == 10 && stage_is(14, 0, 0));
    /* stage 0: a non-negative word is stored at +0x48 / +0x4c by the index */
    assert(stage_run(0, 0, 2, 0, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 0 && data_0034fb00.unknown_4c == 32);
    assert(stage_run(0, 1, 2, 55, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 31 && data_0034fb00.unknown_4c == 55);
    /* stage 1: only word 0 advances; a positive word is unlisted */
    assert(stage_run(1, 0, 4, 0, 1) == 0 && stage_is(7, 2, 0));
    assert(stage_run(1, 0, 4, 5, 1) == 2 && stage_is(14, 0, 0));
    /* stage 2: any non-negative word advances */
    assert(stage_run(2, 0, 5, 0, 1) == 0 && stage_is(7, 3, 0));
    assert(stage_run(2, 1, 5, 9, 1) == 0 && stage_is(7, 3, 0));
    /* stage 3: word 0 finishes with state 13 and result 1 */
    assert(stage_run(3, 0, 3, 0, 1) == 1 && stage_is(13, 0, 0));
    assert(stage_run(3, 0, 3, 5, 1) == 2 && stage_is(14, 0, 0));
    stage_step = TakuLoad_card_file_open;
}

int func_00193ce0(int a, int b, char *c)
{
    assert(a == data_0034fb00.unknown_08 && b == 6 && c == data_0029b100);
    stage_calls++;
    return 71;
}

static int stage_wait_result;
int card_icon_tukuru(void)
{
    stage_calls++;
    return stage_wait_result;
}

int func_00193ba8_mode(int a, int b, char *c, int d)
{
    assert(a == data_0034fb00.unknown_08 && b == 6 && c == data_0029b1be && d == 512);
    stage_calls++;
    return 73;
}

/* TakuSave_make_file (branch coverage) with 0x193ce0, 0x194758, 0x23e7a0, 0x193ba8 and
 * 0x193d18 stubbed. */
static void check_link_open_stages(void)
{
    int i;
    static const int stages[4] = { 0, 1, 3, 4 };
    static const int reply[4] = { 11, 12, 2, 3 };
    static const int result[4] = { 71, 55, 73, 64 };
    stage_step = TakuSave_make_file;
    open_staged = 1;
    /* an unlisted stage calls nothing */
    status_result = 1;
    assert(stage_run(5, 0, 2, 0, 1) == 0 && stage_calls == 0 && stage_is(7, 5, 1));
    for (i = 0; i < 4; i++) {
        int stage = stages[i];
        /* both results are stored; status result or reply word mismatch gives 0 */
        status_result = 0;
        assert(stage_run(stage, i & 1, reply[i], -4, 1) == 0 && stage_calls == 1);
        assert(data_0034fb00.unknown_00 == result[i] && data_0034fb00.unknown_04 == 0);
        assert(stage_is(7, stage, 1));
        status_result = 1;
        assert(stage_run(stage, 0, 9, -4, 1) == 0 && stage_is(7, stage, 1));
        assert(data_0034fb00.unknown_04 == 1);
        /* -2 counts to 3; -4 gives 10; unlisted negative words give 2 */
        assert(stage_run(stage, 1, reply[i], -2, 1) == 0 && stage_is(7, stage, 2));
        assert(stage_run(stage, 1, reply[i], -2, 2) == 3 && stage_is(11, 0, 3));
        assert(stage_run(stage, 0, reply[i], -4, 1) == 10 && stage_is(14, 0, 0));
        assert(stage_run(stage, 0, reply[i], -6, 1) == 2 && stage_is(14, 0, 0));
    }
    /* stage 0: -3 gives 5; word 0 advances; a positive word is unlisted */
    assert(stage_run(0, 0, 11, -3, 1) == 5 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 11, 0, 1) == 0 && stage_is(7, 1, 0));
    assert(stage_run(0, 0, 11, 5, 1) == 2 && stage_is(14, 0, 0));
    /* stage 1: only word 0 advances */
    assert(stage_run(1, 0, 12, 0, 1) == 0 && stage_is(7, 2, 0));
    assert(stage_run(1, 0, 12, 5, 1) == 2 && stage_is(14, 0, 0));
    /* stage 2: 1 advances, 10 is returned, anything else waits */
    stage_wait_result = 1;
    assert(stage_run(2, 0, 0, 0, 1) == 0 && stage_calls == 1 && stage_is(7, 3, 0));
    stage_wait_result = 10;
    assert(stage_run(2, 0, 0, 0, 1) == 10 && stage_calls == 1 && stage_is(7, 2, 1));
    stage_wait_result = 0;
    assert(stage_run(2, 0, 0, 0, 1) == 0 && stage_calls == 1 && stage_is(7, 2, 1));
    /* stage 3: the other listed negative words; a non-negative word is
     * stored at +0x48 / +0x4c by the index */
    assert(stage_run(3, 0, 2, -3, 1) == 5 && stage_is(14, 0, 0));
    assert(stage_run(3, 0, 2, -5, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(3, 0, 2, -7, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(3, 0, 2, 0, 1) == 0 && stage_is(7, 4, 0));
    assert(data_0034fb00.unknown_48 == 0 && data_0034fb00.unknown_4c == 32);
    assert(stage_run(3, 1, 2, 55, 1) == 0 && stage_is(7, 4, 0));
    assert(data_0034fb00.unknown_48 == 31 && data_0034fb00.unknown_4c == 55);
    /* stage 4: word 0 finishes with state 5 and result 9 */
    assert(stage_run(4, 0, 3, 0, 1) == 9 && stage_is(5, 0, 0));
    assert(stage_run(4, 0, 3, 5, 1) == 2 && stage_is(14, 0, 0));
    open_staged = 0;
    stage_step = TakuLoad_card_file_open;
}

/* TakuCustom_card_file_open (branch coverage), same stubs as 0x23f5a0; 0x23fbe0 is expected
 * to receive the word at +8. */
static void check_link_transfer_pair(void)
{
    int stage;
    static struct DotrMiscLargeEntry body_b;
    static const int reply[4] = { 2, 4, 5, 3 };
    static const int result[4] = { 61, 62, 63, 64 };
    stage_step = TakuCustom_card_file_open;
    p_kaban_data = &stage_body;
    data_0034faec = &body_b;
    /* an unlisted stage calls nothing */
    status_result = 1;
    assert(stage_run(4, 0, 2, 0, 1) == 0 && stage_calls == 0 && stage_is(7, 4, 1));
    for (stage = 0; stage < 4; stage++) {
        /* both results are stored; status result or reply word mismatch gives 0 */
        status_result = 0;
        assert(stage_run(stage, stage & 1, reply[stage], -4, 1) == 0 && stage_calls == 1);
        assert(data_0034fb00.unknown_00 == result[stage] && data_0034fb00.unknown_04 == 0);
        assert(stage_is(7, stage, 1) && stage_idle == (stage == 2));
        status_result = 1;
        assert(stage_run(stage, 0, 9, -4, 1) == 0 && stage_is(7, stage, 1));
        assert(data_0034fb00.unknown_04 == 1);
        /* -2 counts to 3; -4 gives 10; unlisted negative words give 2 */
        assert(stage_run(stage, 1, reply[stage], -2, 1) == 0 && stage_is(7, stage, 2));
        assert(stage_run(stage, 1, reply[stage], -2, 2) == 3 && stage_is(3, 0, 3));
        assert(stage_run(stage, 0, reply[stage], -4, 1) == 10 && stage_is(3, 0, 0));
        assert(stage_run(stage, 0, reply[stage], -6, 1) == 2 && stage_is(3, 0, 0));
    }
    /* stage 0: the other negative words; a word >= 0 is stored by index */
    assert(stage_run(0, 0, 2, -3, 1) == 5 && stage_is(3, 0, 0));
    assert(stage_run(0, 0, 2, -5, 1) == 10 && stage_is(3, 0, 0));
    assert(stage_run(0, 0, 2, -7, 1) == 10 && stage_is(3, 0, 0));
    assert(stage_run(0, 0, 2, 0, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 0 && data_0034fb00.unknown_4c == 32);
    assert(stage_run(0, 1, 2, 9, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 31 && data_0034fb00.unknown_4c == 9);
    /* stage 1: only 0 advances; positive words give 2 */
    assert(stage_run(1, 0, 4, 0, 1) == 0 && stage_is(7, 2, 0));
    assert(stage_run(1, 0, 4, 5, 1) == 2 && stage_is(3, 0, 0));
    /* stage 2: any word >= 0 advances */
    assert(stage_run(2, 1, 5, 0, 1) == 0 && stage_is(7, 3, 0));
    assert(stage_run(2, 1, 5, 8, 1) == 0 && stage_is(7, 3, 0));
    /* stage 3: a positive word is unlisted */
    assert(stage_run(3, 0, 3, 5, 1) == 2 && stage_is(3, 0, 0));
    /* stage 3, word 0, index 0: copies through 0x34fae8, result 0x10 */
    memset(dotr_link_images_01e10000, 0, 2 * sizeof(dotr_link_images_01e10000[0]));
    memset(&stage_body, 0, sizeof(stage_body));
    dotr_link_images_01e10000[0].head.unknown_00 = 0x2345;
    dotr_link_images_01e10000[0].body.unknown_words[16359] = 21;
    dotr_link_images_01e10000[0].tail.unknown_words[82] = 22;
    dotr_link_images_01e10000[1].head.unknown_00 = 0x1234;
    dotr_link_images_01e10000[1].body.unknown_words[16359] = 12;
    dotr_link_images_01e10000[1].tail.unknown_words[82] = 14;
    stage_check = 1;
    stage_done = 0;
    stage_done_want = 0;
    assert(stage_run(3, 0, 3, 0, 1) == 0x10 && stage_is(0, 0, 0) && stage_done == 1);
    assert(data_0034fb00.unknown_08 == 1);
    assert(data_0034fb00.unknown_44 == 1 && data_0034fb00.unknown_46 == 0);
    assert(Taku_card_info.head.unknown_00 == 0x2345);
    assert(stage_body.unknown_words[16359] == 21 && body_b.unknown_words[16359] == 0);
    assert(data_00299e10_link.unknown_words[82] == 22);
    /* index 1: copies through 0x34faec, result 0x100 */
    stage_done_want = 1;
    assert(stage_run(3, 1, 3, 0, 1) == 0x100 && stage_is(0, 0, 0) && stage_done == 2);
    assert(data_0034fb00.unknown_08 == 1);
    assert(data_0034fb00.unknown_44 == 0 && data_0034fb00.unknown_46 == 1);
    assert(Taku_card_info.head.unknown_00 == 0x1234);
    assert(stage_body.unknown_words[16359] == 21 && body_b.unknown_words[16359] == 12);
    assert(data_00299e10_link.unknown_words[82] == 14);
    /* the check failing gives 10 on either side, after the copies */
    stage_check = 0;
    assert(stage_run(3, 0, 3, 0, 1) == 10 && stage_is(3, 0, 0) && stage_done == 2);
    assert(data_0034fb00.unknown_08 == 0 && data_0034fb00.unknown_44 == 0);
    assert(stage_run(3, 1, 3, 0, 1) == 10 && stage_is(3, 0, 0) && stage_done == 2);
    assert(data_0034fb00.unknown_08 == 1 && data_0034fb00.unknown_46 == 0);
    memset(dotr_link_images_01e10000, 0, 2 * sizeof(dotr_link_images_01e10000[0]));
    stage_done_want = 6;
    stage_step = TakuLoad_card_file_open;
    data_0034faec = NULL;
}

static int link_poll_flip(int mode)
{
    (void)mode;
    return TakuTrade_Save_card_kansi();
}

/* TakuTrade_Save_card_kansi (branch coverage), same stubs. As for 0x23ce40 the `3` result
 * after the count reaches 3 is not reachable on the host. */
static void check_link_poll_flip(void)
{
    link_step = link_poll_flip;
    /* status result not 1, or status word not 1: only the final mapping */
    status_result = 0;
    status_20 = 1;
    poll_calls = status_calls = 0;
    assert(link_run(0, 0, 0, 0, 13, 1) == 1 && poll_calls == 1 && status_calls == 1);
    assert(data_0034fb00.unknown_00 == 77 && data_0034fb00.unknown_04 == 0);
    assert(data_0034fb00.unknown_30 == 13 && data_0034fb00.unknown_34 == 9);
    assert(link_run(0, 0, 0, 0, 11, 1) == 14);
    assert(link_run(0, 0, 0, 0, 3, 1) == 17);
    assert(link_run(0, 0, 0, 0, 0, 1) == 9);
    assert(link_run(0, 0, 0, 0, 7, 1) == 0);
    status_result = 1;
    status_20 = 0;
    assert(link_run(0, 0, 0, 0, 3, 1) == 17 && data_0034fb00.unknown_04 == 1);
    status_20 = 1;
    /* second status word: -1 and -2 leave, any other non-zero sets state 14 */
    assert(link_run(0, 2, 5, -1, 7, 1) == 0x700 && link_is(14, 0));
    assert(link_run(0, 2, 5, -2, 7, 1) == 0x700 && link_is(14, 0));
    assert(link_run(0, 2, 0, 6, 3, 0) == 0 && link_is(14, 1));
    /* first polled word */
    assert(link_run(0, 0, 0, 0, 7, 1) == 2 && link_is(14, 0));
    assert(link_run(1, 1, 0, 0, 7, 1) == 4 && link_is(14, 0));
    assert(link_run(1, 3, 0, 0, 7, 1) == 4 && link_is(14, 0));
    assert(link_run(0, 4, 0, 0, 7, 1) == 2 && link_is(14, 0));
    /* first word 2, third word 0: the count reaches 3 */
    assert(link_run(0, 2, 0, 0, 13, 1) == 1 && data_0034fb00.unknown_40 == 2);
    assert(data_0034fb00.unknown_30 == 13 && data_0034fb00.unknown_34 == 9);
    assert(link_run(0, 2, 0, 0, 7, 2) == 14 && link_is(11, 3));
    /* third word 1: the count is cleared; state 0 flips the word at +8 */
    assert(link_run(0, 2, 1, 0, 0, 2) == 0 && data_0034fb00.unknown_08 == 1);
    assert(data_0034fb00.unknown_44 == 1 && data_0034fb00.unknown_46 == 0);
    assert(data_0034fb00.unknown_30 == 0 && data_0034fb00.unknown_40 == 0);
    data_0034fb00.unknown_44 = data_0034fb00.unknown_46 = 7;
    data_0034fb00.unknown_30 = 0;
    data_0034fb00.unknown_40 = 2;
    assert(TakuTrade_Save_card_kansi() == 9 && data_0034fb00.unknown_08 == 0);
    assert(data_0034fb00.unknown_44 == 0 && data_0034fb00.unknown_46 == 0);
    assert(data_0034fb00.unknown_30 == 5 && data_0034fb00.unknown_40 == 0);
    assert(link_run(0, 2, 1, 0, 13, 2) == 1 && data_0034fb00.unknown_40 == 0);
    /* state 3: second polled word against 250 */
    poll_14 = 249;
    assert(link_run(0, 2, 1, 0, 3, 2) == 5 && link_is(14, 0));
    poll_14 = 250;
    assert(link_run(0, 2, 1, 0, 3, 2) == 9 && data_0034fb00.unknown_30 == 4);
    assert(data_0034fb00.unknown_34 == 9 && data_0034fb00.unknown_40 == 0);
    poll_14 = 0;
    /* other third words */
    assert(link_run(0, 2, 2, 0, 7, 1) == 2 && link_is(14, 0));
    link_step = TakuCustom_card_kansi;
}

static int stage_body_only;

int func_00194058(int a, struct DotrLinkImage *b, int c)
{
    if (stage_body_only) {
        /* 0x2409e0: body from the pointer for this slot, nothing else */
        int slot = data_0034fb00.unknown_08;
        assert(a == 31 + slot && c == 0x100f0 && stage_idle == 1);
        assert(b == &dotr_link_images_01e10000[slot]);
        assert(b->head.unknown_00 == 0 && b->head.unknown_02 == 0);
        assert(b->body.unknown_words[0] == 21 + 4 * slot);
        assert(b->body.unknown_words[16359] == 22 + 4 * slot);
        assert(b->tail.unknown_words[0] == 0 && b->tail.unknown_words[82] == 0);
        stage_calls++;
        return 65;
    }
    assert(a == 31 + data_0034fb00.unknown_08 && c == 0x100f0 && stage_idle == 1);
    assert(b == &dotr_link_images_01e10000[data_0034fb00.unknown_08]);
    assert(b->head.unknown_00 == 0x3456 && b->head.unknown_02 == 0x789a);
    assert(b->body.unknown_words[0] == 21 && b->body.unknown_words[16359] == 22);
    assert(b->tail.unknown_words[0] == 23 && b->tail.unknown_words[82] == 24);
    stage_calls++;
    return 65;
}

int func_00194a98(int a)
{
    assert(a == 31 + data_0034fb00.unknown_08);
    stage_calls++;
    return 66;
}

/* TakuSave_card_file_kakikomi (branch coverage) with its SDK-block callees, 0x17bb60 and
 * 0x241190 stubbed. */
static void check_link_transfer_back(void)
{
    int stage;
    static const int reply[5] = { 2, 4, 6, 10, 3 };
    static const int result[5] = { 61, 62, 65, 66, 64 };
    stage_step = TakuSave_card_file_kakikomi;
    stage_d_want = 2;
    p_kaban_data = &stage_body;
    memset(&stage_body, 0, sizeof(stage_body));
    stage_body.unknown_words[0] = 21;
    stage_body.unknown_words[16359] = 22;
    Taku_card_info.head.unknown_00 = 0x3456;
    Taku_card_info.head.unknown_02 = 0x789a;
    data_00299e10_link.unknown_words[0] = 23;
    data_00299e10_link.unknown_words[82] = 24;
    status_result = 1;
    assert(stage_run(5, 0, 2, 0, 1) == 0 && stage_calls == 0 && stage_is(7, 5, 1));
    for (stage = 0; stage < 5; stage++) {
        /* no reply yet, then a reply of another kind */
        status_result = 0;
        assert(stage_run(stage, stage & 1, reply[stage], -4, 1) == 0 && stage_calls == 1);
        assert(data_0034fb00.unknown_00 == result[stage] && data_0034fb00.unknown_04 == 0);
        assert(stage_is(7, stage, 1) && stage_idle == (stage == 2));
        status_result = 1;
        assert(stage_run(stage, 0, 9, -4, 1) == 0 && stage_is(7, stage, 1));
        assert(data_0034fb00.unknown_04 == 1);
        /* -2 counts to three; -4 and an unlisted word */
        assert(stage_run(stage, 1, reply[stage], -2, 1) == 0 && stage_is(7, stage, 2));
        assert(stage_run(stage, 1, reply[stage], -2, 2) == 3 && stage_is(11, 0, 3));
        assert(stage_run(stage, 0, reply[stage], -4, 1) == 10 && stage_is(14, 0, 0));
        assert(stage_run(stage, 0, reply[stage], -6, 1) == 2 && stage_is(14, 0, 0));
    }
    /* stage 0: the other negative words, then the handle store */
    assert(stage_run(0, 0, 2, -3, 1) == 5 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 2, -5, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 2, -7, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 2, 0, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 0 && data_0034fb00.unknown_4c == 32);
    assert(stage_run(0, 1, 2, 9, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 31 && data_0034fb00.unknown_4c == 9);
    /* stage 1 */
    assert(stage_run(1, 0, 4, 0, 1) == 0 && stage_is(7, 2, 0));
    assert(stage_run(1, 0, 4, 5, 1) == 2 && stage_is(14, 0, 0));
    /* stage 2: the copies are checked in the 0x194058 stub; next stage is 4 */
    memset(dotr_link_images_01e10000, 0, sizeof(dotr_link_images_01e10000));
    assert(stage_run(2, 1, 6, 0, 1) == 0 && stage_is(7, 4, 0) && stage_idle == 1);
    assert(dotr_link_images_01e10000[0].head.unknown_00 == 0);
    assert(dotr_link_images_01e10000[1].tail.unknown_words[82] == 24);
    assert(stage_run(2, 0, 6, 8, 1) == 0 && stage_is(7, 4, 0));
    assert(stage_run(2, 0, 6, -3, 1) == 5 && stage_is(14, 0, 0));
    assert(stage_run(2, 0, 6, -5, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(2, 0, 6, -7, 1) == 2 && stage_is(14, 0, 0));
    /* stage 3 */
    assert(stage_run(3, 0, 10, 0, 1) == 0 && stage_is(7, 4, 0));
    assert(stage_run(3, 0, 10, 5, 1) == 2 && stage_is(14, 0, 0));
    /* stage 4 */
    assert(stage_run(4, 1, 3, 0, 1) == 0x100 && stage_is(0, 0, 0));
    assert(stage_run(4, 0, 3, 5, 1) == 2 && stage_is(14, 0, 0));
    memset(dotr_link_images_01e10000, 0, sizeof(dotr_link_images_01e10000));
    memset(&stage_body, 0, sizeof(stage_body));
    memset(&data_00299e10_link, 0, sizeof(data_00299e10_link));
    memset(&Taku_card_info, 0, sizeof(Taku_card_info));
    stage_d_want = 1;
    stage_step = TakuLoad_card_file_open;
}

/* TakuTrade_Save_card_file_kakikomi_Ver2 (branch coverage), same stubs as 0x23de90; the 0x194058 stub
 * checks that only the body for the current slot was copied. */
static void check_link_transfer_back_pair(void)
{
    int stage;
    static struct DotrMiscLargeEntry body_b;
    static const int reply[5] = { 2, 4, 6, 10, 3 };
    static const int result[5] = { 61, 62, 65, 66, 64 };
    stage_step = TakuTrade_Save_card_file_kakikomi_Ver2;
    stage_d_want = 2;
    stage_body_only = 1;
    p_kaban_data = &stage_body;
    data_0034faec = &body_b;
    memset(&stage_body, 0, sizeof(stage_body));
    stage_body.unknown_words[0] = 21;
    stage_body.unknown_words[16359] = 22;
    body_b.unknown_words[0] = 25;
    body_b.unknown_words[16359] = 26;
    Taku_card_info.head.unknown_00 = 0x3456;
    data_00299e10_link.unknown_words[82] = 24;
    memset(dotr_link_images_01e10000, 0, sizeof(dotr_link_images_01e10000));
    status_result = 1;
    assert(stage_run(5, 0, 2, 0, 1) == 0 && stage_calls == 0 && stage_is(7, 5, 1));
    for (stage = 0; stage < 5; stage++) {
        /* no reply yet, then a reply of another kind */
        status_result = 0;
        assert(stage_run(stage, stage & 1, reply[stage], -4, 1) == 0 && stage_calls == 1);
        assert(data_0034fb00.unknown_00 == result[stage] && data_0034fb00.unknown_04 == 0);
        assert(stage_is(7, stage, 1) && stage_idle == (stage == 2));
        assert(data_0034fb00.unknown_08 == (stage & 1));
        status_result = 1;
        assert(stage_run(stage, 0, 9, -4, 1) == 0 && stage_is(7, stage, 1));
        assert(data_0034fb00.unknown_04 == 1);
        /* -2 counts to three; -4 and an unlisted word */
        assert(stage_run(stage, 1, reply[stage], -2, 1) == 0 && stage_is(7, stage, 2));
        assert(stage_run(stage, 1, reply[stage], -2, 2) == 3 && stage_is(11, 0, 3));
        assert(stage_run(stage, 0, reply[stage], -4, 1) == 10 && stage_is(14, 0, 0));
        assert(stage_run(stage, 0, reply[stage], -6, 1) == 2 && stage_is(14, 0, 0));
        assert(data_0034fb00.unknown_08 == 0);
    }
    /* stage 0: the other negative words; the handle store for each slot */
    assert(stage_run(0, 0, 2, -3, 1) == 5 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 2, -5, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 2, -7, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(0, 0, 2, 0, 1) == 0 && stage_is(7, 0, 0));
    assert(data_0034fb00.unknown_48 == 0 && data_0034fb00.unknown_4c == 32);
    assert(data_0034fb00.unknown_08 == 1);
    assert(stage_run(0, 1, 2, 9, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_48 == 31 && data_0034fb00.unknown_4c == 9);
    assert(data_0034fb00.unknown_08 == 0);
    /* stage 1 */
    assert(stage_run(1, 0, 4, 0, 1) == 0 && stage_is(7, 1, 0));
    assert(data_0034fb00.unknown_08 == 1);
    assert(stage_run(1, 1, 4, 0, 1) == 0 && stage_is(7, 2, 0));
    assert(data_0034fb00.unknown_08 == 0);
    assert(stage_run(1, 0, 4, 5, 1) == 2 && stage_is(14, 0, 0));
    /* stage 2: the copy is checked in the 0x194058 stub */
    assert(stage_run(2, 0, 6, 0, 1) == 0 && stage_is(7, 2, 0) && stage_idle == 1);
    assert(data_0034fb00.unknown_08 == 1);
    assert(dotr_link_images_01e10000[0].body.unknown_words[16359] == 22);
    assert(stage_run(2, 1, 6, 8, 1) == 0 && stage_is(7, 4, 0));
    assert(data_0034fb00.unknown_08 == 0);
    assert(dotr_link_images_01e10000[1].body.unknown_words[16359] == 26);
    assert(stage_run(2, 0, 6, -3, 1) == 5 && stage_is(14, 0, 0));
    assert(stage_run(2, 0, 6, -5, 1) == 10 && stage_is(14, 0, 0));
    assert(stage_run(2, 0, 6, -7, 1) == 2 && stage_is(14, 0, 0));
    /* stage 3 */
    assert(stage_run(3, 0, 10, 0, 1) == 0 && stage_is(7, 4, 0));
    assert(data_0034fb00.unknown_08 == 0);
    assert(stage_run(3, 0, 10, 5, 1) == 2 && stage_is(14, 0, 0));
    /* stage 4: 0x100 only after the second slot */
    assert(stage_run(4, 0, 3, 0, 1) == 0 && stage_is(5, 4, 0));
    assert(data_0034fb00.unknown_08 == 1);
    assert(stage_run(4, 1, 3, 0, 1) == 0x100 && stage_is(5, 0, 0));
    assert(data_0034fb00.unknown_08 == 0);
    assert(stage_run(4, 0, 3, 5, 1) == 2 && stage_is(14, 0, 0));
    memset(dotr_link_images_01e10000, 0, sizeof(dotr_link_images_01e10000));
    memset(&stage_body, 0, sizeof(stage_body));
    memset(&data_00299e10_link, 0, sizeof(data_00299e10_link));
    memset(&Taku_card_info, 0, sizeof(Taku_card_info));
    data_0034faec = NULL;
    stage_body_only = 0;
    stage_d_want = 1;
    stage_step = TakuLoad_card_file_open;
}

int main(void)
{
    check_link_transfer_back_pair();
    check_link_transfer_back();
    check_link_poll_flip();
    check_link_open_stages();
    check_link_stages();
    check_link_transfer_pair();
    check_link_transfer();
    check_link_poll_mapped();
    check_link_request_mode();
    check_link_reset();
    check_link_request();
    check_link_poll();
    check_link_poll_plain();
    puts("link_step: ok");
    return 0;
}
