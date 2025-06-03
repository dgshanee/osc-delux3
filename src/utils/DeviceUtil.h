#ifndef DEVICE_UTIL_H
#define DEVICE_UTIL_H

#include <stdlib.h>
#include "../../include/miniaudio.h"

#include "WaveData.h"
#include "sounds.h"

#define DEV_FORMAT ma_format_f32
#define DEV_CHANNELS 2
#define DEV_SAMPLE_RATE 48000

typedef struct{
  ma_device* device;
  ma_device_config* deviceConfig;  
} DeviceUtil;

DeviceUtil *createDeviceUtil(WaveData*);

void destroyDeviceUtil(DeviceUtil*);

#endif
