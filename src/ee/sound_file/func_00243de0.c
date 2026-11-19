#include "dotr/ee/sound_file.h"

/* Reads byte_count bytes into buffer. Without the sector path it forwards to
 * func_001800d8 with the record's descriptor. Otherwise it asks func_00192090
 * for byte_count / 2048 sectors until that reports no error, and after each
 * error waits for func_00191a58(0) == 2 and a non-zero func_00191fc0 before
 * trying again; the 0x350620 stream is paused and resumed around the wait
 * when the word at 0x29b2f0 is set. */
int strFileRead(struct DotrSoundReader *reader, int buffer, int byte_count, int position)
{
    struct DotrSoundReadMode mode;
    int error;
    int sectors;
    int done;

    if (reader->by_sector != 0) {
        mode = data_0029b2f8;
        sectors = byte_count >> 11;
        for (;;) {
            func_0017bb60(0);
            done = func_00192090(sectors, buffer, 1, &error);
            if (error == 0) {
                break;
            }
            if (isWithAudio != 0) {
                audioDecPause(&data_00350620_sound);
            }
            func_00192058();
            do {
                while (func_00191a58(0) != 2) {
                }
            } while (func_00191fc0(position, &mode) == 0);
            if (isWithAudio != 0) {
                func_00243230_sound(&data_00350620_sound);
            }
        }
        return done << 11;
    }
    return func_001800d8(reader->descriptor, buffer, byte_count);
}
