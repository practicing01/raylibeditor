#include "raylib.h"
#include "drawgui.h"
#include "moduleLoop.h"
#include "drawnodes.h"
#include <math.h>
#include "raymath.h"
#include "gameplay.h"

void DrawNodes()
{
	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
	
	BeginMode3D(drawNodesCam);
	
	DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, RED);
	DrawGrid(1000, 1.0f);
	
	struct nodeProperties *curNode;
	struct nodeProperties *nextNode;
	curNode = nodePropListStart;
	
	while (curNode != NULL)
	{
		Color tint = WHITE;
		
		if ( (*curNode).selected)
		{
			tint = LIME;
		}
		
		if ( (*curNode).nodeType == NODE)
		{
			//struct nodeTypeData *data = (struct nodeTypeData*)( (*curNode).nodeData );
			DrawCube( (*curNode).loc, 1.0f, 1.0f, 1.0f, tint);
		}
		else if ( (*curNode).nodeType == MODEL)
		{
			struct modelTypeData *data = (struct modelTypeData*)( (*curNode).nodeData );
			DrawModel( (*data).model, (*curNode).loc, 1.0f, tint);
		}
		
		curNode = (*curNode).next;
	}
	
	EndMode3D();
	
}

void SetCameraModeEditor(Camera camera, int mode)
{
    Vector3 v1 = camera.position;
    Vector3 v2 = camera.target;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;

    EDITORCAMERA.targetDistance = sqrtf(dx*dx + dy*dy + dz*dz);   // Distance to target

    // Camera angle calculation
    EDITORCAMERA.angle.x = atan2f(dx, dz);                        // Camera angle in plane XZ (0 aligned with Z, move positive CCW)
    EDITORCAMERA.angle.z = atan2f(dy, sqrtf(dx*dx + dz*dz));      // Camera angle in plane XY (0 aligned with X, move positive CW)

    EDITORCAMERA.playerEyesPosition = camera.position.y;          // Init player eyes position to camera Y position

    // Lock cursor for first person and third person cameras
    previousMousePosition = GetMousePosition();
    if ((mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON)) DisableCursor();
    else EnableCursor();

    EDITORCAMERA.mode = mode;
}

void UpdateEditorCamera(Camera3D *camera)
{
	bool direction[6] = { IsKeyDown(EDITORCAMERA.moveControl[MOVE_FRONT]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_BACK]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_RIGHT]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_LEFT]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_UP]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_DOWN]) };
                          
    static Vector2 previousMousePosition = { 0.0f, 0.0f };
    Vector2 mousePosition = GetMousePosition();

    Vector2 mousePositionDelta = { 0.0f, 0.0f };
    mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - previousMousePosition.y;
    
    previousMousePosition = mousePosition;
                          
	camera->position.x += (sinf(EDITORCAMERA.angle.x)*direction[MOVE_BACK] -
						   sinf(EDITORCAMERA.angle.x)*direction[MOVE_FRONT] -
						   cosf(EDITORCAMERA.angle.x)*direction[MOVE_LEFT] +
						   cosf(EDITORCAMERA.angle.x)*direction[MOVE_RIGHT])*genProps.camSpeed;
	
	camera->position.y += (sinf(EDITORCAMERA.angle.z)*direction[MOVE_FRONT] -
						   sinf(EDITORCAMERA.angle.z)*direction[MOVE_BACK] +
						   1.0f*direction[MOVE_UP] - 1.0f*direction[MOVE_DOWN])*genProps.camSpeed;
	
	camera->position.z += (cosf(EDITORCAMERA.angle.x)*direction[MOVE_BACK] -
						   cosf(EDITORCAMERA.angle.x)*direction[MOVE_FRONT] +
						   sinf(EDITORCAMERA.angle.x)*direction[MOVE_LEFT] -
						   sinf(EDITORCAMERA.angle.x)*direction[MOVE_RIGHT])*genProps.camSpeed;
	
	// Camera orientation calculation
	EDITORCAMERA.angle.x += (mousePositionDelta.x*-CAMERA_MOUSE_MOVE_SENSITIVITY);
	EDITORCAMERA.angle.z += (mousePositionDelta.y*-CAMERA_MOUSE_MOVE_SENSITIVITY);
	
	// Angle clamp
	if (EDITORCAMERA.angle.z > CAMERA_FIRST_PERSON_MIN_CLAMP*DEG2RAD) EDITORCAMERA.angle.z = CAMERA_FIRST_PERSON_MIN_CLAMP*DEG2RAD;
	else if (EDITORCAMERA.angle.z < CAMERA_FIRST_PERSON_MAX_CLAMP*DEG2RAD) EDITORCAMERA.angle.z = CAMERA_FIRST_PERSON_MAX_CLAMP*DEG2RAD;
	
	// Recalculate camera target considering translation and rotation
	Matrix translation = MatrixTranslate(0, 0, (EDITORCAMERA.targetDistance/CAMERA_FREE_PANNING_DIVIDER));
	Matrix rotation = MatrixRotateXYZ((Vector3){ PI*2 - EDITORCAMERA.angle.z, PI*2 - EDITORCAMERA.angle.x, 0 });
	Matrix transform = MatrixMultiply(translation, rotation);
	
	camera->target.x = camera->position.x - transform.m12;
	camera->target.y = camera->position.y - transform.m13;
	camera->target.z = camera->position.z - transform.m14;
}

void UpdateEditorCameraCustom(Camera3D *camera)//fucked and i don't know why. maybe cus i can't math.
{
	bool direction[6] = { IsKeyDown(EDITORCAMERA.moveControl[MOVE_FRONT]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_BACK]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_RIGHT]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_LEFT]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_UP]),
                          IsKeyDown(EDITORCAMERA.moveControl[MOVE_DOWN]) };
                          
    Vector2 mousePosition = GetMousePosition();

    Vector2 mousePositionDelta = { 0.0f, 0.0f };
    mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - previousMousePosition.y;
	
	// Camera orientation calculation
	EDITORCAMERA.angle.x += (mousePositionDelta.x*CAMERA_MOUSE_MOVE_SENSITIVITY);
	EDITORCAMERA.angle.z += (mousePositionDelta.y*CAMERA_MOUSE_MOVE_SENSITIVITY);
	
	// Angle clamp
	/*if (EDITORCAMERA.angle.x > 360.0f) EDITORCAMERA.angle.x = 0.0f;
	else if (EDITORCAMERA.angle.x < 0.0f) EDITORCAMERA.angle.x = 360.0f;
	
	if (EDITORCAMERA.angle.z > 360.0f) EDITORCAMERA.angle.z = 0.0f;
	else if (EDITORCAMERA.angle.z < 0.0f) EDITORCAMERA.angle.z = 360.0f;*/
	
	EDITORCAMERA.angle.x = fmod(EDITORCAMERA.angle.x, 360.0f);
	EDITORCAMERA.angle.z = fmod(EDITORCAMERA.angle.z, 360.0f);
	
	TraceLog(LOG_INFO, "x:%f", EDITORCAMERA.angle.x);
	//TraceLog(LOG_INFO, "%f", EDITORCAMERA.angle.z);
    
    previousMousePosition = mousePosition;
	
	Vector3 dir;
	dir.x = cosf( EDITORCAMERA.angle.x * DEG2RAD );
	dir.z = sinf( EDITORCAMERA.angle.z * DEG2RAD );
	
	dir = Vector3Scale(dir, 10.0f);
	
	camera->target.x = dir.x;
	camera->target.y = dir.y;
	camera->target.z = dir.z;
}
