#ifndef DOTR_EE_TEXTURE_CALLBACKS_H
#define DOTR_EE_TEXTURE_CALLBACKS_H

/* Read-request callbacks of SzModel_LoadTextureFile that store a loaded texture or texture
 * animation file into the entry packed in the request's user word.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* One 16-byte entry of the texture list SzModel_LoadTextureFile (0x1e90b0, object_table)
 * builds; it clears the words at +0, +8 and +12 before the requests. Assumed: +0 the bound
 * texture file, +8 the converted texture animation; +4 and +12 not used here. */
struct DotrTexCallbackEntry {
    void *tex;
    int unknown_04;
    void *anim;
    int unknown_0c;
};

/* View of the read request record handed to the callbacks (resource_submit.h has a fuller
 * view): the loaded size as a 31-bit field at +8, the loaded data at +0x14 and the user word
 * at +0x1c. On the host the pointer moves the later offsets. */
struct DotrTexCallbackRecord {
    char unknown_00[8];
    int size : 31;
    int flag : 1;
    char unknown_0c[8];
    void *data;
    int unknown_18;
    int user;
};

/* The user word packs the entry list address in its low 26 bits and the entry index in its
 * top six (SzModel_LoadTextureFile builds it that way). The target uses the address as is;
 * the host build adds the low bits to a test-owned base instead. */
#ifdef __MWERKS__
#define DOTR_TEX_CALLBACK_LIST(word) ((struct DotrTexCallbackEntry *)((word) & 0x3ffffff))
#else
extern char *dotr_texture_callbacks_base;
#define DOTR_TEX_CALLBACK_LIST(word) ((struct DotrTexCallbackEntry *)(dotr_texture_callbacks_base + ((word) & 0x3ffffff)))
#endif

/* RhText_BindAddress (0x20f250, small_misc): binds a loaded texture file; result unused. */
extern void RhText_BindAddress(void *header);
/* lSzModel_TexAnimConv (0x1e9340, texanim_conv). Assumed: (texture file, text of the
 * animation file) -> the converted animation stored in the entry. */
extern void *func_001e9340_texcb(void *tex, char *text);
/* SzMem_Free (0x217b50, resource_support). */
extern void SzMem_Free(void *pointer);

/* 0x1e9280 lSzModel_LoadTextureFileCB: when the read completed, stores the loaded data as
 * the entry's texture and binds it. MATCHING DEVICE: one variable holds the user word and
 * then the index (separate variables swap registers). */
void lSzModel_LoadTextureFileCB(struct DotrTexCallbackRecord *req, int ok);
/* 0x1e92c0 lSzModel_LoadTexAnimFileCB: when the read completed, stores the data in the
 * entry's animation word; if it is not null, ends the text at size - 1, replaces the word
 * with the converted animation and frees the text. */
void lSzModel_LoadTexAnimFileCB(struct DotrTexCallbackRecord *req, int ok);

#endif
