#include "raylib.h"
#include <stdio.h>

#ifndef ENV_EDITOR_MAX_POINTS
#define ENV_EDITOR_MAX_POINTS 4
#endif

typedef struct{
  Vector2 position;
  Vector2 tangents;

  bool leftLinear;
  bool rightLinear;
  bool movable;
  
}EnvEditorPoint;

typedef struct{
  float start;
  float end;

  int selectedIndex;

  EnvEditorPoint points[ENV_EDITOR_MAX_POINTS];
  int numPoints;
  Rectangle bounds;

  bool editLeftTangent;
  bool editRightTangent;
  Vector2 mouseOffset;
  
}EnvEditorState;

#include "raygui.h"
#include "stdlib.h"

EnvEditorState InitEnvelopeEditor(){
  EnvEditorState state = { 0 };

  state.start = 0;
  state.end = 1;

  state.selectedIndex = 0;
  state.editLeftTangent = false;
  state.editRightTangent = false;
  state.mouseOffset = (Vector2){ 0.0f, 0.0f };

  state.numPoints = 1;
  state.points[0].position = (Vector2){ 0.5f, 0.5f };
  state.points[0].tangents = (Vector2){ 0.0f, 0.0f };
  state.points[0].leftLinear = false;
  state.points[0].rightLinear = false;

  return state;
}

void SayHelloEditor(){
  printf("hello");
}

static int CompareEnvEditorPointPtr(const void *a, const void *b){
  float fa = (*(EnvEditorPoint**)a)->position.x;
  float fb = (*(EnvEditorPoint**)b)->position.x;

  return ((fa>fb) - (fa < fb));
}


void EnvelopeGraphEditor(Rectangle bounds, EnvEditorState *state){
  const float pointSize = 10.0f;
  const float fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
  const float handleLength = pointSize*2.5f;
  const float handleSize = pointSize/1.5f;

  const Rectangle innerBounds = (Rectangle){ bounds.x + fontSize, bounds.y + fontSize, bounds.width - 2*fontSize, bounds.height - 2*fontSize};
  const Vector2 mouse = GetMousePosition();
  const Vector2 mouseLocal = (Vector2){ (mouse.x - innerBounds.x)/innerBounds.width, (innerBounds.y + innerBounds.height-mouse.y)/innerBounds.height};

  //Sort points
  EnvEditorPoint *sortedPoints[ENV_EDITOR_MAX_POINTS] = { 0 };
  for(int i = 0; i < state->numPoints; i++) sortedPoints[i] = &state->points[i];
  qsort(sortedPoints,state->numPoints, sizeof(EnvEditorPoint*), CompareEnvEditorPointPtr);
  //DRAWING
  DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, GRAY);

  //Draw points
  for(int i = 0; i < state->numPoints; i++){
    
  }
}

void SetEnvelopePoints(EnvEditorState envelope){
  envelope.start = 0;
  envelope.end = 100;
  envelope.numPoints = 4;
  envelope.selectedIndex = 0;
  envelope.editLeftTangent = false;
  envelope.editRightTangent = false;

  envelope.points[0].position = (Vector2){0.0f, 0.0f}; envelope.points[0].tangents = (Vector2){0.0f, 1.5f};
  envelope.points[1].position = (Vector2){0.4f, 0.64f}; envelope.points[1].tangents = (Vector2) {-2.82f, -4.49f};
}
