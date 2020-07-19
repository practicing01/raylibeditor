#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"

void GameplayInit();
void GameplayExit();
void GameplayLoop();
void UpdateGuiValues();
void ToggleCursor();

#endif
