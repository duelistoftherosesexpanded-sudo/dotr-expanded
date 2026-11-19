/* Synthetic host tests for the bgm_request group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/bgm_request.h"

struct DotrBgmGame gSzGame_gl_bgm;
static int played, played_b, plays, stopped, stops;

int func_00217980_bgm(int n, int b)
{
    played = n;
    played_b = b;
    plays++;
    return 0;
}
int func_00217a10_bgm(int n)
{
    stopped = n;
    stops++;
    return 0;
}

/* The number played for request n, or -1 when nothing is played. */
static int play(int n)
{
    played = -1;
    plays = stops = 0;
    SzGame_ReqBGM(n, 77);
    assert(stops == 0 && plays <= 1 && (plays == 0 || played_b == 77));
    return played;
}

int main(void)
{
    static const int by_place[10] = {17, 18, 15, 14, 16, 20, 21, 13, 22, 19};
    int i;

    assert(offsetof(struct DotrBgmGame, stage) == 316 && offsetof(struct DotrBgmGame, flag) == 320);
    assert(offsetof(struct DotrBgmGame, place) == 332);

    plays = stops = 0;
    SzGame_ReqBGM(-1, 5);
    assert(stops == 1 && stopped == 256 && plays == 0);
    SzGame_ReqBGM(-1002, 5);
    assert(stops == 2 && plays == 0);

    assert(play(0) == 7 && play(3000) == 7);
    assert(play(6) == 5 && play(7) == 5 && play(8) == 41 && play(9) == 3 && play(10) == 2 && play(15) == 21);
    assert(play(2) == -1 && play(3) == -1 && play(4) == -1 && play(11) == -1 && play(14) == -1 && play(16) == -1);
    assert(play(999) == -1 && play(-3) == -1);

    /* The duel music by stage; the thousands of the request are added. */
    gSzGame_gl_bgm.flag = 0;
    gSzGame_gl_bgm.stage = 0;  assert(play(1) == 7);
    gSzGame_gl_bgm.stage = 1;  assert(play(1) == 10 && play(2001) == 2010);
    gSzGame_gl_bgm.stage = 19; assert(play(1) == 11);
    gSzGame_gl_bgm.stage = 20; assert(play(1) == 12);
    gSzGame_gl_bgm.stage = 21; assert(play(1) == 44);
    gSzGame_gl_bgm.stage = 2;  assert(play(1) == 8);
    gSzGame_gl_bgm.stage = 18; assert(play(1) == 8);
    gSzGame_gl_bgm.flag = 1;   assert(play(1) == 9 && play(5001) == 5009);
    gSzGame_gl_bgm.stage = 1;  assert(play(1) == 10);
    gSzGame_gl_bgm.stage = 22; assert(play(1) == 40);
    gSzGame_gl_bgm.stage = -1; assert(play(1) == 9);

    /* The campaign music by place. */
    for (i = 0; i < 10; i++) {
        gSzGame_gl_bgm.place = i;
        assert(play(5) == by_place[i] && play(4005) == by_place[i]);
    }
    gSzGame_gl_bgm.place = 10; assert(play(5) == -1);
    gSzGame_gl_bgm.place = -1; assert(play(5) == -1);
    puts("bgm_request: ok");
    return 0;
}
