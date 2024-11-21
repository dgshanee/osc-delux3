#ifndef UTILS_H
#define UTILS_H

#include "../include/miniaudio.h"

#include "../include/gui_curve_editor.h"

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

typedef struct{
  WaveData note;
  DeviceUtil device;
}Note;

void LoadCurveDefaults(GuiCurveEditorState[]);

#endif

