#include "dotr/ee/text_merge.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* SzText_LoadMerge (0x2009e0): loads parts of the merged text/texture archives into the
 * SzText item table. Mode 0 frees the models and units and the old groups, then loads nine
 * 0x4800-byte-record blocks of group 0; 1 one whole file; 2/3 the 41 card images of one
 * side's deck and leader (sorted, special cards exchanged); 10 the 17 constant-deck cards;
 * 4 and 7 run 5+6 and 8+9; 5/6 one leader picture; 8/9 the three leaders of a side; 11..17
 * one fixed file each (15..17 a range from anOfs_TEXETC). Each mode formats its file name
 * from the SzFile_GetEnv directory and a gSzText_apchMerge entry.
 * The item search is a static inline helper; the words at 0x293cc0.. and 0x293cd0.. are
 * elements of named arrays, and SzFile_GetEnv gets the label as a pointer, not an int. */
static inline struct DotrTextMergeItem *text_merge_find(int cat, int type)
{
    struct DotrTextMergeItem *p;
    int i;

    p = gSzText_aItem;
    if (type < 0) {
        return NULL;
    }
    for (i = 0; i < 0x400; i++, p++) {
        if (p->ptr != NULL && p->cat == cat && p->type == type) {
            return p;
        }
    }
    return NULL;
}

int SzText_LoadMerge(int arg0, int arg1)
{
    char name[0x80];
    unsigned short ids[41];
    unsigned short list[17];
    unsigned char *dir;
    int result;
    int i;
    int j;
    int idx;
    int base;
    int n;
    int id;
    int got;
    int sector;
    int pos;
    int len;
    int off;
    unsigned short t;

    dir = SzFile_GetEnv(data_002d074b);
    result = 0;
    if (arg0 < 0 || arg0 >= 18) {
        return 0;
    }
    switch (arg0) {
    case 0:
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[0]);
        if (text_merge_find(0, 0) == NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 1) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 5) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 6) != NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        for (i = 0; i < 5; i++) {
            SzModel_Preset(i, -1, 999);
        }
        for (i = 0; i < 2; i++) {
            SzModel_Preset(i + 5, -1, 999);
        }
        for (i = 0; i < 1; i++) {
            SzModel_Preset(i + 7, -1, 999);
        }
        for (i = 0; i < 12; i++) {
            SzModel_Preset(i + 8, -1, 999);
        }
        for (i = 1; i < 11; i++) {
            SzDuel_DelUnit(0, i);
            SzDuel_DelUnit(1, i);
        }
        for (i = 0; i < 10; i++) {
            SzModel_Preset(i + 0x14, -1, 999);
        }
        for (i = 0; i < 2; i++) {
            SzModel_Preset(i + 0x20, -1, 999);
        }
        for (i = 0; i < 40; i++) {
            SzModel_Preset(i + 0x22, -1, 999);
        }
        for (i = 0; i < 10; i++) {
            SzModel_Preset(i + 0x4A, -1, 999);
        }
        SzText_Free(0, 1);
        SzText_Free(0, 5);
        SzText_Free(0, 6);
        SzText_Free(1, -1);
        SzText_Free(7, -1);
        SzText_Free(8, -1);
        {
            int SZ0;
            int CNT0;
            int SEC0;
            int POS0;
            int ID0;
            int K0;

            for (K0 = 0, SEC0 = 0, POS0 = 0, ID0 = 0; K0 <= 8; K0++, SEC0 += 0x1A9640, POS0 += 0x1C2000, ID0 += 0x10) {
                if (K0 < 8) {
                    CNT0 = 100;
                } else {
                    CNT0 = 71;
                }
                SZ0 = CNT0 * 0x4800;
                SzText_Free(0, ID0);
                result |= SzText_LoadFileEx(0, ID0, name, (int)((long long)SEC0 + 0xB1B000), SZ0, POS0, SZ0);
            }
        }
        break;
    case 1:
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[1]);
        if (text_merge_find(0, 1) == NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 0) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 5) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 6) != NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        SzText_Free(0, 0);
        SzText_Free(0, 1);
        SzText_Free(0, 5);
        SzText_Free(0, 6);
        SzText_Free(1, -1);
        SzText_Free(7, -1);
        SzText_Free(8, -1);
        result = SzText_LoadFileEx(0, 1, name, 0xB1B000, -1, 0, -1);
        break;
    case 2:
    case 3:
        idx = arg0 - 2;
        got = SzData_GetLeader(idx, SzData_GetCurrentDeck(idx));
        base = idx * 41 + 0xDF;
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[0]);
        ids[40] = got;
        for (i = 0; i < 40; i++) {
            ids[i] = SzDuel_InqDeck(idx, i);
        }
        for (i = 0; i < 41; i++) {
            for (j = i + 1; j < 41; j++) {
                if (ids[i] > ids[j]) {
                    t = ids[i];
                    ids[i] = ids[j];
                    ids[j] = t;
                }
            }
        }
        for (i = 0; i < 41; base++, i++) {
            id = ids[i];
            if (id != 999) {
                id = SzData_ExchSpecial(idx, id);
            }
            if (text_merge_find(1, id) == NULL) {
                result |= SzText_LoadFileEx(1, id, name, (int)((long long)(base * 0x4410) + 0xB1B000), 0x4800, id * 0x4800, 0x4800);
            }
        }
        break;
    case 10:
        n = 0;
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[0]);
        for (i = 0; i < 17; i++) {
            list[i] = SzData_GetConstDeck(i, 0);
        }
        for (i = 0; i < 17; i++) {
            for (j = i + 1; j < 17; j++) {
                if (list[i] > list[j]) {
                    t = list[i];
                    list[i] = list[j];
                    list[j] = t;
                }
            }
        }
        SzText_Free(0, 0);
        SzText_Free(0, 1);
        SzText_Free(1, -1);
        for (i = 0; i < 17; n++, i++) {
            id = list[i];
            if (text_merge_find(1, id) == NULL) {
                result |= SzText_LoadFileEx(1, id, name, (int)((long long)(n * 0x4410) + 0xB1B000), 0x4800, id * 0x4800, 0x4800);
            }
        }
        break;
    case 4:
        result = SzText_LoadMerge(5, arg1);
        result |= SzText_LoadMerge(6, arg1);
        break;
    case 5:
    case 6:
        idx = arg0 - 5;
        got = SzData_GetLeader(idx, SzData_GetCurrentDeck(idx));
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[2]);
        if (text_merge_find(2, idx) == NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        SzText_Free(2, idx);
        if (got == 999) {
            break;
        }
        id = SzData_ExchSpecial(idx, got);
        pos = idx << 12;
        sector = got << 12;
        if (id != got) {
            sector = (id - 0xAB) << 12;
        }
        result = SzText_LoadFileEx(2, idx, name, (int)((long long)pos + 0xB19000), 0x1000, sector, 0x1000);
        break;
    case 7:
        result = SzText_LoadMerge(8, arg1);
        result |= SzText_LoadMerge(9, arg1);
        break;
    case 8:
    case 9:
        {
            int ID8;
            int I8;
            int IDX8;
            int GOT8;
            int POS8;
            int SEC8;
            int K8;

            IDX8 = arg0 - 8;
            func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[2]);
            SzText_Free(0, 7);
            SzText_Free(0, 8);
            SzText_Free(0, 9);
            SzText_Free(9, -1);
            for (I8 = 0; I8 < 3; I8++) {
                GOT8 = SzData_GetLeader(IDX8, I8);
                ID8 = SzData_ExchSpecial(IDX8, GOT8);
                K8 = I8 + (IDX8 ? 20 : 10);
                POS8 = (IDX8 * 3 + I8) << 12;
                SEC8 = GOT8 << 12;
                if (ID8 != GOT8) {
                    SEC8 = (ID8 - 0xAB) << 12;
                }
                SzText_Free(2, K8);
                result = SzText_LoadFileEx(2, K8, name, (int)((long long)POS8 + 0xA75000), 0x1000, SEC8, 0x1000);
            }
        }
        break;
    case 11:
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[3]);
        if (text_merge_find(0, 3) == NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        SzText_Free(0, 3);
        SzText_Free(3, -1);
        result = SzText_LoadFileEx(0, 3, name, 0x912000, 0x11E800, 0, -1);
        break;
    case 12:
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[4]);
        if (text_merge_find(0, 4) == NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        SzText_Free(0, 4);
        SzText_Free(4, -1);
        result = SzText_LoadFileEx(0, 4, name, 0xA30800, 0x44800, 0, -1);
        break;
    case 13:
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[5]);
        if (text_merge_find(0, 5) == NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 0) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 1) != NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        SzText_Free(0, 0);
        SzText_Free(0, 1);
        SzText_Free(0, 5);
        SzText_Free(1, -1);
        SzText_Free(7, -1);
        result = SzText_LoadFileEx(0, 5, name, 0xB63540, 0x16A800, 0, -1);
        break;
    case 14:
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[6]);
        if (text_merge_find(0, 6) == NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 0) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 1) != NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        for (i = 0; i < 5; i++) {
            SzModel_Preset(i, -1, 999);
        }
        for (i = 0; i < 2; i++) {
            SzModel_Preset(i + 5, -1, 999);
        }
        for (i = 0; i < 1; i++) {
            SzModel_Preset(i + 7, -1, 999);
        }
        for (i = 0; i < 12; i++) {
            SzModel_Preset(i + 8, -1, 999);
        }
        for (i = 0; i < 2; i++) {
            SzModel_Preset(i + 0x20, -1, 999);
        }
        for (i = 0; i < 10; i++) {
            SzModel_Preset(i + 0x4A, -1, 999);
        }
        SzText_Free(0, 0);
        SzText_Free(0, 1);
        SzText_Free(0, 6);
        SzText_Free(1, -1);
        SzText_Free(8, -1);
        result = SzText_LoadFileEx(0, 6, name, 0xCCDD40, 0x47F000, 0, -1);
        break;
    case 15:
        off = anOfs_TEXETC[0];
        len = anOfs_TEXETC[1] - off;
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[7]);
        if (text_merge_find(0, 7) == NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 8) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 9) != NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        SzText_Free(0, 7);
        SzText_Free(0, 8);
        SzText_Free(0, 9);
        SzText_Free(9, -1);
        result = SzText_LoadFileEx(0, 7, name, 0xA75000, len, off, len);
        break;
    case 16:
        off = anOfs_TEXETC[1];
        len = anOfs_TEXETC[2] - off;
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[8]);
        if (text_merge_find(0, 8) == NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 7) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 9) != NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        SzText_Free(0, 8);
        SzText_Free(0, 7);
        SzText_Free(0, 9);
        SzText_Free(9, -1);
        result = SzText_LoadFileEx(0, 8, name, 0xA75000, len, off, len);
        break;
    case 17:
        off = anOfs_TEXETC[2];
        len = anOfs_TEXETC[3] - off;
        func_0019b648(name, data_002d0750, dir, gSzText_apchMerge[9]);
        if (text_merge_find(0, 9) == NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 7) != NULL) {
            arg1 = 1;
        }
        if (text_merge_find(0, 8) != NULL) {
            arg1 = 1;
        }
        if (arg1 == 0) {
            return 1;
        }
        SzText_Free(0, 9);
        SzText_Free(0, 7);
        SzText_Free(0, 8);
        SzText_Free(9, -1);
        result = SzText_LoadFileEx(0, 9, name, 0xA75000, len, off, len);
        break;
    }
    return result;
}
