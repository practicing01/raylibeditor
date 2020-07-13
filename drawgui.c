#include "raylib.h"
#include "drawgui.h"
#include "moduleLoop.h"

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
	PVEC3("Loc", 0, 1.0, 1.0, 1.0),
	PVEC3("Rot", 0, 1.0, 1.0, 1.0),
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
    GuiDMPropertyList((Rectangle){0, (SCREEN_HEIGHT - 280)/2, 180, 280}, genProp, SIZEOF(genProp), &genPropFocus, &genPropScroll);
    
    //selection properties list
    GuiDMPropertyList((Rectangle){SCREEN_WIDTH - 180, (SCREEN_HEIGHT - 280)/2, 180, 280}, selProp, SIZEOF(selProp), &selPropFocus, &selPropScroll);
    
}
