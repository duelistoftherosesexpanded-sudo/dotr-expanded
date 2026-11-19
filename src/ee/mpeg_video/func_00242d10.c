#include "dotr/ee/mpeg_video.h"

int videoCallback(void *mp, struct DotrMvCbData *cbstr, void *data)
{
    /* MATCHING DEVICE: the cast local first, this declaration order, separate assignments. */
    struct DotrMvReadBuf *rb = (struct DotrMvReadBuf *)data;
    char *pd0;
    char *pd1;
    int d0;
    int d1;
    char *ps0;
    unsigned int s0;
    unsigned int diff;
    int s1;
    int len;
    (void)mp; /* unused; the statement only quiets the host compiler */
    ps0 = cbstr->data;
    s0 = cbstr->len;
    diff = rb->data + rb->size - ps0;
    s0 = (s0 < diff) ? s0 : diff;
    s1 = cbstr->len - s0;
    func_002450d0_mv(&data_00350680_mv, &pd0, &d0, &pd1, &d1);
    len = func_00242f20_mv((char *)(((DotrMvAddr)pd0 & 0x0fffffff) | 0x20000000), d0,
                           (char *)(((DotrMvAddr)pd1 & 0x0fffffff) | 0x20000000), d1,
                           ps0, s0, rb->data, s1);
    if (len > 0) {
        if (!func_00245160_mv(&data_00350680_mv, cbstr->pts, cbstr->dts, pd0, len)) {
            func_00242cf0_mv(data_0030f5e0_mv);
        }
    }
    func_002450e0_mv(&data_00350680_mv, len);
    return (len > 0) ? 1 : 0;
}
