#include <stdio.h>
#import "utils.c"
#define MINIAUDIO_IMPLEMENTATION
#import "../include/miniaudio.h"

#define DEVICE_FORMAT ma_format_f32
#define DEVICE_CHANNELS 2
#define DEVICE_SAMPLE_RATE 48000


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
  WaveData* pSineData;
  ma_waveform* pSineWave;

  pSineData = (WaveData*)pDevice->pUserData;
  MA_ASSERT(pSineData != NULL);
  pSineWave = (ma_waveform*)pSineData->sineWave;
  MA_ASSERT(pSineWave != NULL);

  ma_waveform_set_frequency(pSineWave, pSineData->frequency);
  ma_waveform_set_amplitude(pSineWave,pSineData->amplitude);

  ma_waveform_read_pcm_frames(pSineWave,pOutput,frameCount,NULL);

  pSineData->buffer = (float*)pOutput;

  (void)pInput;
  
}


void test_callback_1(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
  printf("hello");
}


void test_callback_2(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
  printf("!olleh");
}
DeviceUtil *generateDevice(){
  DeviceUtil *devUtil = (DeviceUtil *)malloc(sizeof(DeviceUtil));
  ma_device *device = (ma_device *)malloc(3 * sizeof(ma_device));
  ma_device_config deviceConfig;

  WaveData *wav;
  ma_waveform sine;
  ma_waveform_config sineConfig;

  

  devUtil->device = device;
  devUtil->deviceConfig = &deviceConfig;

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = DEVICE_FORMAT;
  deviceConfig.playback.channels = 2;
  deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
  deviceConfig.dataCallback = test_callback_1;
  deviceConfig.pUserData = wav;

  if(ma_device_init(NULL,&deviceConfig,device) != MA_SUCCESS){
    printf("failed to load device.");
    return NULL;
  }

  return devUtil;
  
}

ma_device *generateDevices(){
  ma_device *deviceArr = (ma_device *)malloc(3 * sizeof(ma_device));

  return deviceArr;
}

void makeWaveForm(ma_device *device, ma_waveform_type waveformType, ma_waveform *wave){
  ma_waveform_config waveConfig;

  waveConfig = ma_waveform_config_init(device->playback.format,device->playback.channels,device->sampleRate,waveformType,0.2,220);

  ma_waveform_init(&waveConfig, wave);

}

void initializeDevice(ma_device *device, WaveData *userData){
  ma_device_config deviceConfig;

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = DEVICE_FORMAT;
  deviceConfig.playback.channels = DEVICE_CHANNELS;
  deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = userData;

  if(ma_device_init(NULL, &deviceConfig, device) != MA_SUCCESS){
    printf("Error initializing device.\n");
    return;
  }
}

void initializeDeviceUtil(ma_device *device, DeviceUtil *device_util, WaveData *userData){
  ma_device_config deviceConfig;
  ma_device_config *pDeviceConfig = malloc(sizeof(ma_device_config));
  pDeviceConfig = &deviceConfig;

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = DEVICE_FORMAT;
  deviceConfig.playback.channels = DEVICE_CHANNELS;
  deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = userData;

  if(ma_device_init(NULL, &deviceConfig, device) != MA_SUCCESS){
    printf("Error initializing device.\n");
    return;
  }

  device_util->device = device;
  device_util->deviceConfig = &deviceConfig;
}


void initializeWaveData(WaveData *pUserData){
  float buffer;
  float frequency = 220;
  float amplitude = 0.2f;
  pUserData->buffer = &buffer;
  pUserData->frequency = frequency;
  pUserData->amplitude = amplitude;
}

Oscillator *createOscillator(){
  Oscillator *osc = malloc(sizeof(Oscillator));

  //initialize WaveData
  WaveData **notes = malloc(8 * sizeof(WaveData));
  for(int i = 0; i < 8; i++){
    notes[i] = malloc(sizeof(WaveData));
  }
  //initialize DeviceUtil
  DeviceUtil **deviceUtils = malloc(8 * sizeof(DeviceUtil));
  for(int i = 0; i < 8; i++){
    deviceUtils[i] = malloc(sizeof(DeviceUtil));
    ma_device_config *pDeviceConfig = malloc(sizeof(ma_device_config));
    ma_device *device = malloc(sizeof(ma_device));

    *pDeviceConfig = ma_device_config_init(ma_device_type_playback);
    pDeviceConfig->playback.format = DEVICE_FORMAT;
    pDeviceConfig->playback.channels = DEVICE_CHANNELS;
    pDeviceConfig->sampleRate = DEVICE_SAMPLE_RATE;
    pDeviceConfig->dataCallback = data_callback;
    pDeviceConfig->pUserData = notes[i];

    deviceUtils[i]->device = device;
    deviceUtils[i]->deviceConfig = pDeviceConfig;

    if(ma_device_init(NULL, pDeviceConfig, device) != MA_SUCCESS){
      printf("Error initializing device\n");
    }
  }

  for(int i = 0; i < 8; i++){
    ma_device *currDevice = deviceUtils[i]->device;
    ma_waveform *waveData = malloc(sizeof(ma_waveform));
    ma_waveform_config *pWaveConfig = malloc(sizeof(ma_waveform_config));
    *pWaveConfig = ma_waveform_config_init(currDevice->playback.format, currDevice->playback.channels, currDevice->sampleRate, ma_waveform_type_sine, 0.2, 220);
    if(ma_waveform_init(pWaveConfig, waveData) != MA_SUCCESS){
      printf("Error initializing waveform");
    };
    notes[i]->sineWave = malloc(sizeof(ma_waveform));
    notes[i]->frequency = 220;
    notes[i]->amplitude = 0.2f;
    notes[i]->sineWave = waveData;
  }

  printf("%f", notes[0]->sineWave->config.amplitude);

  for(int i = 0; i < 8; i++){
    ma_device *device = deviceUtils[i]->device;
    if(ma_device_start(device) != MA_SUCCESS){
      printf("Error startinf device\n");
    }
  }

  osc->notes = notes;
  osc->devices = deviceUtils;
  
  osc->isOn = false;

  return osc;
}

