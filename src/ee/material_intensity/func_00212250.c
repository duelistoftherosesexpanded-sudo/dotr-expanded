#include "dotr/ee/material_intensity.h"

/* lRhModel_SetMaterialIntensity (0x212250): walks one or two model packet lists and rewrites
 * the colours of their lighting packets from a material record.
 * Compiled with `#pragma optimization_level 2` and `#pragma schedule on` (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 2
#pragma schedule on
#endif
void lRhModel_SetMaterialIntensity(unsigned char *arg0, unsigned char *arg1, unsigned char *arg2, unsigned char *arg3, unsigned char *modelFileBaseAddr) {
    if (arg0 < arg1) {
    do {
        switch (*(unsigned int *)arg0 & 0xF0000000) {
        case 0x10000000:
            if ((*(long long *)(arg0 + 0) == 0x10000003) && (*(long long *)(arg0 + 8) == 0x6C03003C10000000ULL)) {
                if (((struct DotrMaterialIntensityFlags *)(modelFileBaseAddr + 4))->replace) {
                    *(float *)(arg0 + 0x10) = (float) *(float *)(modelFileBaseAddr + 0x3C);
                    *(float *)(arg0 + 0x14) = (float) *(float *)(modelFileBaseAddr + 0x40);
                    *(float *)(arg0 + 0x18) = (float) *(float *)(modelFileBaseAddr + 0x44);
                    if (*(float *)(arg0 + 0x20) != 0.0f) {
                        *(float *)(arg0 + 0x20) = (float) *(float *)(modelFileBaseAddr + 0x3C);
                        *(float *)(arg0 + 0x24) = (float) *(float *)(modelFileBaseAddr + 0x40);
                        *(float *)(arg0 + 0x28) = (float) *(float *)(modelFileBaseAddr + 0x44);
                    }
                    *(float *)(arg0 + 0x2C) = (float) *(float *)(modelFileBaseAddr + 0x48);
                    *(float *)(arg0 + 0x30) = (float) *(float *)(modelFileBaseAddr + 0x3C);
                    *(float *)(arg0 + 0x34) = (float) *(float *)(modelFileBaseAddr + 0x40);
                    *(float *)(arg0 + 0x38) = (float) *(float *)(modelFileBaseAddr + 0x44);
                }
                if ((*(unsigned char *)(modelFileBaseAddr + 5) & 1)) {
                    *(float *)(arg0 + 0x10) += *(float *)(modelFileBaseAddr + 0x3C);
                    *(float *)(arg0 + 0x14) += *(float *)(modelFileBaseAddr + 0x40);
                    *(float *)(arg0 + 0x18) += *(float *)(modelFileBaseAddr + 0x44);
                    if (*(float *)(arg0 + 0x20) != 0.0f) {
                        *(float *)(arg0 + 0x20) += *(float *)(modelFileBaseAddr + 0x3C);
                        *(float *)(arg0 + 0x24) += *(float *)(modelFileBaseAddr + 0x40);
                        *(float *)(arg0 + 0x28) += *(float *)(modelFileBaseAddr + 0x44);
                    }
                    *(float *)(arg0 + 0x2C) += *(float *)(modelFileBaseAddr + 0x48);
                    *(float *)(arg0 + 0x30) += *(float *)(modelFileBaseAddr + 0x3C);
                    *(float *)(arg0 + 0x34) += *(float *)(modelFileBaseAddr + 0x40);
                    *(float *)(arg0 + 0x38) += *(float *)(modelFileBaseAddr + 0x44);
                    if (*(float *)(arg0 + 0x10) < 0.0f) {
                        *(float *)(arg0 + 0x10) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x14) < 0.0f) {
                        *(float *)(arg0 + 0x14) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x18) < 0.0f) {
                        *(float *)(arg0 + 0x18) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x20) < 0.0f) {
                        *(float *)(arg0 + 0x20) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x24) < 0.0f) {
                        *(float *)(arg0 + 0x24) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x28) < 0.0f) {
                        *(float *)(arg0 + 0x28) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x2C) < 0.0f) {
                        *(float *)(arg0 + 0x2C) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x30) < 0.0f) {
                        *(float *)(arg0 + 0x30) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x34) < 0.0f) {
                        *(float *)(arg0 + 0x34) = 0.0f;
                    }
                    if (*(float *)(arg0 + 0x38) < 0.0f) {
                        *(float *)(arg0 + 0x38) = 0.0f;
                    }
                    if (!(*(float *)(arg0 + 0x10) <= 255.0f)) {
                        *(float *)(arg0 + 0x10) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x14) <= 255.0f)) {
                        *(float *)(arg0 + 0x14) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x18) <= 255.0f)) {
                        *(float *)(arg0 + 0x18) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x20) <= 255.0f)) {
                        *(float *)(arg0 + 0x20) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x24) <= 255.0f)) {
                        *(float *)(arg0 + 0x24) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x28) <= 255.0f)) {
                        *(float *)(arg0 + 0x28) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x2C) <= 255.0f)) {
                        *(float *)(arg0 + 0x2C) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x30) <= 255.0f)) {
                        *(float *)(arg0 + 0x30) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x34) <= 255.0f)) {
                        *(float *)(arg0 + 0x34) = 255.0f;
                    }
                    if (!(*(float *)(arg0 + 0x38) <= 255.0f)) {
                        *(float *)(arg0 + 0x38) = 255.0f;
                    }
                }
                if ((((struct DotrMaterialIntensityFlags *)(modelFileBaseAddr + 4))->scale) && (*(float *)(arg0 + 0x20) != 0.0f)) {
                    *(float *)(arg0 + 0x10) = (float) (*(float *)(arg0 + 0x10) * *(float *)(modelFileBaseAddr + 0x34));
                    *(float *)(arg0 + 0x14) = (float) (*(float *)(arg0 + 0x14) * *(float *)(modelFileBaseAddr + 0x34));
                    *(float *)(arg0 + 0x18) = (float) (*(float *)(arg0 + 0x18) * *(float *)(modelFileBaseAddr + 0x34));
                    *(float *)(arg0 + 0x20) = (float) (*(float *)(arg0 + 0x20) * *(float *)(modelFileBaseAddr + 0x30));
                    *(float *)(arg0 + 0x24) = (float) (*(float *)(arg0 + 0x24) * *(float *)(modelFileBaseAddr + 0x30));
                    *(float *)(arg0 + 0x28) = (float) (*(float *)(arg0 + 0x28) * *(float *)(modelFileBaseAddr + 0x30));
                    *(float *)(arg0 + 0x30) = (float) (*(float *)(arg0 + 0x30) * *(float *)(modelFileBaseAddr + 0x38));
                    *(float *)(arg0 + 0x34) = (float) (*(float *)(arg0 + 0x34) * *(float *)(modelFileBaseAddr + 0x38));
                    *(float *)(arg0 + 0x38) = (float) (*(float *)(arg0 + 0x38) * *(float *)(modelFileBaseAddr + 0x38));
                }
            }
            arg0 = arg0 + *(unsigned short *)arg0 * 0x10 + 0x10;
            break;
        case 0x20000000:
            arg0 = *(unsigned char **)(arg0 + 4);
            break;
        case 0x30000000:
            arg0 += 0x10;
            break;
        default:
            return;
        }
    } while (arg0 < arg1);
    }
    if (arg2 != 0) {
        if (arg2 < arg3) {
        do {
            switch (*(unsigned int *)arg2 & 0xF0000000) {
            case 0x10000000:
                if ((*(long long *)(arg2 + 0) == 0x10000003) && (*(long long *)(arg2 + 8) == 0x6C03003C10000000ULL)) {
                    if (((struct DotrMaterialIntensityFlags *)(modelFileBaseAddr + 4))->replace) {
                        *(float *)(arg2 + 0x10) = (float) *(float *)(modelFileBaseAddr + 0x3C);
                        *(float *)(arg2 + 0x14) = (float) *(float *)(modelFileBaseAddr + 0x40);
                        *(float *)(arg2 + 0x18) = (float) *(float *)(modelFileBaseAddr + 0x44);
                        if (*(float *)(arg2 + 0x20) != 0.0f) {
                            *(float *)(arg2 + 0x20) = (float) *(float *)(modelFileBaseAddr + 0x3C);
                            *(float *)(arg2 + 0x24) = (float) *(float *)(modelFileBaseAddr + 0x40);
                            *(float *)(arg2 + 0x28) = (float) *(float *)(modelFileBaseAddr + 0x44);
                        }
                        *(float *)(arg2 + 0x2C) = (float) *(float *)(modelFileBaseAddr + 0x48);
                        *(float *)(arg2 + 0x30) = (float) *(float *)(modelFileBaseAddr + 0x3C);
                        *(float *)(arg2 + 0x34) = (float) *(float *)(modelFileBaseAddr + 0x40);
                        *(float *)(arg2 + 0x38) = (float) *(float *)(modelFileBaseAddr + 0x44);
                    }
                    if ((*(unsigned char *)(modelFileBaseAddr + 5) & 1)) {
                        *(float *)(arg2 + 0x10) += *(float *)(modelFileBaseAddr + 0x3C);
                        *(float *)(arg2 + 0x14) += *(float *)(modelFileBaseAddr + 0x40);
                        *(float *)(arg2 + 0x18) += *(float *)(modelFileBaseAddr + 0x44);
                        if (*(float *)(arg2 + 0x20) != 0.0f) {
                            *(float *)(arg2 + 0x20) += *(float *)(modelFileBaseAddr + 0x3C);
                            *(float *)(arg2 + 0x24) += *(float *)(modelFileBaseAddr + 0x40);
                            *(float *)(arg2 + 0x28) += *(float *)(modelFileBaseAddr + 0x44);
                        }
                        *(float *)(arg2 + 0x2C) += *(float *)(modelFileBaseAddr + 0x48);
                        *(float *)(arg2 + 0x30) += *(float *)(modelFileBaseAddr + 0x3C);
                        *(float *)(arg2 + 0x34) += *(float *)(modelFileBaseAddr + 0x40);
                        *(float *)(arg2 + 0x38) += *(float *)(modelFileBaseAddr + 0x44);
                        if (*(float *)(arg2 + 0x10) < 0.0f) {
                            *(float *)(arg2 + 0x10) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x14) < 0.0f) {
                            *(float *)(arg2 + 0x14) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x18) < 0.0f) {
                            *(float *)(arg2 + 0x18) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x20) < 0.0f) {
                            *(float *)(arg2 + 0x20) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x24) < 0.0f) {
                            *(float *)(arg2 + 0x24) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x28) < 0.0f) {
                            *(float *)(arg2 + 0x28) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x2C) < 0.0f) {
                            *(float *)(arg2 + 0x2C) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x30) < 0.0f) {
                            *(float *)(arg2 + 0x30) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x34) < 0.0f) {
                            *(float *)(arg2 + 0x34) = 0.0f;
                        }
                        if (*(float *)(arg2 + 0x38) < 0.0f) {
                            *(float *)(arg2 + 0x38) = 0.0f;
                        }
                        if (!(*(float *)(arg2 + 0x10) <= 255.0f)) {
                            *(float *)(arg2 + 0x10) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x14) <= 255.0f)) {
                            *(float *)(arg2 + 0x14) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x18) <= 255.0f)) {
                            *(float *)(arg2 + 0x18) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x20) <= 255.0f)) {
                            *(float *)(arg2 + 0x20) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x24) <= 255.0f)) {
                            *(float *)(arg2 + 0x24) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x28) <= 255.0f)) {
                            *(float *)(arg2 + 0x28) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x2C) <= 255.0f)) {
                            *(float *)(arg2 + 0x2C) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x30) <= 255.0f)) {
                            *(float *)(arg2 + 0x30) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x34) <= 255.0f)) {
                            *(float *)(arg2 + 0x34) = 255.0f;
                        }
                        if (!(*(float *)(arg2 + 0x38) <= 255.0f)) {
                            *(float *)(arg2 + 0x38) = 255.0f;
                        }
                    }
                    if ((((struct DotrMaterialIntensityFlags *)(modelFileBaseAddr + 4))->scale) && (*(float *)(arg2 + 0x20) != 0.0f)) {
                        *(float *)(arg2 + 0x10) = (float) (*(float *)(arg2 + 0x10) * *(float *)(modelFileBaseAddr + 0x34));
                        *(float *)(arg2 + 0x14) = (float) (*(float *)(arg2 + 0x14) * *(float *)(modelFileBaseAddr + 0x34));
                        *(float *)(arg2 + 0x18) = (float) (*(float *)(arg2 + 0x18) * *(float *)(modelFileBaseAddr + 0x34));
                        *(float *)(arg2 + 0x20) = (float) (*(float *)(arg2 + 0x20) * *(float *)(modelFileBaseAddr + 0x30));
                        *(float *)(arg2 + 0x24) = (float) (*(float *)(arg2 + 0x24) * *(float *)(modelFileBaseAddr + 0x30));
                        *(float *)(arg2 + 0x28) = (float) (*(float *)(arg2 + 0x28) * *(float *)(modelFileBaseAddr + 0x30));
                        *(float *)(arg2 + 0x30) = (float) (*(float *)(arg2 + 0x30) * *(float *)(modelFileBaseAddr + 0x38));
                        *(float *)(arg2 + 0x34) = (float) (*(float *)(arg2 + 0x34) * *(float *)(modelFileBaseAddr + 0x38));
                        *(float *)(arg2 + 0x38) = (float) (*(float *)(arg2 + 0x38) * *(float *)(modelFileBaseAddr + 0x38));
                    }
                }
                arg2 = arg2 + *(unsigned short *)arg2 * 0x10 + 0x10;
                break;
            case 0x20000000:
                arg2 = *(unsigned char **)(arg2 + 4);
                break;
            case 0x30000000:
                arg2 += 0x10;
                break;
            default:
                return;
            }
        } while (arg2 < arg3);
        }
    }
}
#ifdef __MWERKS__
#pragma schedule reset
#pragma optimization_level reset
#endif
