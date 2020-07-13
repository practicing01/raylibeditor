#ifndef RAYLIBEDITOR_H
#define RAYLIBEDITOR_H

#include "raylib.h"

enum ModulePhase {SPLASH, MENU, GAMEPLAY};

void SetModule(enum ModulePhase);

#endif
