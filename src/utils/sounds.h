#ifndef SOUNDS_H
#define SOUNDS_H

#include "utils.h"
#include "WaveData.h"
#include "../../include/miniaudio.h"
#include <stdlib.h>
#include <stdio.h>

#define DEVICE_FORMAT ma_format_f32
#define DEVICE_CHANNELS 2
#define DEVICE_SAMPLE_RATE 48000

void data_callback(ma_device*, void*, const void*, ma_uint32);

// DeviceUtil *generateDevice();

ma_device *generateDevices();

void makeWaveForm(ma_device *device, ma_waveform_type waveformType, ma_waveform *wave);

void initializeDevice(ma_device *device, WaveData *userData);

// void initializeDeviceUtil(ma_device *device, DeviceUtil *device_util, WaveData *userData);

void initializeWaveData(WaveData *pUserData);

#endif
