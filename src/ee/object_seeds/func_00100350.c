#include "dotr/ee/object_seeds.h"

int lMain_InitModel2(void)
{
    int i;
    int side;
    int cardID;

    for (i = 0; i < 84; i++) {
        if (SzModel_Create(i) == 0) {
            return 0;
        }
        SzModel_SetPos(i, &gMain_ModelData[i].vector);
        SzModel_SetRot(i, &gMain_ModelData[i].words);
        SzModel_Preset(i, gMain_ModelData[i].kind, gMain_ModelData[i].value);
        if (gMain_ModelData[i].kind == 5) {
            cardID = SzDuel_Xax2New(gMain_ModelData[i].value);
            SzDuel_AddUnit(i - 30, gMain_ModelData[i].unknown_08, gMain_ModelData[i].unknown_0c, cardID, 1);
            SzDuel_SetUnitStatus(-1, i, 4, 1);
            SzDuel_SetUnitOpenRate(-1, i, 0, 255);
            SzDuel_SetUnitCtlRateM(-1, i, 1, 0);
        } else if (gMain_ModelData[i].kind == 4) {
            side = (i - 20 < 5) ? 0 : 1;
            cardID = SzDuel_Xax2New(gMain_ModelData[i].value);
            SzDuel_AddUnit(side, gMain_ModelData[i].unknown_08, gMain_ModelData[i].unknown_0c, cardID, 0);
            SzDuel_SetUnitStatus(-1, i, 4, 1);
            SzDuel_SetUnitOpenRate(-1, i, 0, 255);
            SzDuel_SetUnitCtlRateM(-1, i, 1, 0);
        }
    }
    return 1;
}
