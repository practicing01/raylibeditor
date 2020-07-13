#ifndef MODULELOOP_H
#define MODULELOOP_H

#include "raylib.h"

struct DeltaTime
{
	float deltaTime;
	float elapsedTime;
};

struct DeltaTime dt;
Vector2 defaultRes;

void *moduleData;
void (*ModuleLoop)(void);

void ScaleVec2(Vector2 *result, Vector2 *point, Vector2 *oldRes, Vector2 *newRes);
void ExitGame();

char canExit;

#endif
