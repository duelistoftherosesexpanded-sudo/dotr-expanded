#include "dotr/ee/resource_boot.h"

int RhFile_Init(void)
{
    struct DotrResourceBootState *state = &dotr_ee_resource_boot_state_0031c2f0;
    unsigned int i;
    struct DotrResourceBootModule *module;
    struct DotrResourceBootEntry *entry;
    struct DotrResourceBootFile file;

    func_0017e240(0);
    func_00191778(0);
    while (!func_00183a38(data_002d0657)) {
    }
    while (!func_00183a00()) {
    }
    func_0017e240(0);
    func_00182838();
    func_00182cc8();
    func_00191778(0);
    func_00191c50(2);
    func_0017fa60();
    while (func_00191a58(0) != 2) {
    }
    module = gSzFile_aIopList;
    for (i = 0; module->name; i++) {
        if (!RhFile_LoadIopModuleWrapper(module->name, module->argument)) {
            return 0;
        }
        module++;
    }
    if (func_00197548(data_002d0638)) {
        entry = mapFile;
        for (i = 0; i < 97; i++) {
            while (func_00191a58(0) != 2) {
            }
            if (func_00190ff0(&file, entry->name)) {
                entry->first_sector = file.first_sector;
                entry->sector_count = file.byte_size >> 11;
            } else {
                entry->name = data_002d066f;
            }
            entry++;
        }
    }
    func_00190c48(0, state->buffer_080, 0x4000);
    func_00190a90(lRhFile_CdCallback);
    return 1;
}
