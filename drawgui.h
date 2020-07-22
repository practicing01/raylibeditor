#ifndef DRAWGUI_H
#define DRAWGUI_H

#include "raylib.h"
#include "../raygui-master/src/raygui.h"
#include "../raygui-master/examples/custom_file_dialog/gui_file_dialog.h"
#include "../raygui-master/examples/property_list/dm_property_list.h"

GuiFileDialogState fileDialogState;
char fileNameToLoad[512];
int modelCount;
extern int nodeTypeActive;
extern int modelListActive;
extern char modelFiles[1024][1024];

extern GuiDMProperty genProp[];
extern GuiDMProperty selProp[];

void DrawGui();
void GetModelNames(char *curDir);

#endif
