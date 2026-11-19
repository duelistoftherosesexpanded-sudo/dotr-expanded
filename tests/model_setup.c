/* Synthetic host tests for the model_setup group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/model_setup.h"

struct DotrSetupCoordinate require_coordinate[4];
int require_coordinate_no[1];
int require_coordinate_index;
struct DotrSetupVec3 data_00295080;
struct DotrSetupPad dotr_setup_pad;

/* Every stub appends one entry to a trace string. */
static char trace[512];
static struct DotrSetupNode nodes[4];
static float matrices[4][4];

static void add(const char *format, ...);
#include <stdarg.h>
static void add(const char *format, ...)
{
    va_list arguments;
    size_t used = strlen(trace);
    va_start(arguments, format);
    vsnprintf(trace + used, sizeof(trace) - used, format, arguments);
    va_end(arguments);
}

void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    assert(((size_t)destination & 15) == 0);
    assert(count == 1 || (count == 3 && destination == (void *)&dotr_setup_pad));
    memcpy(destination, source, (size_t)count * 16);
    return destination;
}
void RhGeomEx_PushMatrix(void)
{
    add("P;");
}
void RhGeomEx_PopMatrix(void)
{
    add("O;");
}
void RhGeomEx_GetMatrix(void *matrix)
{
    add("G%d;", (int)((float (*)[4])matrix - matrices));
}
void RhGeom_ScaleMatrix(void *matrix, float *scale)
{
    if (matrix == NULL) {
        add("K-:%g,%g,%g;", scale[0], scale[1], scale[2]);
    } else {
        add("K%d:%g,%g,%g;", (int)((float (*)[4])matrix - matrices), scale[0], scale[1], scale[2]);
    }
}
/* rot is printed as the node index, `p` for the scratchpad copy or `-` for null. */
void RhGeomEx_ScaleRotXYZTransMatrix(void *unknown, void *rot, void *trans)
{
    float *values = trans;
    assert(unknown == NULL);
    if (rot == (void *)dotr_setup_pad.rot) {
        add("S,rp");
    } else if (rot != NULL) {
        add("S,r%d", (int)((struct DotrSetupNode *)((char *)rot - offsetof(struct DotrSetupNode, rot)) - nodes));
    } else {
        add("S,r-");
    }
    if (trans != NULL) {
        add(",t:%g,%g,%g;", values[0], values[1], values[2]);
    } else {
        add(",t-;");
    }
}

/* Motion walkers' callees. The item stubs return the next configured result. */
static char items0[64];
static char items1[64];
static unsigned int item_results[4];
static int item_calls;
static int view_results[4];
static int view_calls;

static void check_item_outputs(float *position, int *rotation, float *scale)
{
    assert(position == &dotr_setup_pad.position.x && rotation == dotr_setup_pad.rot && scale == dotr_setup_pad.scale);
}
unsigned int lRhModel_GetMultiMotionItem(void *item0, void *item1, float frame0, float rate0, float frame1, float rate1,
                                         unsigned int flags, float *position, int *rotation, float *scale, float rate)
{
    check_item_outputs(position, rotation, scale);
    add("I%d,%d,%g,%g,%g,%g,%x,%g;", (int)((char *)item0 - items0), (int)((char *)item1 - items1), frame0, rate0,
        frame1, rate1, flags, rate);
    return item_results[item_calls++];
}
unsigned int lRhModel_GetInterpolateMotionItem(void *data, unsigned short flags, float frame, float *position,
                                               int *rotation, float *scale, float rate)
{
    check_item_outputs(position, rotation, scale);
    add("I%d,%x,%g,%g;", (int)((char *)data - items0), flags, frame, rate);
    return item_results[item_calls++];
}
unsigned int lRhModel_GetPulseMotionItem(void *data, int frame, unsigned short flags, float *position, int *rotation,
                                         float *scale, float rate)
{
    check_item_outputs(position, rotation, scale);
    add("I%d,%d,%x,%g;", (int)((char *)data - items0), frame, flags, rate);
    return item_results[item_calls++];
}
int func_001a22c0(float value)
{
    return (int)value + 100;
}
int *RhMem_CopyMemory4(int *destination, int *source, int count)
{
    assert(destination == (int *)&dotr_setup_pad && count == 3);
    add("C%d;", (int)((struct DotrSetupNode *)source - nodes));
    memcpy(destination, source, 12);
    return destination;
}
int RhView_CheckViewVolume(struct DotrSetupVec4 *centre, float radius)
{
    assert(centre == &dotr_setup_pad.position);
    add("V:%g,%g,%g,%g,%g;", centre->x, centre->y, centre->z, centre->w, radius);
    return view_results[view_calls++];
}
void RhGeom_GetMatrix(float *matrix)
{
    add("M%d;", (int)((struct DotrSetupCoordinate *)((char *)matrix - offsetof(struct DotrSetupCoordinate, matrix)) -
                      require_coordinate));
}

static void set_node(int index, int flags, int matrix, float scale, float trans, unsigned int child, unsigned int next)
{
    struct DotrSetupNode *node = &nodes[index];
    memset(node, 0, sizeof(*node));
    node->flags = (unsigned short)flags;
    node->matrix = (unsigned short)matrix;
    node->scale[0] = scale; node->scale[1] = scale + 1; node->scale[2] = scale + 2;
    node->trans[0] = trans; node->trans[1] = trans + 1; node->trans[2] = trans + 2;
    node->child = child;
    node->next = next;
}

/* Branch coverage: type 3 against the other types, each of the flag bits 0, 1 and 2 set and
 * clear, matrix index zero and non-zero, a coordinate entry found (first and second entry),
 * not found, and no entries at all, child present and absent, next present and absent, and
 * the 28-bit mask on both offsets. */
static void check_no_motion(void)
{
    struct DotrSetupPad model;
    float parent[4] = { 1, 2, 3, 0 };
    unsigned int size = sizeof(struct DotrSetupNode);

    memset(&model, 0, sizeof(model));
    model.nodes = (char *)nodes;
    model.matrices = (char *)matrices;
    set_node(0, 0, 2, 2, 1, 0, 0x30000000u | size);
    set_node(1, 7, 0, 5, 5, 0xf0000000u | (2 * size), 0);
    set_node(2, 1, 1, 2, 1, 0, 3 * size);
    set_node(3, 2, 0, 1, 0, 0, 0);
    memset(require_coordinate, 0, sizeof(require_coordinate));
    require_coordinate[0].index = 11;
    require_coordinate[1].index = 12;
    require_coordinate[2].index = 13; /* beyond the count: never matched */
    require_coordinate_no[0] = 2;
    require_coordinate_index = 10;
    trace[0] = 0;
    lRhModel_SetupNoMotionRecursive(&model, &nodes[0], parent);
    assert(strcmp(trace, "P;S,r0,t:1,4,9;G2;K2:2,6,12;O;"
                         "M0;"
                         "P;S,r2,t-;G1;K1:2,6,12;M1;O;"
                         "P;S,r-,t:0,2,6;O;") == 0);
    assert(require_coordinate_index == 14);
    assert(require_coordinate[0].flag == 1 && require_coordinate[1].flag == 1 && require_coordinate[2].flag == 0);
    assert(parent[0] == 1 && parent[1] == 2 && parent[2] == 3);

    /* No entries in use: nothing is looked up, the index still counts. */
    set_node(0, 7, 0, 1, 1, 0, 0);
    require_coordinate[0].index = 20;
    require_coordinate[0].flag = 0;
    require_coordinate_no[0] = 0;
    require_coordinate_index = 20;
    trace[0] = 0;
    lRhModel_SetupNoMotionRecursive(&model, &nodes[0], parent);
    assert(trace[0] == 0 && require_coordinate_index == 21 && require_coordinate[0].flag == 0);
}

/* The three motion walkers share one fixture; `kind` selects the walker and the text its item
 * stub prints for (item offset, flags, rate). */
typedef int (*Walker)(struct DotrSetupNode *node, int index, float *parent);
static const char *item_text(int kind, int offset, unsigned int flags, const char *rate)
{
    static char text[4][64];
    static int next;
    char *out = text[next++ & 3];
    if (kind == 0) {
        sprintf(out, "I%d,%d,7,0.25,9,0.75,%x,%s;", offset, offset, flags, rate);
    } else if (kind == 1) {
        sprintf(out, "I%d,%x,7,%s;", offset, flags, rate);
    } else {
        sprintf(out, "I%d,107,%x,%s;", offset, flags, rate);
    }
    return out;
}
static void set_sphere(int index, float x, float y, float z, float radius)
{
    nodes[index].sphere.x = x; nodes[index].sphere.y = y; nodes[index].sphere.z = z; nodes[index].sphere.w = radius;
}
static void reset_pad(int motion_flags, int count)
{
    memset(&dotr_setup_pad, 0, sizeof(dotr_setup_pad));
    dotr_setup_pad.nodes = (char *)nodes;
    dotr_setup_pad.matrices = (char *)matrices;
    dotr_setup_pad.item0 = items0;
    dotr_setup_pad.item1 = items1;
    dotr_setup_pad.frame0 = 7; dotr_setup_pad.frame1 = 9;
    dotr_setup_pad.rate0 = 0.25f; dotr_setup_pad.rate1 = 0.75f;
    dotr_setup_pad.rate = 0.5f;
    dotr_setup_pad.count = count;
    dotr_setup_pad.motion_flags = (unsigned short)motion_flags;
    memset(require_coordinate, 0, sizeof(require_coordinate));
    item_calls = 0;
    view_calls = 0;
    trace[0] = 0;
}

/* Branch coverage, per walker: index below the count and not, 24- and 16-byte items, first
 * node and later nodes (rate 1 against the pad's rate, the scale copy with and without motion
 * flag bit 2, root pointer set and null), item results with type 3 and with each of bits 0, 1
 * and 2, matrix index zero and non-zero, node flag bit 15 set and clear, the largest scale in
 * each of the three positions, a coordinate entry found, not found and none in use, child and
 * next present and absent with the 28-bit mask, and the returned index. */
static void check_motion(Walker walk, int kind)
{
    float parent[4] = { 1, 2, 3, 0 };
    float ones[4] = { 1, 1, 1, 0 };
    struct DotrSetupVec4 root;
    unsigned int size = sizeof(struct DotrSetupNode);
    char expected[512];

    /* 24-byte items, two of three nodes have one, root wanted, second node's entry requested. */
    reset_pad(4, 2);
    set_node(0, 0, 1, 2, 1, size, 0x30000000u | (2 * size));
    set_sphere(0, 5, 6, 7, 2);
    set_node(1, 0x8000, 0, 1, 4, 0, 0);
    set_node(2, 0x8003, 0, 3, 0, 0, 0);
    nodes[2].scale[1] = 1; nodes[2].scale[2] = 1;
    memset(&root, 0, sizeof(root));
    dotr_setup_pad.root = &root;
    item_results[0] = 0; item_results[1] = 7;
    view_results[0] = 2;
    require_coordinate[0].index = 11;
    require_coordinate_no[0] = 1;
    require_coordinate_index = 10;
    assert(walk(&nodes[0], 0, parent) == 3);
    sprintf(expected, "%sC0;P;S,rp,t:1,4,9;G1;K1:2,6,12;P;K-:2,6,12;V:5,6,7,1,12;O;%sM0;O;", item_text(kind, 0, 0, "1"),
            item_text(kind, 24, 0x8000, "0.5"));
    assert(strcmp(trace, expected) == 0);
    assert(root.x == 1 && root.y == 2 && root.z == 3 && memcmp(&root.w, &nodes[0].next, 4) == 0);
    assert(dotr_setup_pad.visible == 2 && require_coordinate_index == 13 && require_coordinate[0].flag == 1);
    assert(item_calls == 2 && view_calls == 1);

    /* 16-byte items, no root, no entries; the largest scale first and in the middle. */
    reset_pad(0, 2);
    set_node(0, 0, 0, 0, 1, 0, size);
    nodes[0].scale[0] = 4; nodes[0].scale[1] = 2; nodes[0].scale[2] = 1;
    nodes[0].trans[1] = 1; nodes[0].trans[2] = 1;
    set_sphere(0, 1, 2, 3, 1);
    set_node(1, 0, 0, 0, 2, 0, 0);
    nodes[1].scale[0] = 1; nodes[1].scale[1] = 5; nodes[1].scale[2] = 2;
    nodes[1].trans[1] = 2; nodes[1].trans[2] = 2;
    set_sphere(1, 0, 0, 0, 3);
    dotr_setup_pad.visible = 1;
    item_results[0] = 1; item_results[1] = 2;
    view_results[0] = 4; view_results[1] = 8;
    require_coordinate_no[0] = 0;
    require_coordinate_index = 0;
    assert(walk(&nodes[0], 0, ones) == 2);
    sprintf(expected, "%sP;S,rp,t-;P;K-:4,2,1;V:1,2,3,1,2;O;O;%sP;S,r-,t:2,2,2;P;K-:1,5,2;V:0,0,0,1,7.5;O;O;",
            item_text(kind, 0, 0, "1"), item_text(kind, 16, 0, "0.5"));
    assert(strcmp(trace, expected) == 0);
    assert(dotr_setup_pad.visible == 13 && require_coordinate_index == 2);
}

int main(void)
{
    check_no_motion();
    check_motion(lRhModel_SetupMultiMotionRecursive, 0);
    check_motion(lRhModel_SetupFloatMotionRecursive, 1);
    check_motion(lRhModel_SetupPulseMotionRecursive, 2);
    puts("model_setup: ok");
    return 0;
}
