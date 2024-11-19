
#define MINIAUDIO_IMPLEMENTATION
#import "../include/miniaudio.h"

#include "../include/gui_curve_editor.h"

typedef struct{
  ma_waveform* sineWave;
  float* buffer;
  float frequency;
  float amplitude;
} WaveData;

typedef struct{
  ma_device* device;
  ma_device_config* deviceConfig;  
} DeviceUtil;


typedef struct{
  WaveData **notes;
  DeviceUtil **devices;
  bool isOn;
}Oscillator;

void freeOscillator(Oscillator *pOsc){
  free(pOsc);
}

void LoadCurveDefaults(GuiCurveEditorState curves[])
{
    // X pos
    curves[0].start = 0;
    curves[0].end = 60;
    curves[0].numPoints = 4;
    curves[0].selectedIndex = 0;
    curves[0].editLeftTangent = false;
    curves[0].editRightTangent = false;
    curves[0].points[0].position =(Vector2) {0.000000, 0.000000}; curves[0].points[0].tangents = (Vector2) {0.000000, 1.515101};  curves[0].points[0].leftLinear =   1;curves[0].points[0].rightLinear =  1;
    curves[0].points[1].position =(Vector2) {0.422414, 0.640000}; curves[0].points[1].tangents = (Vector2) {-2.824348, -4.494999};curves[0].points[1].leftLinear =   0;curves[0].points[1].rightLinear =  0;
    curves[0].points[2].position =(Vector2) {0.732759, 0.210000}; curves[0].points[2].tangents = (Vector2) {0.000000, 2.956133};  curves[0].points[2].leftLinear =   0;curves[0].points[2].rightLinear =  1;
    curves[0].points[3].position =(Vector2) {1.000000, 1.000000}; curves[0].points[3].tangents = (Vector2) {2.956133, 0.000000};  curves[0].points[3].leftLinear =   1;curves[0].points[3].rightLinear =  1;

    // // Y pos
    // curves[1].start = 405;
    // curves[1].end = 135;
    // curves[1].numPoints = 7;
    // curves[1].selectedIndex = 0;
    // curves[1].editLeftTangent = false;
    // curves[1].editRightTangent = false;
    // curves[1].points[0].position = (Vector2) {0.000000, 1.000000};curves[1].points[0].tangents = (Vector2) { 0.000000  , 0.000000};curves[1].points[0].leftLinear =  0;curves[1].points[0].rightLinear =  0;
    // curves[1].points[1].position = (Vector2) {0.140000, 0.000000};curves[1].points[1].tangents = (Vector2) {-10.000000 ,10.000000};curves[1].points[1].leftLinear =  0;curves[1].points[1].rightLinear =  0;
    // curves[1].points[2].position = (Vector2) {0.450000, 0.000000};curves[1].points[2].tangents = (Vector2) {-10.000000 ,10.000000};curves[1].points[2].leftLinear =  0;curves[1].points[2].rightLinear =  0;
    // curves[1].points[3].position = (Vector2) {0.670000, 0.000000};curves[1].points[3].tangents = (Vector2) {-10.000000 ,10.000000};curves[1].points[3].leftLinear =  0;curves[1].points[3].rightLinear =  0;
    // curves[1].points[4].position = (Vector2) {0.830000, 0.000000};curves[1].points[4].tangents = (Vector2) {-10.000000 ,10.000000};curves[1].points[4].leftLinear =  0;curves[1].points[4].rightLinear =  0;
    // curves[1].points[5].position = (Vector2) {0.940000, 0.000000};curves[1].points[5].tangents = (Vector2) {-10.000000 ,10.000000};curves[1].points[5].leftLinear =  0;curves[1].points[5].rightLinear =  0;
    // curves[1].points[6].position = (Vector2) {1.000000, 0.000000};curves[1].points[6].tangents = (Vector2) {-10.000000 , 0.000000};curves[1].points[6].leftLinear =  0;curves[1].points[6].rightLinear =  0;

    // // X size
    // curves[2].start = 1;
    // curves[2].end = 64;
    // curves[2].numPoints = 16;
    // curves[2].selectedIndex = 0;
    // curves[2].editLeftTangent = false;
    // curves[2].editRightTangent = false;
    // curves[2].points[0].position =  (Vector2) {0.000000, 0.492063}; curves[2].points[0].tangents =  (Vector2) {0,0}; curves[2].points[0].leftLinear =  0; curves[2].points[0].rightLinear =  0;
    // curves[2].points[1].position =  (Vector2) {0.130000, 0.492063}; curves[2].points[1].tangents =  (Vector2) {0,0}; curves[2].points[1].leftLinear =  0; curves[2].points[1].rightLinear =  0;
    // curves[2].points[2].position =  (Vector2) {0.140000, 0.746032}; curves[2].points[2].tangents =  (Vector2) {0,0}; curves[2].points[2].leftLinear =  0; curves[2].points[2].rightLinear =  0;
    // curves[2].points[3].position =  (Vector2) {0.150000, 0.492063}; curves[2].points[3].tangents =  (Vector2) {0,0}; curves[2].points[3].leftLinear =  0; curves[2].points[3].rightLinear =  0;
    // curves[2].points[4].position =  (Vector2) {0.440000, 0.490000}; curves[2].points[4].tangents =  (Vector2) {0,0}; curves[2].points[4].leftLinear =  0; curves[2].points[4].rightLinear =  0;
    // curves[2].points[5].position =  (Vector2) {0.450000, 0.682540}; curves[2].points[5].tangents =  (Vector2) {0,0}; curves[2].points[5].leftLinear =  0; curves[2].points[5].rightLinear =  0;
    // curves[2].points[6].position =  (Vector2) {0.460000, 0.480000}; curves[2].points[6].tangents =  (Vector2) {0,0}; curves[2].points[6].leftLinear =  0; curves[2].points[6].rightLinear =  0;
    // curves[2].points[7].position =  (Vector2) {0.660000, 0.492063}; curves[2].points[7].tangents =  (Vector2) {0,0}; curves[2].points[7].leftLinear =  0; curves[2].points[7].rightLinear =  0;
    // curves[2].points[8].position =  (Vector2) {0.670000, 0.619048}; curves[2].points[8].tangents =  (Vector2) {0,0}; curves[2].points[8].leftLinear =  0; curves[2].points[8].rightLinear =  0;
    // curves[2].points[9].position =  (Vector2) {0.680000, 0.492063}; curves[2].points[9].tangents =  (Vector2) {0,0}; curves[2].points[9].leftLinear =  0; curves[2].points[9].rightLinear =  0;
    // curves[2].points[10].position = (Vector2) {0.820000, 0.492063}; curves[2].points[10].tangents = (Vector2) {0,0}; curves[2].points[10].leftLinear = 0; curves[2].points[10].rightLinear = 0;
    // curves[2].points[11].position = (Vector2) {0.830000, 0.619048}; curves[2].points[11].tangents = (Vector2) {0,0}; curves[2].points[11].leftLinear = 0; curves[2].points[11].rightLinear = 0;
    // curves[2].points[12].position = (Vector2) {0.840000, 0.492063}; curves[2].points[12].tangents = (Vector2) {0,0}; curves[2].points[12].leftLinear = 0; curves[2].points[12].rightLinear = 0;
    // curves[2].points[13].position = (Vector2) {0.930000, 0.492063}; curves[2].points[13].tangents = (Vector2) {0,0}; curves[2].points[13].leftLinear = 0; curves[2].points[13].rightLinear = 0;
    // curves[2].points[14].position = (Vector2) {0.940000, 0.619048}; curves[2].points[14].tangents = (Vector2) {0,0}; curves[2].points[14].leftLinear = 0; curves[2].points[14].rightLinear = 0;
    // curves[2].points[15].position = (Vector2) {0.950000, 0.492063}; curves[2].points[15].tangents = (Vector2) {0,0}; curves[2].points[15].leftLinear = 0; curves[2].points[15].rightLinear = 0;
}
