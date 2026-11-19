#include "dotr/ee/boot_sequence.h"

int *Main_Term(void)
{
    int index;

    SzView_Destroy(0);
    index = 0;
    do {
        SzModel_Destroy(index);
        index = index + 1;
    } while (index < 0x54);
    RhBase_Term();
    return SzPad_Term();
}
