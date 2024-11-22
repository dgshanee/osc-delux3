#ifndef OSC_H
#define OSC_H

#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

#include "DeviceUtil.h"
#include "WaveData.h"


typedef struct{
  WaveData *wav;
  DeviceUtil *device;
}Note;


typedef struct{
  Note *notes[8];
  bool isOn;
}Oscillator;

void destroyNote(Note *);

Oscillator *createOscillator();

void destroyOscillator(Oscillator *);
#endif
