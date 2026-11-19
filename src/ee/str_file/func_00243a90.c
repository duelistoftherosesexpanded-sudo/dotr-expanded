#include "dotr/ee/str_file.h"

int strFileOpen(struct DotrStrFile *file, char *name)
{
    char path[256];
    char dev[76];
    char mode[4];
    char *colon;
    int n;
    int i;
    int len;
    char *c;

    colon = func_001976d8(name, ':');
    if (colon != 0) {
        n = colon - name;
        func_0019c2b8(dev, name, n);
        dev[n] = 0;
        if (func_0019bc90(dev, data_0030f7f0) == 0) {
            len = func_0019bef0(colon + 1);
            file->by_sector = 1;
            for (i = 0; i < len; i++) {
                c = colon + i + 1;
                if (*c == '/') {
                    *c = '\\';
                }
                *c = func_0019d9e0(*c);
            }
            func_0019b648(path, data_0030f7fb, colon + 1,
                          func_001976d8(name, ';') ? data_0030f7f7 : data_0030f7f8);
        } else {
            file->by_sector = 0;
            func_0019b648(path, data_0030f800, dev, colon + 1);
        }
    } else {
        func_0019bdd8(dev, data_0030f806);
        file->by_sector = 0;
        func_0019b648(path, data_0030f800, dev, name);
    }
    func_00199b50(data_0030f80c, path);
    if (file->by_sector != 0) {
        while (func_00191a58(0) != 2) {
        }
        file->cd_buffer = func_001828c0(0x28010);
        func_00191f90(80, 5, ((unsigned int)(file->cd_buffer + 15) >> 4) << 4);
        if (func_00190ff0(&file->cd_position, path) == 0) {
            func_00199b50(data_0030f816, path);
            return 0;
        }
        file->size = file->cd_size;
        mode[0] = 0;
        mode[1] = 0;
        mode[2] = 0;
        func_00191fc0(file->cd_position, mode);
    } else {
        file->descriptor = func_0017fa98(path, 1);
        if (file->descriptor < 0) {
            func_00199b50(data_0030f839, path);
            return 0;
        }
        file->size = func_0017fea0(file->descriptor, 0, 2);
        if (file->size < 0) {
            func_00199b50(data_0030f854, path, file->size);
            func_0017fd20(file->descriptor);
            return 0;
        }
        if (func_0017fea0(file->descriptor, 0, 0) < 0) {
            func_00199b50(data_0030f86f, path);
            func_0017fd20(file->descriptor);
            return 0;
        }
    }
    return 1;
}


