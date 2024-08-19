#include <stdio.h>
#define MINIAUDIO_IMPLEMENTATION
#import "../include/miniaudio.h"
#import "sounds.c"
#import "utils.c"

#import "../include/raylib.h"

#define RAYGUI_IMPLEMENTATION
#import "../include/raygui.h"

#define DEVICE_FORMAT ma_format_f32
#define DEVICE_CHANNELS 2
#define DEVICE_SAMPLE_RATE 48000

int main(void)
{
  //GUI properties
  const int screenHeight = 540;
  const int screenWidth = 800;
  bool devicePlaying = false;
  int prevX; // For drawing lines in between the dots...
  int prevY;
  bool hasBegun = false;
  int marginX = 25;
  int marginY = 75;

  //Initialize beginning wave
  WaveData userData;
  WaveData* pUser = &userData;
  ma_waveform sineWave;
  ma_waveform_config sineConfig;

  //Initialize the arrays for the three waveforms
  ma_waveform *pWaveforms = malloc(3 * sizeof(ma_waveform));
  WaveData *pUserDataArr = malloc(3 * sizeof(WaveData));
  ma_device *pDevices = malloc(3 * sizeof(ma_device));
  bool devicesPlaying[3] = { false };
  bool devicesToggled[3] = { false };

   for(int i = 0; i < 3; i++){
     pUserDataArr[i].sineWave = &pWaveforms[i]; //TODO: rename sineWave WaveData
     initializePUserData(&pUserDataArr[i]);
     initializeDevice(&pDevices[i], &pUserDataArr[i]);
   }

  //The devices have been successfully initialized and they're ready to go

   for(int i = 0; i < 3; i++){
     makeWaveForm(&pDevices[i], ma_waveform_type_sine, &pWaveforms[i]);
   }

  //Userdata holds the data of the waves so that they can be changed in the callback
  //TODO: update variable names to be more general
  userData.sineWave = &sineWave;
  userData.amplitude = 0.2f;

  //Begin initializing device
  ma_device device;
  ma_device_config deviceConfig;

  //Initialize device
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

  //Makes the waveform and initializes it
  makeWaveForm(&device, ma_waveform_type_sine, &sineWave);

  //More GUI properties
  InitWindow(screenWidth,screenWidth,"osc-delux3");
  SetTargetFPS(120);
  char keyCodeStr[20];
  //Start main game loop
  while(!WindowShouldClose()){
    int keyCode = GetKeyPressed();
    if(keyCode != 0){
      snprintf(keyCodeStr, sizeof(keyCodeStr), "key down: %d", keyCode);
    }
    //Space bar keycode is 32

    //If the space bar is down, all toggled devices should play
    if(IsKeyDown(32)){
      for(int iDevice = 0; iDevice < 3; iDevice++){
        if(devicesToggled[iDevice] && !devicesPlaying[iDevice]){
          ma_device_set_master_volume(&pDevices[iDevice], 1.0f);
          devicesPlaying[iDevice] = true;
        }
      }
    }

    if(IsKeyUp(32)){
      for(int iDevice = 0; iDevice < 3; iDevice++){
        ma_device_set_master_volume(&pDevices[iDevice],0.0f);
        devicesPlaying[iDevice] = false;
      }
    }

      BeginDrawing();
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
      DrawText(keyCodeStr, 20, 20, 10, RED);
       //Draws the sine waves :3
      for(int iData = 0; iData < 3; iData++){
        if(devicesToggled[iData]){
        
          for(int i = 0; i < 1024; i++){
            int x = (i/3) + marginX; //3 changes the width. After the + it changes the margin
            int y = (screenHeight / (float)marginY) + (pUserDataArr[iData].buffer[i]*100) + (iData * marginY * 2 + 25);

            DrawPixel(x, y, RED);
          }
        }
      }

    //Draws the toggle buttons
      for(int iDevice = 0; iDevice < 3; iDevice++){
        char isOn[5];

        if(devicesToggled[iDevice]){
          strcpy(isOn, "OFF");
        }else{
          strcpy(isOn, "ON");
        }
        if(GuiButton((Rectangle){(1024.0f / 3) + marginX, (screenHeight / (float)marginY) + (iDevice * marginY * 2 + 25), 50, 50}, isOn)){
          if(devicesToggled[iDevice]){
            ma_device_stop(&pDevices[iDevice]);
          }else{
            ma_device_start(&pDevices[iDevice]);
          }
          devicesToggled[iDevice] = !devicesToggled[iDevice];
        }
    }

    //Draws wave buttons
    for(int iDevice = 0; iDevice < 3; iDevice++){
      if(GuiButton((Rectangle){(1024.0f / 3) + marginX + 50, (screenHeight / (float)marginY) + (iDevice * marginY * 2 + 25), 50, 50}, "SAW")){
        makeWaveForm(&pDevices[iDevice],ma_waveform_type_sawtooth,&pWaveforms[iDevice]);
      }

      if(GuiButton((Rectangle){(1024.0f / 3) + marginX + 100, (screenHeight / (float)marginY) + (iDevice * marginY * 2 + 25), 50, 50}, "SIN")){
              makeWaveForm(&pDevices[iDevice],ma_waveform_type_sine,&pWaveforms[iDevice]);
      }

      GuiSlider((Rectangle){(1024.0f / 3) + marginX + 150, (screenHeight / (float)marginY) + (iDevice * marginY * 2 + 25), 200, 10}, NULL, NULL, &pUserDataArr[iDevice].amplitude, 0.0f, 1.0f);
    }

    EndDrawing();
  }
  //Uninit everything
  CloseWindow();

  ma_device_uninit(&device);

} 
