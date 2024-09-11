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

  //Draw points - also select them
  int selectedIndex = -1;
  char idxText[100];
  for(int i = 0; i < state->numPoints; i++){
    const EnvEditorPoint *p = sortedPoints[i];

    const Vector2 screenPos = (Vector2){ p->position.x*innerBounds.width + innerBounds.x, innerBounds.y + innerBounds.height - p->position.y*innerBounds.height};
    const Rectangle pointRect = (Rectangle){ screenPos.x - pointSize/2.0f, screenPos.y - pointSize/2.0f, pointSize, pointSize};

    DrawRectangle(pointRect.x, pointRect.y, pointRect.width, pointRect.height, BLACK);
    if(CheckCollisionPointRec(mouse, pointRect)){
      selectedIndex = i;
    }
  }

  sprintf(idxText, "Selected: %i", selectedIndex);

  DrawText(idxText, bounds.x - 100, bounds.y - 100, 20, BLACK);
  //Draw curves
  for(int i = 0; i < state->numPoints - 1; i++){
    const EnvEditorPoint *p1 = sortedPoints[i];
    const EnvEditorPoint *p2 = sortedPoints[i+1];

    const Vector2 screenPos1 = (Vector2){ p1->position.x * innerBounds.width + innerBounds.x, innerBounds.y + innerBounds.height - p1->position.y * innerBounds.height};
    const Vector2 screenPos2 = (Vector2){ p2->position.x * innerBounds.width + innerBounds.x, innerBounds.y + innerBounds.height - p2->position.y * innerBounds.height};

    const float scale = (p2->position.x - p1->position.x)/3.0f;
    const Vector2 offset1 = (Vector2){scale, scale*p1->tangents.y};
    const Vector2 offset2 = (Vector2){-scale, -scale*p2->tangents.x};

    const Vector2 c1 = (Vector2){p1->position.x + offset1.x, p1->position.y + offset1.y};
    const Vector2 c2 = (Vector2){p2->position.x + offset2.x, p2->position.y + offset2.y};

    const Vector2 screenC1 = (Vector2){c1.x * innerBounds.width + innerBounds.x, c1.y * innerBounds.height + innerBounds.y};
    const Vector2 screenC2 = (Vector2){c2.x * innerBounds.width + innerBounds.x, c2.y * innerBounds.height + innerBounds.y};

    DrawSplineSegmentBezierCubic(screenPos1, screenC1, screenC2, screenPos2, 1, BLUE);
  }
}

void SetEnvelopePoints(EnvEditorState *envelope){
  envelope->start = 0;
  envelope->end = 100;
  envelope->numPoints = 4;
  envelope->selectedIndex = 0;
  envelope->editLeftTangent = false;
  envelope->editRightTangent = false;

  envelope->points[0].position = (Vector2){0.0f, 0.0f}; envelope->points[0].tangents = (Vector2){0.0f, 17.0f};
  envelope->points[1].position = (Vector2){0.15f, 1.0f}; envelope->points[1].tangents = (Vector2) {0.0f, -12.0f};
  envelope->points[2].position = (Vector2){0.35f, 0.96f}; envelope->points[2].tangents = (Vector2){12.0f, 7.0f};
  envelope->points[3].position = (Vector2){0.55f, 0.0f}; envelope->points[3].tangents = (Vector2){-14.0f, 0.0f};
}
