#include "envelope_graph_editor.h"

const int CURVE_CYCLE_LENGTH = 3;
const enum Curve_State curve_cycle[CURVE_CYCLE_LENGTH] = {C1, C2, LINEAR};

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

//For sorting the points
static int CompareEnvEditorPointPtr(const void *a, const void *b){
  float fa = (*(EnvEditorPoint**)a)->position.x;
  float fb = (*(EnvEditorPoint**)b)->position.x;

  return ((fa>fb) - (fa < fb));
}


int collision_cooldown = 20;
int currIdx = 0;

float EnvelopeCurveEval(EnvEditorState *state, float *time, float animationTime){
  //SORT
  EnvEditorPoint* sorted[ENV_EDITOR_MAX_POINTS];

  for(int i = 0; i < state->numPoints; i++) sorted[i] = &state->points[i];

  qsort(sorted, state->numPoints, sizeof(EnvEditorPoint*), CompareEnvEditorPointPtr);

  //This honestly should be in the state
  const float fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
  const Rectangle innerBounds = (Rectangle){ state->bounds.x + fontSize, state->bounds.y + fontSize, state->bounds.width - 2*fontSize, state->bounds.height - 2*fontSize};

  //points
  const EnvEditorPoint *p1 = sorted[currIdx];
  const EnvEditorPoint *p2 = sorted[currIdx + 1];
  const EnvEditorPoint *last = sorted[state->numPoints-1];

  //cycle time is the amount of time it takes to get from one point to the next
  //all cycle times added up == animationTime
  float cycleTime = ((p2->position.x - p1->position.x)/last->position.x) * animationTime;
  float t = *time/cycleTime;
  float timePos = state->bounds.x + (state->bounds.width * t);
  DrawLine(timePos + (innerBounds.x - state->bounds.x), state->bounds.y, timePos + (innerBounds.x - state->bounds.x), state->bounds.y + state->bounds.height, BLUE);

  //MANAGE TIME
  if(*time > cycleTime){
    currIdx++;
    if(currIdx >= state->numPoints - 1) currIdx = 0;
    *time = 0;
  }
  //where the points are positioned on the screen  
  const Vector2 screenPos1 = (Vector2){ p1->position.x * innerBounds.width + innerBounds.x, innerBounds.y + innerBounds.height - p1->position.y * innerBounds.height};
  const Vector2 screenPos2 = (Vector2){ p2->position.x * innerBounds.width + innerBounds.x, innerBounds.y + innerBounds.height - p2->position.y * innerBounds.height}; 

  float lowScreenPosY = innerBounds.y + innerBounds.height - sorted[0]->position.y * innerBounds.height;
  float highScreenPosY = innerBounds.y + innerBounds.height - sorted[1]->position.y * innerBounds.height;


  float pixelX;
  float pixelY;

  //FEATURE: evaluate curve based on the bezier position
  //TODO: return value based on the bezier position
  if(curve_cycle[p1->curve_state_index] == C1){
    
    const Vector2 screenC1 = (Vector2){screenPos1.x + ((screenPos2.x - screenPos1.x)/2.0f), fmin(screenPos1.y, screenPos2.y)};

    pixelX = (1-t)*(1-t)*screenPos1.x+2*(1-t)*t*screenC1.x+t*t*screenPos2.x;
    pixelY = (1-t)*(1-t)*screenPos1.y+2*(1-t)*t*screenC1.y+t*t*screenPos2.y;

    //DEBUGGING: circle tracing the bezier curve
    DrawCircle(pixelX, pixelY, 2.0f, PURPLE);

    //formula to get a number between 0 and 1 that represents the evaluation
    return fabs(pixelY - lowScreenPosY) / (lowScreenPosY - highScreenPosY - 1);
  }
  else if(curve_cycle[p1->curve_state_index] == C2){
    const Vector2 screenC2 = (Vector2){screenPos1.x + ((screenPos2.x - screenPos1.x)/2.0f), fmax(screenPos1.y, screenPos2.y)};

    pixelX = (1-t)*(1-t)*screenPos1.x+2*(1-t)*t*screenC2.x+t*t*screenPos2.x;
    pixelY = (1-t)*(1-t)*screenPos1.y+2*(1-t)*t*screenC2.y+t*t*screenPos2.y;

    //DEBUGGING: circle tracing the bezier curve
    DrawCircle(pixelX, pixelY, 2.0f, PURPLE);

    return fabs(pixelY - lowScreenPosY) / (lowScreenPosY - highScreenPosY - 1);

  }else{
    pixelX = (1-t)*screenPos1.x+(t * screenPos2.x);
    pixelY = (1-t)*screenPos1.y+(t * screenPos2.y);

    //DEBUGGING: circle tracing the bezier curve
    DrawCircle(pixelX, pixelY, 2.0f, PURPLE);

    return fabs(pixelY - lowScreenPosY) / (lowScreenPosY - highScreenPosY - 1);
  }


  //DEBUGGING: line tracing the bounds
  Vector2 line1 = (Vector2){ screenPos2.x, state->bounds.y};
  Vector2 line2 = (Vector2){ screenPos2.x, state->bounds.y + state->bounds.height };
  DrawLineV(line1, line2, PURPLE);
  //if all else fails
  return state->start;
}

//For double-clicking
float currClicks = 0.0f;
bool doubleClick = false;
void EnvelopeGraphEditor(Rectangle bounds, EnvEditorState *state){
  const float pointSize = 10.0f;
  const float fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
  const float handleLength = pointSize*2.5f;
  const float handleSize = pointSize/1.5f;

  const Rectangle innerBounds = (Rectangle){ bounds.x + fontSize, bounds.y + fontSize, bounds.width - 2*fontSize, bounds.height - 2*fontSize};
  const Vector2 mouse = GetMousePosition();
  const Vector2 mouseLocal = (Vector2){ (mouse.x - innerBounds.x)/innerBounds.width, (innerBounds.y + innerBounds.height-mouse.y)/innerBounds.height};

  //Check for double click
  currClicks = currClicks <= 0 ? 0 : currClicks - 0.05;
  
  if(currClicks <= 1) doubleClick = false;
  else doubleClick = true;

  if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
    currClicks += 1.0f;
  }

  state->bounds.x = bounds.x;
  state->bounds.y = bounds.y;
  state->bounds.width = bounds.width;
  state->bounds.height = bounds.height;

  //Sort points
  EnvEditorPoint *sortedPoints[ENV_EDITOR_MAX_POINTS] = { 0 };
  for(int i = 0; i < state->numPoints; i++) sortedPoints[i] = &state->points[i];
  qsort(sortedPoints,state->numPoints, sizeof(EnvEditorPoint*), CompareEnvEditorPointPtr);
  //DRAWING
  DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, Fade(GRAY, 0.2f));

  //Draw points - also select them
  state->selectedIndex = -1;
  char idxText[100];
  for(int i = 0; i < state->numPoints; i++){
    const EnvEditorPoint *p = sortedPoints[i];

    const Vector2 screenPos = (Vector2){ p->position.x*innerBounds.width + innerBounds.x, innerBounds.y + innerBounds.height - p->position.y*innerBounds.height};
    const Rectangle pointRect = (Rectangle){ screenPos.x - pointSize/2.0f, screenPos.y - pointSize/2.0f, pointSize, pointSize};

    //Selects the point the cursor is hovering over
    if(CheckCollisionPointRec(mouse, pointRect)){
      state->selectedIndex = i;
    }

    DrawRectangle(pointRect.x, pointRect.y, pointRect.width, pointRect.height, state->selectedIndex == i ? PURPLE : BLACK);
  }

  //debugging - do not delete
  // sprintf(idxText, "Selected: %f", currClicks);

  //Draw curves
  for(int i = 0; i < state->numPoints - 1; i++){
    const EnvEditorPoint *p1 = sortedPoints[i];
    const EnvEditorPoint *p2 = sortedPoints[i+1];

    const Vector2 screenPos1 = (Vector2){ p1->position.x * innerBounds.width + innerBounds.x, innerBounds.y + innerBounds.height - p1->position.y * innerBounds.height};
    const Vector2 screenPos2 = (Vector2){ p2->position.x * innerBounds.width + innerBounds.x, innerBounds.y + innerBounds.height - p2->position.y * innerBounds.height};

    const float scale = (p2->position.x - p1->position.x)/3.0f;
    const Vector2 offset1 = (Vector2){scale, scale*p1->tangents.y};
    const Vector2 offset2 = (Vector2){-scale, -scale*p2->tangents.x};

    //c1 is above the curve
    //c2 is below the curve
    //Basically the top and bottom parts of the bezier curve w/r/t the screen 
    const Vector2 screenC1 = (Vector2){screenPos1.x + ((screenPos2.x - screenPos1.x)/2.0f), fmin(screenPos1.y, screenPos2.y)};
    const Vector2 screenC2 = (Vector2){screenPos1.x + ((screenPos2.x - screenPos1.x)/2.0f), fmax(screenPos1.y, screenPos2.y)};

    DrawCircle(screenC1.x, screenC1.y, 5.0f, RED);
    DrawCircle(screenC2.x, screenC2.y, 5.0f, PURPLE);

    //Draws the lines according to the curve state i.e. what index curve_state_index is at
    if(curve_cycle[p1->curve_state_index] == C1) DrawSplineSegmentBezierQuadratic(screenPos1, screenC1, screenPos2, 1.0f, RED);
    else if(curve_cycle[p1->curve_state_index] == C2) DrawSplineSegmentBezierQuadratic(screenPos1, screenC2, screenPos2, 1.0f, RED);
    else DrawSplineSegmentLinear(screenPos1, screenPos2, 1.0f, RED);
  }

  //Selecting a point
  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (state->selectedIndex != -1)){
    if(state->selectedIndex != 0){
      EnvEditorPoint *p = &state->points[state->selectedIndex];
      EnvEditorPoint *p0 = &state->points[state->selectedIndex - 1];
      if(state->selectedIndex != state->numPoints - 1){
        EnvEditorPoint *p2 = &state->points[state->selectedIndex + 1];
      }else{
        EnvEditorPoint *p2 = NULL;
      }

      const Vector2 newLocalPos = (Vector2){ mouseLocal.x + state->mouseOffset.x, mouseLocal.y + state->mouseOffset.y };

      p->position.x = (newLocalPos.x < 0) ? 0 : ((newLocalPos.x > 1) ? 1 : newLocalPos.x);
      if(state->selectedIndex == 2) p->position.y = (newLocalPos.y < 0) ? 0 : ((newLocalPos.y > 1) ? 1 : newLocalPos.y);
    }
  }
  //If you double click on a point, its curve toggles from C1 to C2
  if(doubleClick && state->selectedIndex != -1){
    EnvEditorPoint *p = sortedPoints[state->selectedIndex];

    p->curve_state_index = (p->curve_state_index + 1) % CURVE_CYCLE_LENGTH;
    doubleClick = false;
    currClicks = 0;
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
