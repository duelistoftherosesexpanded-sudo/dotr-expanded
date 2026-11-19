#include "dotr/ee/resource_boot.h"

int RhFile_LoadIopModuleWrapperEx(char *name, int argument_size, char *arguments, int flag)
{
    char *path = (char *)dotr_ee_resource_boot_state_0031c2f0.unknown_000;
    lRhFile_StrUpper(name);
    /* Both arms are identical in the original instructions. */
    if (flag) {
        func_0019bdd8(path, data_002d0670);
    } else {
        func_0019bdd8(path, data_002d0670);
    }
    func_0019b9d0(path, name);
    func_0019b9d0(path, data_002d067d);
    while (func_00183538(path, argument_size, arguments) < 0) {
    }
    return 1;
}
