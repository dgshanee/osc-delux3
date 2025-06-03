#ifndef WAVEDATA_H
#define WAVEDATA_H

#include "../../include/miniaudio.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct{
  ma_waveform* sineWave;
  float *buffer;
  float frequency;
  float amplitude;
} WaveData;

WaveData *createWaveData();

void destroyWaveData(WaveData*);

#endif
