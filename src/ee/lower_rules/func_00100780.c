#include "dotr/ee/lower_rules.h"

void lMain_LoadModel(void)
{
    int i;

    for (i = 0; i < 84; i++) {
        int kind = SzModel_GetModelType(i);

        if (SzModel_IsModelReady(i) != 0) {
            if ((unsigned)(kind - 4) < 2 && func_001df540_word(-1, i) == 0) {
                continue;
            }
            SzModel_Load(i, -1, -1, 1);
        }
    }
}
