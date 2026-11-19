#include "dotr/ee/object_table.h"

int SzModel_Load(int modelId, int modelType, int modelNo, int packed)
{
    struct DotrObject *pModel;
    char *dir;
    char *env;
    int is_pack;
    int is_one;
    int id;
    int hi;
    int lo;
    int start;
    int size;
    char format[128];
    char name[128];

    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (!pModel) {
        return 0;
    }
    if (pModel->drawFlag.unknown_bit_3) {
        while (pModel->drawFlag.unknown_bit_3) {
            SzFile_DoRequestS();
        }
    }
    pModel->modelId = -1;
    if (modelType >= 0) {
        pModel->type = modelType;
    }
    if (modelNo >= 0) {
        pModel->modelNo = modelNo;
    } else {
        modelNo = pModel->modelNo;
    }
    pModel->drawFlag.unknown_bit_2 = 0;
    if (modelNo < 0 || modelNo >= gSzModel_awIgnore[pModel->type]) {
        return 0;
    }
    dir = gSzModel_apchBase[pModel->type];
    if (packed && gSzModel_fPacking[0]) {
        if (func_00197548(data_002cf9d5)) {
            env = SzFile_GetEnv(data_002cf9f0);
            is_pack = 0;
            is_one = 0;
            if (pModel->type != 4 && pModel->type != 5 && pModel->type != 6) {
            } else {
                is_pack = 1;
            }
            if (pModel->type == 1) {
                is_one = 1;
            }
            if (is_pack) {
                if (modelNo == 737) {
                    id = 671;
                } else {
                    id = SzDuel_Xax2New(modelNo);
                }
                if (id < 683) {
                    int n = pModel->type - 4;
                    struct DotrObjectPackTriple base = data_00292f20;
                    struct DotrObjectPackTriple addr = data_00292f30;

                    /* data_002cf9f5: the Monster.mrg pack path format. */
                    func_0019b648(name, data_002cf9f5, env);
                    if (lSzModel_LoadModelPack(modelId, name, addr.v[n] + ((modelId - base.v[n]) << 20), 0x100000, id << 20, 0x100000)) {
                        return 1;
                    }
                }
                return 0;
            }
            if (is_one) {
                hi = modelNo / 100;
                lo = modelNo % 100;
                if (hi >= 0 && hi < 2 && lo >= 0 && lo < 21) {
                    id = lo + (hi ? 60 : 39);
                    start = anOfs_MODEL[id];
                    size = anOfs_MODEL[id + 1] - start;
                    func_0019b648(name, data_002cf9dd, env);
                    /* The code multiplies in 64 bits here; hence the long. */
                    if (lSzModel_LoadModelPack(modelId, name, (int)((long)hi * 0xa000 + 0x10aa800), size, start, size)) {
                        return 1;
                    }
                }
                return 0;
            }
        }
        env = SzFile_GetEnv(data_002cfa17);
        func_0019b648(format, data_002cfa0a, env, dir, dir);
        func_0019b648(name, format, modelNo, modelNo);
        if (!lSzModel_LoadModelPack(modelId, name, 0, -1, 0, -1)) {
            return 0;
        }
    } else {
        env = SzFile_GetEnv(data_002cfa17);
        func_0019b648(format, data_002cfa1d, env, dir, dir);
        func_0019b648(name, format, modelNo, modelNo);
        if (lSzModel_LoadModel(modelId, name) == 0) {
            return 0;
        }
    }
    return 1;
}
