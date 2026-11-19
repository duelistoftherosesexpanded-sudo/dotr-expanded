#include "dotr/ee/sound_lifecycle.h"
#define m (g_SDValue_lifecycle[0])
void SD_InitializeDevice(void){
 if(!m || !g_SDBuffer[0])return;
 if(!(m->unknown_0000&1)){
  SDRPC_Init();
  SDRPC_SendCommand(0xf005);
  if(!(m->unknown_0000&0x20)){
   SDRPC_SendCommand(0xf002,0xc1800);
   m->unknown_0014=m->unknown_0180;
   m->unknown_0000|=0x20;
  }
  SDRPC_SendCommand(0x5f10,0,0x5010);
  SDRPC_SendCommand(0x5f10,1,0x9f810);
  SDRPC_SendCommand(0x5f10,2,0x199810);
  SDRPC_SendCommand(0x5f10,3,0x1cb810);
  SDRPC_SendCommand(0x5f10,4,0x1fd810);
  SDRPC_SendCommand(0x5f10,5,0x8f810);
  SDRPC_SendCommand(0x5f10,6,0x9f810);
  SDRPC_SendCommand(0x5f10,7,0xaf810);
  SDRPC_SendCommand(0x5f10,8,0xbf810);
  SDRPC_SendCommand(0x5f10,9,0xd7810);
  SDRPC_SendCommand(0x5f10,10,0xef810);
  SDRPC_SendCommand(0x5f10,11,0x107810);
  SDRPC_SendCommand(0x5f12,0x150000,0x38000);
  SD_vInitializeFileHandler();SD_vInitSDThread();SD_vInitializeAutoDMA();
  m->unknown_0000|=1;m->unknown_0004|=14;m->unknown_2a6c=0;
 }
 m->unknown_0000|=0x80;
}
