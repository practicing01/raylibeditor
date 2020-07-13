#ifndef SPLASH_H
#define SPLASH_H

#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"

struct SplashData
{
	Model logo;
	Model island;
	Texture2D tex;
	Camera camera;
	Vector3 logoPos;
	char splashPhase;
	char maxPhases;
	Music logoMusic;
};

void SplashPhase();
void SplashInit();
void SplashExit();
void SplashLoop();

#endif
