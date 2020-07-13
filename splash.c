#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"
#include "splash.h"
#include <stdlib.h> 

void SplashPhase()
{
	if ((* (struct SplashData *)moduleData).splashPhase == 0)
	{
		Image image = LoadImage("splash/raylib_logo.png");
		
		Vector2 scaledDims;
		Vector2 dims;
		dims.x = image.width;
		dims.y = image.height;
		Vector2 curRes;
		curRes.x = GetScreenWidth();
		curRes.y = GetScreenHeight();
		
		ScaleVec2(&scaledDims, &dims, &defaultRes, &curRes);
		
		ImageResize(&image, scaledDims.x, scaledDims.y);
		
		(* (struct SplashData *)moduleData).tex = LoadTextureFromImage(image);
		
		UnloadImage(image);
	}
	else if ((* (struct SplashData *)moduleData).splashPhase == 1)
	{
		UnloadTexture( (* (struct SplashData *)moduleData).tex );
		
		(* (struct SplashData *)moduleData).logo = LoadModel("splash/evitosoft.glb");
		(* (struct SplashData *)moduleData).island = LoadModel("splash/island.glb");
		
		PlayMusicStream((* (struct SplashData *)moduleData).logoMusic);
	}
}

void SplashInit()
{
	dt.elapsedTime = 0.0f;
	
	struct SplashData *data = (struct SplashData *)malloc(sizeof(struct SplashData));
	
	moduleData = data;
	ModuleLoop = SplashLoop;
	
	(*data).camera.position = (Vector3){ 0.0f, 0.0f, 10.0f };
	(*data).camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	(*data).camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	(*data).camera.fovy = 45.0f;
	(*data).camera.type = CAMERA_PERSPECTIVE;
	(*data).logoPos = (Vector3){ 0.0f, 0.0f, 0.0f };
	
	(*data).logoMusic = LoadMusicStream("splash/Start_Sounds_013.ogg");
	SetMusicVolume((*data).logoMusic, 0.5f);
	
	(*data).maxPhases = 2;
	(*data).splashPhase = 0;
	SplashPhase();
}

void SplashExit()
{
	UnloadModel( (* (struct SplashData *)moduleData).logo );
	UnloadModel( (* (struct SplashData *)moduleData).island );
	
	UnloadMusicStream( (* (struct SplashData *)moduleData).logoMusic );
	free( (struct SplashData *)moduleData );
	moduleData = NULL;
}

void SplashLoop()
{	
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	int width = (* (struct SplashData *)moduleData).tex.width;
	int height = (* (struct SplashData *)moduleData).tex.height;
		
	BeginDrawing();	
		
		if ((* (struct SplashData *)moduleData).splashPhase == 0)
		{
			ClearBackground(RAYWHITE);
			
			DrawTexture( (* (struct SplashData *)moduleData).tex ,screenWidth/2 - width/2, screenHeight/2 - height/2, WHITE);
			
			if (dt.elapsedTime >= 3.0f)
			{
				dt.elapsedTime = 0.0f;
				(* (struct SplashData *)moduleData).splashPhase++;
				SplashPhase();
			}
	
		}
		else if ((* (struct SplashData *)moduleData).splashPhase == 1)
		{
			ClearBackground(GOLD);
			
			UpdateCamera(&( (* (struct SplashData *)moduleData).camera ));
			UpdateMusicStream( (* (struct SplashData *)moduleData).logoMusic );
			
			BeginMode3D((* (struct SplashData *)moduleData).camera);
                DrawModel((* (struct SplashData *)moduleData).logo, (* (struct SplashData *)moduleData).logoPos, 1.0f, WHITE);
                DrawModel((* (struct SplashData *)moduleData).island, (* (struct SplashData *)moduleData).logoPos, 1.0f, WHITE);
            EndMode3D();
            
            float timePlayed = GetMusicTimePlayed( (* (struct SplashData *)moduleData).logoMusic );
            float timeLength = GetMusicTimeLength( (* (struct SplashData *)moduleData).logoMusic );
            
            //DrawText(TextFormat("Time played: %f Time Length: %f Division: %f", timePlayed, timeLength, timePlayed/timeLength), 190, 250, 20, BLACK);
            
            if (timePlayed/timeLength >= 0.9f)//Should be 1.0f but that doesn't work for some reason (fps maybe?).
            {
				StopMusicStream( (* (struct SplashData *)moduleData).logoMusic );
				dt.elapsedTime = 0.0f;
				(* (struct SplashData *)moduleData).splashPhase++;
				SplashPhase();
			}
		}
		
	EndDrawing();
	
	if ((* (struct SplashData *)moduleData).splashPhase >= (* (struct SplashData *)moduleData).maxPhases)
	{
		SplashExit();
		SetModule(MENU);
	}
}
