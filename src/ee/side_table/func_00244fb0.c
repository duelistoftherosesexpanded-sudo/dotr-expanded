#include "dotr/ee/small_misc.h"

int videoDecCreate(struct DotrMiscPlayer *p, int a, int b, int c, unsigned d, int e, int f, int g)
{
    func_0018eb08(p, a, b);
    func_0018ef20(p, 0, (DotrMiscPlayerCallback)(void (*)(void))mpegError, 0);
    func_0018ef20(p, 1, mpegNodata, 0);
    func_0018ef20(p, 2, mpegStopDMA, 0);
    func_0018ef20(p, 3, mpegRestartDMA, 0);
    func_0018ef20(p, 5, (DotrMiscPlayerCallback)(void (*)(void))mpegTS, 0);
    func_002450f0_player(p);
    func_00243ee0_span(&p->unknown_48, c, d, e, f, g);
    return 1;
}
