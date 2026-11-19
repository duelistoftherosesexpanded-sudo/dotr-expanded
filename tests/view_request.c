/* Synthetic host tests for the view_request group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/view_request.h"

DotrViewReqWord gSzView_hView_req[2];
struct DotrViewReqSlot gSzView_CameraReq_req[8];

static struct DotrViewReqCamera camera;

static void reset(void)
{
    struct DotrViewReqVec polar = {1.0f, 0.0f, 3.0f, 4.0f};
    struct DotrViewReqVec direction = {5.0f, 6.0f, 7.0f, 8.0f};
    struct DotrViewReqVec position = {9.0f, 10.0f, 11.0f, 12.0f};

    memset(gSzView_CameraReq_req, 0, sizeof(gSzView_CameraReq_req));
    memset(&camera, 0, sizeof(camera));
    camera.polar = polar;
    camera.direction = direction;
    camera.position = position;
    camera.roll = 0x100;
    camera.fov = 45.75f;
    gSzView_hView_req[0] = (DotrViewReqWord)&camera;
    gSzView_hView_req[1] = (DotrViewReqWord)&camera;
}

static int same(const struct DotrViewReqVec *a, const struct DotrViewReqVec *b)
{
    return a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w;
}

static void check_header(const struct DotrViewReqSlot *slot, int type, int owner, int target, int count)
{
    assert(slot->active != 0 && slot->type == type && slot->flag == 0 && slot->other == 0);
    assert(slot->owner == owner && slot->target == target && slot->count == count && slot->timer == 0);
}

/* Refusals: a view other than 0 (either side), a null view block, no free slot, an unknown
 * type (nothing is marked). */
static void check_refusals(void)
{
    int i;

    reset();
    assert(SzView_ReqCamera(1, 0, 0, 5, 0, NULL) == 0);
    assert(SzView_ReqCamera(-1, 0, 0, 5, 0, NULL) == 0);
    assert(gSzView_CameraReq_req[0].active == 0);
    gSzView_hView_req[0] = 0;
    assert(SzView_ReqCamera(0, 0, 0, 5, 0, NULL) == 0);
    reset();
    for (i = 0; i < 8; i++) {
        gSzView_CameraReq_req[i].active = 1;
    }
    assert(SzView_ReqCamera(0, 0, 0, 5, 0, NULL) == 0);
    reset();
    assert(SzView_ReqCamera(0, 0, 10, 5, 0, NULL) == 0);
    assert(SzView_ReqCamera(0, 0, -1, 5, 0, NULL) == 0);
    assert(gSzView_CameraReq_req[0].active == 0);
}

/* Field of view and roll; the first free slot is taken; count 0 becomes 1, a wider count keeps
 * its low halfword. */
static void check_scalars(void)
{
    reset();
    gSzView_CameraReq_req[0].active = 1;
    gSzView_CameraReq_req[1].active = 1;
    assert(SzView_ReqCamera(0, 3, 0, 20, 60, NULL) == 1);
    check_header(&gSzView_CameraReq_req[2], 0, 0, 3, 20);
    assert(gSzView_CameraReq_req[2].from.i == 45 && gSzView_CameraReq_req[2].to.i == 60);

    reset();
    assert(SzView_ReqCamera(0, -2, 0, 0, 7, NULL) == 1);
    check_header(&gSzView_CameraReq_req[0], 0, 0, -2, 1);
    reset();
    assert(SzView_ReqCamera(0, 0, 0, 0x10005, 7, NULL) == 1);
    assert(gSzView_CameraReq_req[0].count == 5);

    /* roll: modulo 0x10000, above 0x8000 folded to negative, 0x8000 kept */
    reset();
    camera.roll = 0x18001;
    assert(SzView_ReqCamera(0, 1, 1, 4, 0x8000, NULL) == 1);
    check_header(&gSzView_CameraReq_req[0], 1, 0, 1, 4);
    assert(gSzView_CameraReq_req[0].from.i == -0x7fff && gSzView_CameraReq_req[0].to.i == 0x8000);
    reset();
    camera.roll = -5;
    assert(SzView_ReqCamera(0, 1, 1, 4, 0x2fff0, NULL) == 1);
    assert(gSzView_CameraReq_req[0].from.i == -5 && gSzView_CameraReq_req[0].to.i == -0x10);
}

/* Vector requests: direction, position, polar angles (both types), position with an integer
 * end, and the track types with their default count. */
static void check_vectors(void)
{
    struct DotrViewReqVec to = {21.0f, 2.0f, 23.0f, 24.0f};
    struct DotrViewReqTrack track;

    reset();
    assert(SzView_ReqCamera(0, 0, 2, 9, 0, &to) == 1);
    assert(same(&gSzView_CameraReq_req[0].from.v, &camera.direction) && same(&gSzView_CameraReq_req[0].to.v, &to));
    reset();
    assert(SzView_ReqCamera(0, 0, 3, 9, 0, &to) == 1);
    assert(same(&gSzView_CameraReq_req[0].from.v, &camera.position) && same(&gSzView_CameraReq_req[0].to.v, &to));

    /* polar: y 0 to 2.0 is less than half a turn apart: kept */
    reset();
    assert(SzView_ReqCamera(0, 0, 4, 9, 0, &to) == 1);
    check_header(&gSzView_CameraReq_req[0], 4, 0, 0, 9);
    assert(same(&gSzView_CameraReq_req[0].from.v, &camera.polar) && same(&gSzView_CameraReq_req[0].to.v, &to));
    /* 0 to 3.0: 0x9000 apart after rounding, so 2 pi is subtracted */
    reset();
    to.y = 3.0f;
    assert(SzView_ReqCamera(0, 0, 5, 9, 0, &to) == 1);
    assert(gSzView_CameraReq_req[0].to.v.y == 3.0f - 6.2831855f && gSzView_CameraReq_req[0].to.v.x == 21.0f);
    assert(to.y == 3.0f);
    /* 0 to -3.0: exactly -0x8000 apart, kept; to -3.5: below -0x8000, 2 pi added */
    reset();
    to.y = -3.0f;
    assert(SzView_ReqCamera(0, 0, 4, 9, 0, &to) == 1);
    assert(gSzView_CameraReq_req[0].to.v.y == -3.0f);
    reset();
    to.y = -3.5f;
    assert(SzView_ReqCamera(0, 0, 4, 9, 0, &to) == 1);
    assert(gSzView_CameraReq_req[0].to.v.y == -3.5f + 6.2831855f);

    reset();
    assert(SzView_ReqCamera(0, 0, 6, 9, 77, &to) == 1);
    assert(same(&gSzView_CameraReq_req[0].from.v, &camera.position) && gSzView_CameraReq_req[0].to.i == 77);
    reset();
    assert(SzView_ReqCamera(0, 0, 7, 9, 78, NULL) == 1);
    check_header(&gSzView_CameraReq_req[0], 7, 0, 0, 9);

    /* tracks: the count defaults to frames - 1 (low halfword), then 0 becomes 1 */
    memset(&track, 0, sizeof(track));
    track.frames = 30;
    reset();
    assert(SzView_ReqCamera(0, 0, 8, 0, 5, &track) == 1);
    check_header(&gSzView_CameraReq_req[0], 8, 0, 0, 29);
    assert(gSzView_CameraReq_req[0].from.p == &track && gSzView_CameraReq_req[0].to.i == 5);
    reset();
    assert(SzView_ReqCamera(0, 0, 9, 12, 5, &track) == 1);
    check_header(&gSzView_CameraReq_req[0], 9, 0, 0, 12);
    track.frames = 1;
    reset();
    assert(SzView_ReqCamera(0, 0, 9, 0, 5, &track) == 1);
    assert(gSzView_CameraReq_req[0].count == 1);
    track.frames = 0;
    reset();
    assert(SzView_ReqCamera(0, 0, 8, 0, 5, &track) == 1);
    assert(gSzView_CameraReq_req[0].count == 0xffff);
}

int main(void)
{
    check_refusals();
    check_scalars();
    check_vectors();
    return 0;
}
