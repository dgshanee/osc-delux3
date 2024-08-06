#include <stdio.h>
#define MINIAUDIO_IMPLEMENTATION
#import "../include/miniaudio.h"

#import "../include/raylib.h"

#define RAYGUI_IMPLEMENTATION
#import "../include/raygui.h"

#define DEVICE_FORMAT ma_format_f32
#define DEVICE_CHANNELS 2
#define DEVICE_SAMPLE_RATE 48000

typedef struct{
  ma_waveform* sineWave;
  float* buffer;
  float frequency;
} SineData;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
  SineData* pSineData;
  ma_waveform* pSineWave;

  pSineData = (SineData*)pDevice->pUserData;
  MA_ASSERT(pSineData != NULL);
  pSineWave = (ma_waveform*)pSineData->sineWave;
  MA_ASSERT(pSineWave != NULL);

  ma_waveform_set_frequency(pSineWave, pSineData->frequency);

  ma_waveform_read_pcm_frames(pSineWave,pOutput,frameCount,NULL);

  pSineData->buffer = (float*)pOutput;

  (void)pInput;
  
}

void updateBuffer(float* buffer){
  for(int i = 0; i < 1024; i++){
    buffer[i] = i;
  }
}

int main(void)
{
  SineData userData;
  SineData* pUser = &userData;
  ma_waveform sineWave;
  ma_waveform_config sineConfig;

  userData.sineWave = &sineWave;

  ma_device device;
  ma_device_config deviceConfig;

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = DEVICE_FORMAT;
  deviceConfig.playback.channels = DEVICE_CHANNELS;
  deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &userData;
  
  if(ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS){
    printf("Error initializing device.");
    return -1;
  }

  sineConfig = ma_waveform_config_init(device.playback.format, device.playback.channels, device.sampleRate, ma_waveform_type_sine, 0.2, 220 );
  ma_waveform_init(&sineConfig, &sineWave);

  if(ma_device_start(&device) != MA_SUCCESS){
    printf("Error starting device.");
    ma_device_uninit(&device);
    return -1;
  }
  bool devicePlaying = true;

  const int screenHeight = 540;
  const int screenWidth = 800;
  InitWindow(screenWidth,screenWidth,"osc-delux3");
  SetTargetFPS(60);

  while(!WindowShouldClose()){
    for(int i = 0; i < 50; i++){
      printf("%f\n", userData.buffer[i]);
    }
    BeginDrawing();
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      for(int i = 0; i < 1024; i++){
        DrawPixel(i,(screenHeight / 2.0f) + (userData.buffer[i]*100), RED);
      }

      if(GuiButton((Rectangle){200, 200, 50, 50}, "Stop me")){
        if(devicePlaying){
          ma_device_stop(&device);
          devicePlaying = false;        
        }else{
          ma_device_start(&device);
          devicePlaying = true;        
        }
      }
      GuiSlider((Rectangle){400,400,200,10}, NULL, NULL, &userData.frequency, 220, 440);

    EndDrawing();
  }
  CloseWindow();

  ma_device_uninit(&device);
} 
