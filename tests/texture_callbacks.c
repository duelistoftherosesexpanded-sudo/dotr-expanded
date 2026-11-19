/* Synthetic host tests for the texture_callbacks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/texture_callbacks.h"

static char arena[4096];
char *dotr_texture_callbacks_base = arena;

static char order[16];
static int calls;
static void *bound, *conv_tex, *freed;
static char conv_text[32];
static char converted;

static void said(char c)
{
    assert(calls < (int)sizeof order - 1);
    order[calls++] = c;
    order[calls] = 0;
}

void RhText_BindAddress(void *header)
{
    said('B');
    bound = header;
}
void *func_001e9340_texcb(void *tex, char *text)
{
    said('C');
    conv_tex = tex;
    strcpy(conv_text, text);
    return &converted;
}
void SzMem_Free(void *pointer)
{
    said('F');
    freed = pointer;
}

static void reset(void)
{
    memset(arena, 0, sizeof arena);
    calls = 0;
    order[0] = 0;
    bound = conv_tex = freed = NULL;
    conv_text[0] = 0;
}

/* The entry list at byte offset 256 of the arena; index in the top six bits. */
static struct DotrTexCallbackEntry *list(void)
{
    return (struct DotrTexCallbackEntry *)(arena + 256);
}

static void check_texture(void)
{
    struct DotrTexCallbackRecord req;
    char file[8];

    memset(&req, 0, sizeof req);
    req.data = file;
    req.user = 256 | (3 << 26);

    reset();
    lSzModel_LoadTextureFileCB(&req, 0);
    assert(calls == 0 && list()[3].tex == NULL);

    reset();
    lSzModel_LoadTextureFileCB(&req, 1);
    assert(strcmp(order, "B") == 0 && bound == file);
    assert(list()[3].tex == file && list()[2].tex == NULL && list()[4].tex == NULL && list()[3].anim == NULL);

    reset();
    req.user = 256;
    lSzModel_LoadTextureFileCB(&req, 1);
    assert(list()[0].tex == file && bound == file);
}

static void check_anim(void)
{
    struct DotrTexCallbackRecord req;
    char text[16] = "ANIMDATA";
    char tex[4];

    memset(&req, 0, sizeof req);
    req.user = 256 | (2 << 26);
    req.size = 5;
    req.data = text;

    reset();
    lSzModel_LoadTexAnimFileCB(&req, 0);
    assert(calls == 0 && list()[2].anim == NULL && strcmp(text, "ANIMDATA") == 0);

    /* Completed with no data: the null is stored and nothing else happens. */
    reset();
    list()[2].anim = text;
    req.data = NULL;
    lSzModel_LoadTexAnimFileCB(&req, 1);
    assert(calls == 0 && list()[2].anim == NULL);

    /* Completed with data: the text is ended at size - 1, converted and freed. */
    reset();
    list()[2].tex = tex;
    req.data = text;
    lSzModel_LoadTexAnimFileCB(&req, 1);
    assert(strcmp(order, "CF") == 0);
    assert(text[4] == 0 && strcmp(conv_text, "ANIM") == 0 && conv_tex == tex);
    assert(freed == text && list()[2].anim == &converted && list()[1].anim == NULL);
}

int main(void)
{
    check_texture();
    check_anim();
    puts("texture_callbacks: ok");
    return 0;
}
