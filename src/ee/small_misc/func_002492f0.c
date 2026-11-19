#include "dotr/ee/small_misc.h"

int Ta_GetToonAgreement(int cardNo)
{
    return (data_0029d586[cardNo].unknown_bytes[0] >> 1) & 1;
}
