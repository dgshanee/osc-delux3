#include "WaveData.h"

WaveData *createWaveData(){
  WaveData *res = (WaveData *)malloc(sizeof(WaveData));

  ma_waveform *wave = (ma_waveform*)malloc(sizeof(ma_waveform));
  ma_waveform_config waveConfig = ma_waveform_config_init(ma_format_f32, 2, 48000, ma_waveform_type_sine, 0.2f, 220);

  if(ma_waveform_init(&waveConfig, wave) != MA_SUCCESS){
   printf("failed to setup waveform\n");
  }

  res->frequency = 220;
  res->amplitude = 0.2f;
  res->sineWave = wave;

  return res;
}

void destroyWaveData(WaveData *w){
  ma_waveform_uninit(w->sineWave);
  free(w->sineWave);
  free(w->buffer);
  w->sineWave = NULL;
  w->buffer = NULL;
  free(w);
  w = NULL;
}
