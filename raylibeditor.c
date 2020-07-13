/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples in Notepad++, provided with default raylib installer package, 
*   just press F6 and run [raylib_compile_execute] script, it will compile and execute.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on [C:\raylib\raylib\examples] directory and
*   raylib official webpage: [www.raylib.com]
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raylibeditor.h"
#include "moduleLoop.h"
#include "splash.h"
#include "menu.h"
#include "gameplay.h"

void ExitGame()//todo: free memory from modules on abrupt exits.
{
	canExit = 1;
}

void ScaleVec2(Vector2 *result, Vector2 *point, Vector2 *oldRes, Vector2 *newRes)
{
	int x = ( ((*newRes).x * (*point).x )) / (*oldRes).x;
	int y = ( ((*newRes).y * (*point).y )) / (*oldRes).y;
	
	(*result).x = x;
	(*result).y = y;
}

void SetModule(enum ModulePhase mp)
{
	if (mp == MENU)
	{
		MenuInit();
	}
	else if (mp == GAMEPLAY)
	{
		GameplayInit();
	}
	else if (mp == SPLASH)
	{
		SplashInit();
	}
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    defaultRes.x = 800;
    defaultRes.y = 450;
    
    canExit = 0;
    
    //enum ModulePhase phase = SPLASH;

    InitWindow(defaultRes.x, defaultRes.y, "raylibtest");

    SetTargetFPS(30);               // Set our game to run at 30 frames-per-second
    
    InitAudioDevice();
    //--------------------------------------------------------------------------------------
	
	//SetModule(SPLASH);
	SetModule(GAMEPLAY);//direct for testing
	
    // Main game loop
    while (canExit == 0)
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
		dt.deltaTime = GetFrameTime();
		dt.elapsedTime += dt.deltaTime;
		
        // Draw
        //----------------------------------------------------------------------------------
        ModuleLoop();
        //----------------------------------------------------------------------------------
    }

	CloseAudioDevice();
	CloseWindow();

    return 0;
}
