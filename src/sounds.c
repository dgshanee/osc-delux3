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


