# Credits

## kira-502/dotr-decomp

The C (or, for five VU0 routines, the assembly) of the 166 functions below, 394,912 bytes of game
code, comes from [kira-502/dotr-decomp](https://github.com/kira-502/dotr-decomp) (CC0-1.0), a
parallel decompilation of the same executable, as do two techniques this build relies on: compiling
some functions warm (after a fixed warm-up file) and original-file units
([DECOMPILATION.md](DECOMPILATION.md)). Each function was compiled here with this project's
toolchain and compares byte for byte with the original executable; names and types follow this
project. The C was taken from that repository at commits `1477fbca`, `6332a2e0`, `9253fbf6`. The
same project reported the other revision of `SLUS_205.15` noted in [FINDINGS.md](FINDINGS.md).

**L0nn: AI entry routines** ([ai-entries.md](modules/ai-entries.md)): `L032`, `L033`, `L034`, `L035`, `L036`.

**SzView: camera** ([camera.md](modules/camera.md)): `SzView_Act`, `SzView_SetFog_Pipe`, `SzView_DoRequest`, `SzView_ReqCamera`, `SzView_V2IV`, `SzView_ActCamera`.

**Taku/Tk: campaign map and save screens** ([campaign.md](modules/campaign.md)): `Taku_Prim_Set_SP1`, `Taku_Prim_Set_FT4`, `Taku_Prim_Set_GT4`, `Taku_Window_Set`, `Taku_Camp_Ivent`, `Taku_Camp_Ivent_Name`, `Taku_Camp_Ivent_Window`, `Taku_Camp_2D`, `Taku_Camp_BUN_HANTEI`, `Taku_Camp_Ivent_Ga`, `Taku_Camp_BUN_HYOUJI`, `Taku_Camp_TIKEI_Map_Hyouji`, `TkCamp_Act`, `TakuLoad_Name_Set`.

**Ta: duel rules and card effects** ([duel-rules.md](modules/duel-rules.md)): `Ta_Eff_009`, `Ta_Manage_ActEfc`, `Ta_Eff_109`.

**SzDuel: the duel** ([duel.md](modules/duel.md)): `SzDuel_DrawHandCard_Pipe`, `SzDuel_DrawDialog_Pipe`, `lSzDuel_DrawDialog_Window_Pipe`, `lSzDuel_DrawWindow_PlayerInfo_Pipe`, `lSzDuel_DrawWindow_CardInfo_Pipe`, `lSzDuel_DrawWindow_ExIcon_Pipe`, `lSzDuel_DrawWindow_Emblem_Pipe`, `lSzDuel_DrawWindow_Frame_Pipe`, `lSzDuel_DrawWindow_Base_Pipe`, `lSzDuel_DrawWindow_ExBoard_Pipe`, `SzDuel_DrawWindow_Font_Pipe`, `lSzDuel_DrawWindow_DualShock2_Pipe`, `lSzDuel_DrawBuildDeck_Bag_Pipe`, `lSzDuel_DrawBuildDeck_Deck_Pipe`, `lSzDuel_DrawBuildDeck_Card_Pipe`, `lSzDuel_DrawBuildDeck_Detail_Pipe`, `lSzDuel_DrawBuildDeck_Detail_Basic_Pipe`, `lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe`, `lSzDuel_DrawBuildDeck_Detail_Ability_Pipe`, `lSzDuel_DrawBuildDeck_Library_Pipe`, `lSzDuel_DrawBuildDeck_BagCursor_Pipe`, `lSzDuel_DrawBuildDeck_DeckCursor_Pipe`, `lSzDuel_DrawTrade_Bag_Pipe`, `lSzDuel_DrawTrade_Card_Pipe`, `lSzDuel_DrawTrade_Cursor_Pipe`, `lSzDuel_DrawTrade_Ready_Pipe`, `SzDuel_DrawCustomDuel_Pipe`, `lSzDuel_DrawCustomDuel_Title_Pipe`, `lSzDuel_DrawCustomDuel_PlayerInfo_Pipe`, `lSzDuel_DrawCustomDuel_Menu_Top_Pipe`, `lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe`, `lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe`, `lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe`, `lSzDuel_DrawCustomDuel_Menu_LeaderAbility_Pipe`, `lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe`, `lSzDuel_DrawCustomDuel_Menu_SelectMap_Pipe`, `lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe`, `lSzDuel_DrawMapEdit_Graph_Pipe`, `SzDuel_Act`, `lSzDuel_LockCamera2`, `SzDuel_SetupCard_Pipe`, `lSzDuel_OpenHandCursor_Pipe`.

**Sz: other engine modules** ([engine.md](modules/engine.md)): `lSzBattle_DrawStage_Pipe`, `lSzBattle_DrawSea_Pipe`, `lSzBattle_DrawNormal_Pipe`, `lSzBattle_DrawFakeMirror`, `lSzBattle_DrawFakeShadow`, `SzMsg_LoadFont_Pipe`, `SzText_LoadMerge`, `SzPad_Status`, `SzColor_HLS2RGB`, `SzColor_RGB2SEP`.

**SzGame: modes and menus** ([game-modes.md](modules/game-modes.md)): `SzGame_ActMenu`, `lSzGame_TEXTURE_TEST`, `lSzGame_DrawTitle_Pipe`.

**Main: start-up and main loop** ([main.md](modules/main.md)): `main`.

**Ex: 3D mathematics and primitives** ([math-prims.md](modules/math-prims.md)): `Ex3d_RotTransPersAutoCalcQ`, `Ex3d_InitClipPlane`, `ExSortPrim_Pipe`.

**SzModel: models** ([models.md](modules/models.md)): `SzModel_LoadMerge`, `lSzModel_TexAnimConv`, `SzModel_Act`, `SzModel_DrawWithFunc_Pipe`, `SzModel_SetAnimIntrReverseParam`.

**Other and unnamed** ([other.md](modules/other.md)): `ShFx_Spline`, `ShFx_Vib2P`, `LkVec2Rot`, `LkVec2RotXY`, `LkAssignUV`, `LkAssignUVST`, `LkAssignUVST3`, `RH2_DATA_IN2`, `PAINT_PIRM_G4`, `PAINT_PIRM_GT4`, `GT4_BOX_IN__2DMAP`, `R3D_RRR_XYZ`, `card_icon_tukuru`, `vblankHandler`, `handler_endimage`, `viBufGetTs`, `ShAnime_GetFixInfo`, `ShAnime_GetAnmDat`, `ShFace_Prim`, `Battle`, `SearchUnitByRange`, `ShSlot_Flear`, `ShSlot_Start`, `ShSlot_HitCardRequest`, `ShSlot_CheckLine`, `ShSlot_IsFlashStop`, `ShSlot_DrawParts`, `ShSlot_DrawEye`, `ShSlot_FlashBox`, `ShSlot_DrawReel`, `ShSlot_Draw_Pipe`, `ShSlot_Init`, `ShSlot_Act`.

**Rh: files, models, geometry** ([render-files.md](modules/render-files.md)): `RhMath_Cos`, `RhGeom_MulMatrix`, `RhGeom_MulMatrixRev`, `RhGeom_InvertMatrix`, `RhGeom_ScaleMatrix`, `RhGeom_ScaleRotXYZTransMatrix`, `RhGeom_MulMatrixVector`, `RhGeom_MulMatrixVectorArray`, `RhGeom_RotationInterpolate`, `RhGeom_RoationComplexInterpolate`, `RhGeomEx_SetMatrixForStraightLight`, `RhGeomEx_SetMatrixForPointLight`, `RhGeomEx_ScaleRotXYZTransMatrix`, `lRhModel_DrawNormalMethod`, `lRhModel_DrawFastMethod`, `lRhModel_SetupNoMotion`, `lRhModel_SetupMultiMotion`, `lRhModel_SetupFloatMotion`, `lRhModel_SetupPulseMotion`, `lRhModel_SetMaterialIntensity`, `lRhModelCpu_SetupGlobal`, `lRhModelCpu_TraceDynamicBlock`, `lRhModelCpu_CalcIndepStrip`, `lRhModelCpu_CalcIndepPoint`, `lRhModelCpu_CalcSharedStrip`, `lRhModelCpu_CalcSharedPoint`.

**TaLogic: opponent AI** ([talogic.md](modules/talogic.md)): `TaLogicCom_IsOpenUnit__00111320`, `TaLogic006_GetSummonPiece`, `TaLogic008_GetSummonPiece`, `TaLogic010_GetSummonPiece`, `TaLogic011_GetSummonPiece`, `TaLogic011_IsUseLifeHourGlass`, `TaLogic012_CommandLineUnit`, `TaLogic012_GetEmptyLine`, `TaLogic013_CommandInvadeUnit`, `TaLogic018_GetVirusMove`, `TaLogicCom_IsOpenUnit__0016b610`, `TaLogic022_CommandEfcMonster`, `TaLogic_GetRootTree2`, `TaLogic_GetInvadeRootTree2`, `TaLogic_GetLandChange`.

## Other contributors

- **batzpup**: names from his hand-made Ghidra project for `SLUS_205.15` (2019), used with
  permission. From it come the names of about 260 data objects that the Japanese build leaves
  unnamed, about 1,400 parameter and 400 local-variable names, about 260 structure field names
  (unit, model, map-cell, card, save-data and AI records), the value names in
  [GAME_DATA.md](GAME_DATA.md#enumerations), many of the comments naming cards, effects and
  modes on `case` labels, and the function name `useSoftFocus`. Where the Japanese build's own
  names exist, those are used instead.
- **Zacksly**: "Xbox Series Button Icons & Controls" (Zacksly Game Assets, CC BY), the letter
  shapes of the widescreen mod's Xbox button pictures (`mods/widescreen/art/xbox_buttons.png`,
  made into the game's texture by `tools/dlc_buttons.py`).
- Further references: [REFERENCES.md](REFERENCES.md); licences: [LICENSING.md](LICENSING.md).
