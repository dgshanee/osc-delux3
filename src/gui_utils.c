#include "gui_utils.h"

void InitCamera(Camera *camera){
  camera->position = (Vector3){ 0.0f, 0.0f, 10.0f };
  camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
  camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };
  camera->fovy = 45.0f;
  camera->projection = CAMERA_PERSPECTIVE;
}

void DrawMovingSkull(float x, float y, double height, double width){
  Model model = LoadModel("../assets/wireframe.obj");

  Vector3 position = { x, y, 0.0f };
}
