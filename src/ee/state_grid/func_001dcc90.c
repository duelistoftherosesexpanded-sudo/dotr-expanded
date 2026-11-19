#include "dotr/ee/state_grid.h"

void SzDuel_InitMapInfo(struct DotrStateCell *mapInfo)
{
    mapInfo->unit = 0;
    mapInfo->unknown_04[0].unknown_2 = 0;
    mapInfo->unknown_04[0].unknown_1 = 0;
    mapInfo->unknown_04[0].unknown_0 = 0;
    mapInfo->unknown_04[0].unknown_3 = 0;
    mapInfo->unknown_04[1].unknown_2 = 0;
    mapInfo->unknown_04[1].unknown_1 = 0;
    mapInfo->unknown_04[1].unknown_0 = 0;
    mapInfo->unknown_04[1].unknown_3 = 0;
    mapInfo->unknown_04[2].unknown_2 = 0;
    mapInfo->unknown_04[2].unknown_1 = 0;
    mapInfo->unknown_04[2].unknown_0 = 0;
    mapInfo->unknown_04[2].unknown_3 = 0;
    mapInfo->unknown_04[3].unknown_2 = 0;
    mapInfo->unknown_04[3].unknown_1 = 0;
    mapInfo->unknown_04[3].unknown_0 = 0;
    mapInfo->unknown_04[3].unknown_3 = 0;
    mapInfo->field = 0;
    mapInfo->mapMovable = 0;
    mapInfo->targetSelectionHighlight = 0;
    mapInfo->selectionHighlightType = 0;
    mapInfo->mapAlpha = 128;
    mapInfo->fieldRate = 0;
}
