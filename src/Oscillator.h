#ifndef OSC_H
#define OSC_H

#include <stdlib.h>
#include "utils.h"

typedef struct{
  Note *notes[8];
  bool isOn;
}Oscillator;

Oscillator *createOscillator();

void deleteOscillator(Oscillator *);
#endif
