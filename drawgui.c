#include "raylib.h"
#include "drawgui.h"
#include "moduleLoop.h"
#include <string.h>
#include "gameplay.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "../raygui-master/src/raygui.h"
#undef RAYGUI_IMPLEMENTATION

#define GUI_FILE_DIALOG_IMPLEMENTATION
#include "../raygui-master/examples/custom_file_dialog/gui_file_dialog.h"

#define GUI_PROPERTY_LIST_IMPLEMENTATION
#include "../raygui-master/examples/property_list/dm_property_list.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SIZEOF(A) (sizeof(A)/sizeof(A[0]))

//general properties list
GuiDMProperty genProp[] = {
	PBOOL("Cam Lock", 0, false),
	PFLOAT("Cam Speed", 0, 1.0f),
	PVEC3("Loc Int", 0, 1.0, 1.0, 1.0),
	PVEC3("Rot Int", 0, 1.0, 1.0, 1.0),
	PVEC3("Scale Int", 0, 1.0, 1.0, 1.0)
};
int genPropFocus = 0, genPropScroll = 0;

//selection properties list
GuiDMProperty selProp[] = {
	PSECTION("#102#Transform", 1, 3),
	PVEC3("Loc", 0, 0.0, 0.0, 0.0),
	PVEC3("Rot", 0, 0.0, 0.0, 0.0),
	PVEC3("Scale", 0, 1.0, 1.0, 1.0),
	PSECTION("#102#Col", 1, 3),
	PBOOL("Trigger", 0, false),
	PSELECT("Col Shape", 0, "Sphere;Box", 0),
	PVEC3("Col Scale", 0, 1.0, 1.0, 1.0),
	PSECTION("#102#Col Layers", 1, 32),
	PBOOL("Layer 0", 0, false),
	PBOOL("Layer 1", 0, false),
	PBOOL("Layer 2", 0, false),
	PBOOL("Layer 3", 0, false),
	PBOOL("Layer 4", 0, false),
	PBOOL("Layer 5", 0, false),
	PBOOL("Layer 6", 0, false),
	PBOOL("Layer 7", 0, false),
	PBOOL("Layer 8", 0, false),
	PBOOL("Layer 9", 0, false),
	PBOOL("Layer 10", 0, false),
	PBOOL("Layer 11", 0, false),
	PBOOL("Layer 12", 0, false),
	PBOOL("Layer 13", 0, false),
	PBOOL("Layer 14", 0, false),
	PBOOL("Layer 15", 0, false),
	PBOOL("Layer 16", 0, false),
	PBOOL("Layer 17", 0, false),
	PBOOL("Layer 18", 0, false),
	PBOOL("Layer 19", 0, false),
	PBOOL("Layer 20", 0, false),
	PBOOL("Layer 21", 0, false),
	PBOOL("Layer 22", 0, false),
	PBOOL("Layer 23", 0, false),
	PBOOL("Layer 24", 0, false),
	PBOOL("Layer 25", 0, false),
	PBOOL("Layer 26", 0, false),
	PBOOL("Layer 27", 0, false),
	PBOOL("Layer 28", 0, false),
	PBOOL("Layer 29", 0, false),
	PBOOL("Layer 30", 0, false),
	PBOOL("Layer 31", 0, false),
	PSECTION("#102#Layers Col", 1, 32),
	PBOOL("Layer 0", 0, false),
	PBOOL("Layer 1", 0, false),
	PBOOL("Layer 2", 0, false),
	PBOOL("Layer 3", 0, false),
	PBOOL("Layer 4", 0, false),
	PBOOL("Layer 5", 0, false),
	PBOOL("Layer 6", 0, false),
	PBOOL("Layer 7", 0, false),
	PBOOL("Layer 8", 0, false),
	PBOOL("Layer 9", 0, false),
	PBOOL("Layer 10", 0, false),
	PBOOL("Layer 11", 0, false),
	PBOOL("Layer 12", 0, false),
	PBOOL("Layer 13", 0, false),
	PBOOL("Layer 14", 0, false),
	PBOOL("Layer 15", 0, false),
	PBOOL("Layer 16", 0, false),
	PBOOL("Layer 17", 0, false),
	PBOOL("Layer 18", 0, false),
	PBOOL("Layer 19", 0, false),
	PBOOL("Layer 20", 0, false),
	PBOOL("Layer 21", 0, false),
	PBOOL("Layer 22", 0, false),
	PBOOL("Layer 23", 0, false),
	PBOOL("Layer 24", 0, false),
	PBOOL("Layer 25", 0, false),
	PBOOL("Layer 26", 0, false),
	PBOOL("Layer 27", 0, false),
	PBOOL("Layer 28", 0, false),
	PBOOL("Layer 29", 0, false),
	PBOOL("Layer 30", 0, false),
	PBOOL("Layer 31", 0, false),
	PSECTION("#102#Anim", 1, 2),
	PBOOL("Animated", 0, false),
	PVEC2("Frames", 0, 0, 20),
	PBOOL("Visible", 0, true),
	PBOOL("Hidden", 0, false)
};
int selPropFocus = 0, selPropScroll = 0;

int nodeTypeActive = 0;
bool nodeTypeEditMode = false;
int modelListActive = -1;
int modeListScrollIndex = 0;

char modelFiles[1024][1024];
char modelFileNames[1024][1024];
const char *modelFilesPtr[1024];

void GetModelNames(char *curDir)
{
	int fileCount = 0;
    char **files = GetDirectoryFiles(curDir, &fileCount);
    
    for (int x = 0; x < fileCount; x++)
    {
		char filePath[1024];
		strcpy(filePath, curDir );
		strcat(filePath, "/");
		strcat(filePath, files[x]);
		
		if (DirectoryExists(filePath))
		{
			if (strcmp(files[x], ".") != 0 && strcmp(files[x], ".."))
			{
				GetModelNames(filePath);
			}
		}
		else//not a directory
		{
			if (FileExists(filePath) && IsFileExtension(files[x], ".glb"))
			{
				//TraceLog(LOG_INFO, files[x]);
				
				strcpy(modelFiles[modelCount], filePath);
				strcpy(modelFileNames[modelCount], GetFileNameWithoutExt(filePath));
				modelFilesPtr[modelCount] = modelFileNames[modelCount];
				
				modelCount++;
			}
		}
	}
	
	ClearDirectoryFiles();
}

void DrawGui()
{
	//open dialog
	if (fileDialogState.SelectFilePressed)
	{
		// Load image file (if supported extension)
		if (IsFileExtension(fileDialogState.fileNameText, ".txt"))
		{
			strcpy(fileNameToLoad, TextFormat("%s/%s", fileDialogState.dirPathText, fileDialogState.fileNameText));
		}

		fileDialogState.SelectFilePressed = false;
	}
	
	if (fileDialogState.fileDialogActive) GuiLock();

	if (GuiButton((Rectangle){ 0, 0, 140, 30 }, GuiIconText(RICON_FILE_OPEN, "Open File"))) fileDialogState.fileDialogActive = true;

	GuiUnlock();
	GuiFileDialog(&fileDialogState);
	
	//save button
	GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
	if (GuiButton((Rectangle){ 140, 0, 125, 30 }, GuiIconText(RICON_FILE_SAVE, "Save File")))
	{
		//
	}
    
    //general properties list
    GuiDMPropertyList((Rectangle){0, 30, 180, 400}, genProp, SIZEOF(genProp), &genPropFocus, &genPropScroll);
    
    //selection properties list
    GuiDMPropertyList((Rectangle){SCREEN_WIDTH - 180, (SCREEN_HEIGHT - 280)/2, 180, 280}, selProp, SIZEOF(selProp), &selPropFocus, &selPropScroll);
    
    //add node butt
	Rectangle addNodeRect;
	addNodeRect.x = 390;
	addNodeRect.y = 0;
	addNodeRect.width = 50;
	addNodeRect.height = 25;
	
	DrawRectangleRec( addNodeRect , LIGHTGRAY);
	
	if (CheckCollisionPointRec(GetMousePosition(), addNodeRect) )
	{
		DrawRectangleLines( addNodeRect.x, addNodeRect.y,
		addNodeRect.width, addNodeRect.height, RED);
		
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		{
			AddNode();
		};
	}
	else
	{
		DrawRectangleLines( addNodeRect.x, addNodeRect.y,
		addNodeRect.width, addNodeRect.height, DARKGRAY);
	}
	
	DrawText( "add",
		addNodeRect.x,
		addNodeRect.y,
		20, MAROON);
		
    //new object type
    if (GuiDropdownBox((Rectangle){ 265,0, 125, 30 },
    "node;model;spawn;sfx;gfx;sprite;terrain;camera",
    &nodeTypeActive, nodeTypeEditMode)) nodeTypeEditMode = !nodeTypeEditMode;
    
    //models list
    modelListActive = GuiListViewEx((Rectangle){ 0, SCREEN_HEIGHT/2, 180, 384 }, modelFilesPtr, modelCount, NULL, &modeListScrollIndex, modelListActive);
}
