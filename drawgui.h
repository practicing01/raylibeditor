#ifndef DRAWGUI_H
#define DRAWGUI_H

#include "raylib.h"
#include "../raygui-master/src/raygui.h"
#include "../raygui-master/examples/custom_file_dialog/gui_file_dialog.h"

GuiFileDialogState fileDialogState;
char fileNameToLoad[512];
int modelCount;

void DrawGui();
void GetModelNames(char *curDir);

#endif
