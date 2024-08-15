
#define MINIAUDIO_IMPLEMENTATION
#import "../include/miniaudio.h"

typedef struct{
  ma_waveform* sineWave;
  float* buffer;
  float frequency;
  float amplitude;
} WaveData;

typedef struct{
  ma_device* device;
  ma_device_config* deviceConfig;  
} DeviceUtil;

void initializePUserData(WaveData *pUserData){
  float buffer;
  float frequency = 220;
  float amplitude = 0.2f;
  pUserData->buffer = &buffer;
  pUserData->frequency = frequency;
  pUserData->amplitude = amplitude;
}
