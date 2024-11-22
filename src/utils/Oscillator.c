#include "Oscillator.h"
#include "DeviceUtil.h"
#include "WaveData.h"

Oscillator *createOscillator(){
  Oscillator *res = (Oscillator*) malloc(sizeof(Oscillator));
  for(int i = 0; i < 8; i++){
    WaveData *wav = createWaveData();
    DeviceUtil *devUtil = createDeviceUtil(wav);

    res->notes[i] = (Note *)malloc(sizeof(Note));
    res->notes[i]->wav = wav;
    res->notes[i]->device = devUtil;
  }

  res->isOn = false;

  return res;
}

void destroyNote(Note *note){
  destroyWaveData(note->wav);
  destroyDeviceUtil(note->device);
  note->wav = NULL;
  note->device = NULL;

  free(note);
}

void destroyOscillator(Oscillator *osc){
  for(int i = 0; i < 8; i++){
    destroyNote(osc->notes[i]);
    osc->notes[i] = NULL;
  }

  free(osc);
}
