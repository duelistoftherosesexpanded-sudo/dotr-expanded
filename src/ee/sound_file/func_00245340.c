#include "dotr/ee/sound_file.h"

/* decBs0: decodes pictures until the stream ends (result 1) or the decoder's
 * state word becomes 1 (result -1, after a message). Each picture waits for a
 * free item of the queue at 0x350600; while the frame count is still zero the
 * image tags of every item are rebuilt for the decoder's width and height. */
int decBs0(struct DotrSoundVideoDec *dec)
{
    int ret;
    DotrSoundAddress data;
    int i;
    int w;
    int h;

    ret = 1;
    while (func_0018ee58(dec) == 0) {
        if (dec->state == 1) {
            ret = -1;
            func_00199b50(data_0030f8b0);
            break;
        }
        while ((data = voBufGetData(&data_00350600_sound)) == 0) {
            switchThread();
        }
        if (func_0018ed40(dec, data, 0x654) < 0) {
            ErrMessage((DotrSoundAddress)data_0030f8c8);
        }
        if (dec->frame_count == 0) {
            w = dec->width;
            h = dec->height;
            for (i = 0; i < data_00350600_sound.capacity; i++) {
                setImageTag(data_00350600_sound.items[i].tags[0], data_00350600_sound.images[i], 0, w, h);
                setImageTag(data_00350600_sound.items[i].tags[1], data_00350600_sound.images[i], 1, w, h);
            }
        }
        voBufIncCount(&data_00350600_sound);
        switchThread();
    }
    func_0018ee78(dec);
    return ret;
}
