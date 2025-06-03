#include "DeviceUtil.h"

DeviceUtil *createDeviceUtil(WaveData *wav){
  DeviceUtil *res = (DeviceUtil*)malloc(sizeof(DeviceUtil));

  ma_device *device = (ma_device*)malloc(sizeof(ma_device));
  ma_device_config *deviceConfig = (ma_device_config*)malloc(sizeof(ma_device_config));

  *deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig->playback.format = DEV_FORMAT;
  deviceConfig->playback.channels = DEV_CHANNELS;
  deviceConfig->sampleRate = DEV_SAMPLE_RATE;
  deviceConfig->dataCallback = data_callback;
  deviceConfig->pUserData = wav;

  if(ma_device_init(NULL, deviceConfig, device) != MA_SUCCESS){
    printf("Failed to load device\n");
    return NULL;
  }

  //start it here to debug
  if(ma_device_start(device) != MA_SUCCESS){
    printf("failed to start device\n");
  }

  res->device = device;
  res->deviceConfig = deviceConfig;

  printf("%p\n", res->device->playback.pIntermediaryBuffer);

  return res;
}

void destroyDeviceUtil(DeviceUtil *devUtil){

  devUtil->device->playback.pIntermediaryBuffer = NULL;
  
  ma_device_uninit(devUtil->device);
  free(devUtil->device);
  devUtil->device = NULL;

  free(devUtil->deviceConfig);
  devUtil->deviceConfig = NULL;

  free(devUtil);
  devUtil = NULL;
}
