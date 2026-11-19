#include "dotr/ee/small_misc.h"

void RhZSort_Start(int vifPacketArea, int b)
{
    gRhZSort_zsl[0] = vifPacketArea;
    gRhZSort_zsl[1] = b;
    gRhZSort_zsl[3] = b;
    gRhZSort_zsl[2] = 1;
}
