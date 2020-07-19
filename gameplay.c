#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"
#include "gameplay.h"
#include "drawgui.h"
#include <string.h>
#include "drawnodes.h"

void UpdateGuiValues()
{
	if(IsKeyReleased(KEY_ENTER))
	{
		PLAYER_MOVEMENT_SENSITIVITY = genProp[1].value.vfloat.val;
		drawNodesCam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	}
}

void ToggleCursor()
{
	if(IsKeyReleased(KEY_SPACE))
	{
		if (IsCursorHidden())
		{
			EnableCursor();
		}
		else
		{//neither work for stopping the camera from jerking off. maybe i'm doing something wrong...
			DisableCursor();
			//previousMousePosition = GetMousePosition();
			//SetMousePosition(previousMousePosition.x, previousMousePosition.y);
		}
	}
}

void GameplayInit()
{
	SetWindowSize(1024, 768);
	
	dt.elapsedTime = 0.0f;
	ModuleLoop = GameplayLoop;
	
	fileDialogState = InitGuiFileDialog(512, 384, GetWorkingDirectory(), false);
	
	GuiLoadStyleDefault();
	GuiSetStyle(LISTVIEW, LIST_ITEMS_HEIGHT, 24);
	GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 12);
	
	modelCount = 0;
	const char *workDir = GetWorkingDirectory();
	char curDir[1024];
	strcpy(curDir, workDir );
	strcat(curDir, "/models");
	GetModelNames(curDir);
	
	drawNodesCam.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    drawNodesCam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    drawNodesCam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    drawNodesCam.fovy = 45.0f;
    drawNodesCam.type = CAMERA_PERSPECTIVE;
    
    SetCameraModeEditor(drawNodesCam, CAMERA_FIRST_PERSON);
    
    SetCameraMoveControls(KEY_W, KEY_S, KEY_D, KEY_A, KEY_E, KEY_Q);
    PLAYER_MOVEMENT_SENSITIVITY = 1.0;
}

void GameplayExit()
{
	//
}

void GameplayLoop()
{
	ToggleCursor();
	
	UpdateGuiValues();
	
	if (IsCursorHidden())
	{
		UpdateEditorCamera(&drawNodesCam);
		//UpdateEditorCameraCustom(&drawNodesCam);
	}
	
	BeginDrawing();
		ClearBackground(RAYWHITE);
		
		DrawNodes();
		
		DrawGui();
            
	EndDrawing();
	
	if (WindowShouldClose())
	{
		ExitGame();
	}
}
