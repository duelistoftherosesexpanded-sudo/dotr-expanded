#include "dotr/ee/resource_boot.h"

int RhFile_LoadIopModuleWrapper(char *name, int argument)
{
    return RhFile_LoadIopModuleWrapperEx(name, 0, 0, argument);
}
