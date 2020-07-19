#ifndef DRAWNODES_H
#define DRAWNODES_H

#include "raylib.h"
#include "drawgui.h"

Camera3D drawNodesCam;

typedef struct {
    unsigned int mode;              // Current camera mode
    float targetDistance;           // Camera distance from position to target
    float playerEyesPosition;       // Player eyes position from ground (in meters)
    Vector3 angle;                  // Camera angle in plane XZ

    // Camera movement control keys
    int moveControl[6];             // Move controls (CAMERA_FIRST_PERSON)
    int smoothZoomControl;          // Smooth zoom control key
    int altControl;                 // Alternative control key
    int panControl;                 // Pan view control key
} CameraData;

static CameraData EDITORCAMERA = {        // Global CAMERA state context
    .mode = 0,
    .targetDistance = 0,
    .playerEyesPosition = 1.85f,
    .angle = { 0 },
    .moveControl = { 'W', 'S', 'D', 'A', 'E', 'Q' },
    .smoothZoomControl = 341,       // raylib: KEY_LEFT_CONTROL
    .altControl = 342,              // raylib: KEY_LEFT_ALT
    .panControl = 2                 // raylib: MOUSE_MIDDLE_BUTTON
};

typedef enum {
    MOVE_FRONT = 0,
    MOVE_BACK,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN
} CameraMove;

Vector2 previousMousePosition;

float PLAYER_MOVEMENT_SENSITIVITY;
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.003f
#define CAMERA_FIRST_PERSON_MIN_CLAMP 89.0f
#define CAMERA_FIRST_PERSON_MAX_CLAMP -89.0f
#define CAMERA_FREE_PANNING_DIVIDER 5.1f

void DrawNodes();
void SetCameraModeEditor(Camera camera, int mode);
void UpdateEditorCamera(Camera3D *camera);
void UpdateEditorCameraCustom(Camera3D *camera);

#endif
