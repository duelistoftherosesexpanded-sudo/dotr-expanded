/* Synthetic host tests for the texanim_conv group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/texanim_conv.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

char *apchMode[7];
char data_002cfa72[4], data_002cfa75[4], data_002cfaa2[4], data_002cfac2[4], data_002cfaf2[4], data_002cfb16[4];
char data_002cfb18[4], data_002cfb32[4], data_002cfb34[4], data_002cfb3a[4], data_002cfb3c[4], data_002cfb40[4];
char data_002cfb45[4], data_002cfb4b[4], data_002cfb4d[4];

static int tokens;

int func_0017da98(const char *arg0, ...) { return 0; }
int func_001950a0(const char *s) { return 0; }
int func_0019b6e0(const char *str, const char *fmt, ...) { return 0; }
int func_0019b8f8(const char *s1, const char *s2) { return 0; }
int func_0019bc90(const char *a, const char *b) { return 0; }
char *func_0019bdd8(char *dst, const char *src) { return dst; }
int func_0019bef0(const char *s) { return 0; }
int func_0019c028(const char *s1, const char *s2, unsigned int n) { return 0; }
/* The tokeniser finds no line at all. */
char *func_0019d430(char *s, const char *delim)
{
    tokens++;
    return NULL;
}
DotrTexAnimWord SzMem_Alloc(int arg0) { return 0; }

int main(void)
{
    static unsigned char text[64];

    /* An empty description: nothing is allocated, the text buffer's first entry is marked as the
     * last one with no frames, and the result is null. */
    memset(text, 0xff, sizeof text);
    assert(lSzModel_TexAnimConv(0, text) == NULL && tokens == 1);
    assert(((struct DotrTexAnimEntry *)text)->last == 1 && ((struct DotrTexAnimEntry *)text)->lo == 0);
    assert(((struct DotrTexAnimEntry *)text)->hi == 0);
    return 0;
}
