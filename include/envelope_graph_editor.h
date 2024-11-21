#ifndef ENVELOPE_GRAPH_EDITOR_H
#define ENVELOPE_GRAPH_EDITOR_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define ENV_EDITOR_MAX_POINTS 4


typedef struct{
  Vector2 position;
  Vector2 tangents;

  bool leftLinear;
  bool rightLinear;
  bool movable;
  //Current index of the curve state. 0 == C1, 1 == C2, &c.
  int curve_state_index;  
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


enum Curve_State{
  C1,
  C2,
  LINEAR
};

EnvEditorState InitEnvelopeEditor();

void SayHelloEditor();

static int CompareEnvEditorPointPtr(const void *a, const void *b);

float EnvelopeCurveEval(EnvEditorState*, float*, float);

void EnvelopeGraphEditor(Rectangle, EnvEditorState*);

void SetEnvelopePoints(EnvEditorState*);

#endif
