#include "dotr/ee/frame_setup.h"

DotrSetupWord SzDuel_StartCardSession_Pipe(DotrSetupWord cursor)
{
    return SzDuel_StartCardSessionWithPalette_Pipe(cursor, 2);
}
