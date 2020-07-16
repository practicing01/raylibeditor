#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"
#include "gameplay.h"
#include "drawgui.h"
#include <string.h>

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
}

void GameplayExit()
{
	//
}

void GameplayLoop()
{
	BeginDrawing();
		ClearBackground(RAYWHITE);
		//DrawText("GameplayLoop", 190, 200, 20, LIGHTGRAY);
		
		DrawGui();
            
	EndDrawing();
	
	if (WindowShouldClose())
	{
		ExitGame();
	}
}
