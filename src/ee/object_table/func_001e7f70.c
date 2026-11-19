#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* SzModel_LoadMerge (0x1e7f70): loads the shared model pack of one of seven types (names
 * from the Japanese build's symbol table: SzModel_LoadMerge, aShareData, anOfs_MODEL).
 * One loop is written with a goto; the data are reached through views (object_table.h). */
int SzModel_LoadMerge(int modelType, int arg1, int arg2)
{
    int *start;
    int value;
    struct DotrModelMergeWords sources;
    int fileIndex;
    int offset;
    int length;
    int file;
    struct DotrModelMergeInts types;
    struct DotrModelMergeInts counts;
    struct DotrModelMergeInts starts;
    char *path;
    char filePath[128];
    struct DotrModelShare *info;
    short *commonSource;
    int i;

    if (modelType < 0 || modelType >= 7) {
        return 0;
    }

    sources = data_00292be0;
    sources.v[0] = (short *)&gSzDuel_DuelData_merge[arg1 * 10];
    sources.v[1] = &gSzDuel_RingData_merge[arg1];
    sources.v[2] = (short *)&gSzDuel_StageData_merge[24 * arg1];
    commonSource = (short *)&gSzDuel_EventData_merge[arg1 * 80];
    sources.v[3] = commonSource;
    sources.v[4] = commonSource;
    sources.v[5] = commonSource;
    sources.v[6] = (short *)&gSzDuel_FieldData_merge[arg1 * 20];
    types = data_00292c10;
    counts = data_00292c40;
    starts = data_00292c70;
    fileIndex = arg1 + *(unsigned short *)((unsigned char *)data_00292ba4_merge + modelType * 8);
    offset = anOfs_MODEL_merge[fileIndex];
    length = data_00292a54_merge[fileIndex] - offset;
    info = &aShareData[modelType];
    file = info->addr;
    path = filePath;
    func_0019b648(path, data_002cf9dd, (DotrMergeAddr)SzFile_GetEnv(data_002cf9f0));
    if (4 != modelType) {
        if (SzText_GetItem(0, 0) != NULL) {
            SzText_Free(0, 0);
            SzText_Free(1, -1);
        }
    }
    if (3 != modelType) {
        SzText_Free(0, 5);
        SzText_Free(0, 6);
        SzText_Free(7, -1);
        SzText_Free(8, -1);
    }
    if (modelType == 4) {
        counts.v[7] = 9;
    }

    if (arg2 == 0) {
        int j;
        int id;
        int count;
        int *start;
        int *amount;

        start = starts.v;
        amount = counts.v;
        i = 0;
        do {
            if ((1 << i) & info->mask) {
                id = *start;
                count = *amount;
                j = 0;
                goto test;
                body:
                    if (SzModel_IsModelStandby(id) != 0) {
                        arg2 = 1;
                    }
                    id++;
                    j++;
                test:
                if (j < count) goto body;
            }
            i++;
            start++;
            amount++;
        } while (i < 9);

        {
            int *start;
            unsigned short *limit;
            int *amount;
            short *source;
            int i;

            source = sources.v[modelType];
            if (source != NULL) {
                i = 0;
                start = &starts.v[modelType];
                amount = &counts.v[modelType];
                limit = &gSzModel_awIgnore[types.v[modelType]];
                while (i < *amount) {
                    value = *source;
                    id = i + *start;
                    if (value < *limit && value != SzModel_GetModelNo(id)) {
                        arg2 = 1;
                    }
                    source++;
                    i++;
                }
            }
        }
    }

    if (0 == arg2) {
        return 1;
    }

    {
        int *amount;
        int *type;
        int i;
        int j;

        start = starts.v;
        amount = counts.v;
        type = types.v;
        i = 0;
        do {
            if ((1 << i) & info->mask) {
                j = 0;
                while (j < *amount) {
                    SzModel_Preset(j + *start, -1, 999);
                    j++;
                }
                if (*type == 4) {
                    j = 1;
                    do {
                        SzDuel_DelUnit(0, j);
                        SzDuel_DelUnit(1, j);
                        j++;
                    } while (j < 11);
                }
            }
            i++;
            start++;
            amount++;
            type++;
        } while (i < 9);
    }

    {
        i = 0;
        while (i < counts.v[modelType]) {
            SzModel_Preset(starts.v[modelType] + i, -1, sources.v[modelType][i]);
            i++;
        }
        if (func_001fecb0_merge(modelType, 0, (DotrMergeAddr)path, file, length, offset, length,
                                (DotrMergeAddr)lSzModel_LoadMergeCB, fileIndex) == 0) {
            return 0;
        }
        i = 0;
        while (i < counts.v[modelType]) {
            struct DotrObject *obj = gSzModel_ahModel[i + starts.v[modelType]];
            if (obj != NULL) {
                DOTR_MERGE_FLAGS(obj)->pending = 0;
                DOTR_MERGE_FLAGS(obj)->active = 1;
            }
            i++;
        }
    }
    return 1;
}
