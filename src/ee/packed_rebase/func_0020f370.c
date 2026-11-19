#include "dotr/ee/packed_rebase.h"

int RhModel_BindAddress(DotrRebaseHeader *header)
{
    DotrRebaseSegment *segment = (DotrRebaseSegment *)((char *)header + 16);
    int count;
    DotrRebaseWord *list;
    DotrRebaseWord *target;
    DotrRebaseWord address;
    DotrRebaseWord entry, ptrValue;

    list = (DotrRebaseWord *)((char *)header + header->list);
    for (count = 0; (entry = *list) != (DotrRebaseWord)-1; count++) {
        switch (entry & 0xf0000000) {
        case 0x20000000:
            target = (DotrRebaseWord *)((char *)header + segment[1].offset + (entry & 0x0fffffff));
            break;
        case 0x30000000:
            target = (DotrRebaseWord *)((char *)header + segment[2].offset + (entry & 0x0fffffff));
            break;
        case 0x40000000:
            target = (DotrRebaseWord *)((char *)header + segment[3].offset + (entry & 0x0fffffff));
            break;
        case 0x50000000:
            target = (DotrRebaseWord *)((char *)header + segment[4].offset + (entry & 0x0fffffff));
            break;
        default:
            goto skip;
        }
        ptrValue = *target;
        switch (ptrValue & 0xf0000000) {
        case 0x20000000:
            address = (DotrRebaseWord)((char *)header + segment[1].offset + (ptrValue & 0x0fffffff));
            break;
        case 0x30000000:
            address = (DotrRebaseWord)((char *)header + segment[2].offset + (ptrValue & 0x0fffffff));
            break;
        case 0x40000000:
            address = (DotrRebaseWord)((char *)header + segment[3].offset + (ptrValue & 0x0fffffff));
            break;
        case 0x50000000:
            address = (DotrRebaseWord)((char *)header + segment[4].offset + (ptrValue & 0x0fffffff));
            break;
        default:
            goto skip;
        }
        *target = address;
    skip:
        list++;
    }
    return 1;
}
