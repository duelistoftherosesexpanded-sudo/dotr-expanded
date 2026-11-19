/* Host smoke test for main (0x1000d0), the program entry.
 * Fixtures are host data, not game data; stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* The inline block that sets the stack pointer to 0x912000 is target-only (`#ifdef __MWERKS__`); the
 * host build runs on its own stack. Built without -Werror: the C passes string addresses as 32-bit
 * words, which warns on a 64-bit host; the stub only records the low bits it compares. */
#include <assert.h>
#include "dotr/ee/program_main.h"

/* The host build compiles the source with main renamed (CMakeLists.txt), so this file can have its own. */
s32 dotr_program_main(void);

char data_002b8f00[8], data_002b8f05[8], data_002b8f07[8], data_002b8f0d[8], data_002b8f13[8], data_002b8f18[8];
char data_002b8f1f[8], data_002b8f27[8], data_002b8f2c[8], data_002b8f32[8];

static char order[16];
static int count;
static int registered;
static s32 pairs[7][3];
static s32 init_result;

void SzMem_Init(void) { order[count++] = 'm'; }
void func_0019b0f8(s32 arg0, s32 arg1, s32 arg2)
{
    order[count++] = 'r';
    pairs[registered][0] = arg0;
    pairs[registered][1] = arg1;
    pairs[registered][2] = arg2;
    registered++;
}
s32 Main_Init(void) { order[count++] = 'i'; return init_result; }
void Main_Loop(void) { order[count++] = 'l'; }
void Main_Term(void) { order[count++] = 't'; }

static int same(const char *a, const char *b)
{
    while (*a != 0 && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

int main(void)
{
    /* Ordinary start: memory, seven 0x19b0f8 calls, init succeeds, loop, term. */
    init_result = 1;
    assert(dotr_program_main() == 0);
    assert(same(order, "mrrrrrrrilt"));
    assert(pairs[0][0] == (s32)(long)data_002b8f00 && pairs[0][1] == (s32)(long)data_002b8f05 && pairs[0][2] == 1);
    assert(pairs[3][1] == (s32)(long)data_002b8f18 && pairs[6][0] == (s32)(long)data_002b8f2c);
    assert(pairs[6][1] == (s32)(long)data_002b8f32 && pairs[6][2] == 1);
    return 0;
}
