#ifndef DOTR_EE_MODEL_CALLBACKS_H
#define DOTR_EE_MODEL_CALLBACKS_H

/* Load callbacks of the SzModel requests (lSzModel_LoadModelCB at 0x1f01a0 and its
 * neighbours): they receive the resource submit record, pick the gSzModel_ahModel object by
 * the record word at 0x1c and call the object_table loaders.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

/* Format string at 0x2cfb94, used with the "DBDIR" answer and the kind string twice;
 * its text is not documented here. */
extern char data_002cfb94[];
/* 0x1e90b0: assumed prototype from this call site; one int argument assumed, result unused. */
extern int SzModel_LoadTextureFile(int modelId);
/* 0x1ffc50 as resource_helpers.h declares it. */
int SzFile_EntryGroup(int fileType, int value);

/* 0x1f01a0, the callback lSzModel_LoadModel and lSzModel_LoadModelPackCB pass to
 * SzFile_ReadRequest. The object is the object_table view; the word at +0x13c is
 * written through a cast of that view's byte member. The switch on the kind byte
 * has a compiler-made table at 0x2cf970 (8 words). Assumed: parameter types from
 * the callback type, block-local buffers in the two switch arms (two pairs of
 * 128-byte buffers in the frame), an int number. */
void lSzModel_LoadModelCB(struct DotrResourceSubmitRecord *record, int completed);

#endif
