/* Synthetic host tests for the model_load group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/model_load.h"

struct DotrModelLoadObject *gSzModel_ahModel[84];
unsigned short AlternateCardIds[16];
unsigned short gSzDuel_Camera[16];
char data_002cfa17[] = "DBDIR";
char data_002cfa2a[] = "LABEL";

static int gnode_calls, gnode_index, gnode_unknown, *gnode_flag;
static int env_calls, module_calls, module_args[3];
static const char *env_args[2];
int lSzModel_GetGNode(int index, int unknown, int *flag)
{
    gnode_index = index; gnode_unknown = unknown; gnode_flag = flag; gnode_calls++;
    return 0;
}
char *SzFile_GetEnv(const char *label) { env_args[env_calls++ & 1] = label; return "root"; }
void SzEffect_LoadModule(int a, int b, int c)
{
    module_args[0] = a; module_args[1] = b; module_args[2] = c; module_calls++;
}

/* 0x1e8bf0 branch coverage: index at the bound (a negative index cannot occur after the
 * mask), empty slot, no file, each of kinds 3..7, scale above 1, below 0.3 and between,
 * first selector valid / absent with a later motion / absent with none, other selectors
 * negative, absent and present. */
static void check_load_callback(void)
{
    static struct DotrModelLoadObject obj;
    static union { struct DotrModelLoadHeader h; int words[0x100]; } file;
    struct DotrModelLoadNode *node = (struct DotrModelLoadNode *)&file.words[0x40];
    int kind;

    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    lSzModel_LoadCB(84);
    lSzModel_LoadCB(5);
    assert(gnode_calls == 0);

    memset(&obj, 0, sizeof(obj));
    gSzModel_ahModel[5] = &obj;
    obj.loading = -1; obj.unknown_bit_0 = 3;
    lSzModel_LoadCB(5);
    assert(obj.loading == 0 && obj.unknown_bit_0 == 3 && obj.index == 0 && gnode_calls == 0);

    AlternateCardIds[9] = 3; gSzDuel_Camera[3] = 0xd234;
    for (kind = 3; kind <= 7; kind++) {
        int special = kind >= 4 && kind <= 6;
        memset(&obj, 0, sizeof(obj));
        memset(&file, 0, sizeof(file));
        obj.base = (char *)&file; obj.desc = &file.h;
        file.h.node_offset = 0x100; file.h.flags = 0x7c0 | 0x15;
        node->x = 1.0f; node->y = 2.0f; node->z = 3.0f; node->radius = 80.0f;
        obj.kind = kind; obj.id = 9; obj.look = 0x3fff; obj.loading = -1;
        obj.heads[0] = 1;
        obj.motion[1] = -1; obj.motion[2] = 3; obj.motion[3] = 0;
        gnode_calls = env_calls = module_calls = 0;
        lSzModel_LoadCB(0x400 | 5);
        assert(obj.loading == 0 && obj.index == 5 && obj.node == node && obj.level == 0x1f);
        assert(gnode_calls == 1 && gnode_index == 5 && gnode_unknown == -1);
        assert(gnode_flag == &file.h.gnode_flag);
        assert(obj.pos[0] == 1.0f && obj.pos[1] == 2.0f && obj.pos[2] == 3.0f && obj.pos[3] == 1.0f);
        assert(obj.radius == 80.0f && obj.scale == 0.5f);
        assert(obj.motion[0] == 0 && obj.motion[1] == -1 && obj.motion[2] == 0 && obj.motion[3] == 0);
        if (special) {
            assert(obj.loaded_id == 9 && obj.look == 0x1234);
            assert(env_calls == 2 && env_args[0] == data_002cfa17 && env_args[1] == data_002cfa2a);
            assert(file.h.flags == (0x3c0 | 0x15));
            assert(module_calls == 1 && module_args[0] == 1 && module_args[1] == 9 && module_args[2] == 0);
        } else {
            assert(obj.loaded_id == 0 && obj.look == 0x3fff && env_calls == 0 && module_calls == 0);
            assert(file.h.flags == (0x7c0 | 0x15));
        }
    }

    obj.kind = 0;
    node->radius = 20.0f;
    obj.heads[0] = 0; obj.heads[6] = 1; obj.motion[0] = 2; obj.motion[1] = 6; obj.motion[2] = 1;
    lSzModel_LoadCB(5);
    assert(obj.scale == 1.0f && obj.motion[0] == 6 && obj.motion[1] == 6 && obj.motion[2] == 6);

    node->radius = 400.0f;
    obj.heads[6] = 0; obj.motion[0] = 2; obj.motion[1] = -3;
    lSzModel_LoadCB(5);
    assert(obj.scale == 0.3f && obj.motion[0] == 0 && obj.motion[1] == -3 && obj.motion[2] == 0);
    gSzModel_ahModel[5] = 0;
}

int main(void)
{
    check_load_callback();
    return 0;
}
