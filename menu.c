#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"
#include "menu.h"
#include <stdlib.h>
#include <string.h>

void MenuInit()
{	
	dt.elapsedTime = 0.0f;
	
	struct MenuData *data = (struct MenuData *)malloc(sizeof(struct MenuData));
	
	Vector2 scaledDims;
	Vector2 dims;
	dims.x = 100;
	dims.y = 50;
	Vector2 curRes;
	curRes.x = GetScreenWidth();
	curRes.y = GetScreenHeight();
	
	ScaleVec2(&scaledDims, &dims, &defaultRes, &curRes);
	
	(*data).playButt.rect.x = (curRes.x/2) - scaledDims.x/2;
	(*data).playButt.rect.y = (curRes.y/2) - scaledDims.y;
	(*data).playButt.rect.width = scaledDims.x;
	(*data).playButt.rect.height = scaledDims.y;
	
	(*data).exitButt.rect.x = (curRes.x/2) - scaledDims.x/2;
	(*data).exitButt.rect.y = (curRes.y/2);
	(*data).exitButt.rect.width = scaledDims.x;
	(*data).exitButt.rect.height = scaledDims.y;
	
	(*data).playButt.defautFontSize = 20;
	int defaultArea = defaultRes.x * defaultRes.y;
	int curArea = curRes.x * curRes.y;
	(*data).playButt.curFontSize = ( curArea * (*data).playButt.defautFontSize ) / defaultArea;
	
	(*data).exitButt.defautFontSize = 20;
	(*data).exitButt.curFontSize = (*data).playButt.curFontSize;
	
	strcpy( (*data).playButt.text, "Play");
	strcpy( (*data).exitButt.text, "Exit");
	
	int textWidth = MeasureText( (*data).playButt.text, (*data).playButt.curFontSize );
	(*data).playButt.textPos.x = ( (*data).playButt.rect.width/2 ) - (textWidth/2);
	(*data).playButt.textPos.y = ( (*data).playButt.rect.height/2 ) - ( (*data).playButt.curFontSize/2 );
	(*data).playButt.textPos.x += (*data).playButt.rect.x;
	(*data).playButt.textPos.y += (*data).playButt.rect.y;
	
	textWidth = MeasureText( (*data).exitButt.text, (*data).exitButt.curFontSize );
	(*data).exitButt.textPos.x = ( (*data).exitButt.rect.width/2 ) - (textWidth/2 );
	(*data).exitButt.textPos.y = ( (*data).exitButt.rect.height/2 ) - ( (*data).exitButt.curFontSize/2 );
	(*data).exitButt.textPos.x += (*data).exitButt.rect.x;
	(*data).exitButt.textPos.y += (*data).exitButt.rect.y;
	
	moduleData = data;
	ModuleLoop = MenuLoop;
}

void MenuExit()
{
	free( (struct MenuData *)moduleData );
	moduleData = NULL;
}

void MenuLoop()
{
	BeginDrawing();
		ClearBackground(RAYWHITE);
		
		//Play butt
		
		DrawRectangleRec( (* (struct MenuData *)moduleData).playButt.rect , LIGHTGRAY);
		
		if (CheckCollisionPointRec(GetMousePosition(), (* (struct MenuData *)moduleData).playButt.rect) )
		{
			DrawRectangleLines( (* (struct MenuData *)moduleData).playButt.rect.x, (* (struct MenuData *)moduleData).playButt.rect.y,
			(* (struct MenuData *)moduleData).playButt.rect.width, (* (struct MenuData *)moduleData).playButt.rect.height, RED);
			
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				MenuExit();
				SetModule(GAMEPLAY);
				return;
			};
		}
		else
		{
			DrawRectangleLines( (* (struct MenuData *)moduleData).playButt.rect.x, (* (struct MenuData *)moduleData).playButt.rect.y,
			(* (struct MenuData *)moduleData).playButt.rect.width, (* (struct MenuData *)moduleData).playButt.rect.height, DARKGRAY);
		}
		
		DrawText( (* (struct MenuData *)moduleData).playButt.text,
		(* (struct MenuData *)moduleData).playButt.textPos.x,
		(* (struct MenuData *)moduleData).playButt.textPos.y,
		(* (struct MenuData *)moduleData).playButt.curFontSize, MAROON);
		
		//Exit butt
		
		DrawRectangleRec( (* (struct MenuData *)moduleData).exitButt.rect , LIGHTGRAY);
		
		if (CheckCollisionPointRec(GetMousePosition(), (* (struct MenuData *)moduleData).exitButt.rect) )
		{
			DrawRectangleLines( (* (struct MenuData *)moduleData).exitButt.rect.x, (* (struct MenuData *)moduleData).exitButt.rect.y,
			(* (struct MenuData *)moduleData).exitButt.rect.width, (* (struct MenuData *)moduleData).exitButt.rect.height, RED);
			
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				ExitGame();
				return;
			};
		}
		else
		{
			DrawRectangleLines( (* (struct MenuData *)moduleData).exitButt.rect.x, (* (struct MenuData *)moduleData).exitButt.rect.y,
			(* (struct MenuData *)moduleData).exitButt.rect.width, (* (struct MenuData *)moduleData).exitButt.rect.height, DARKGRAY);
		}
		
		DrawText( (* (struct MenuData *)moduleData).exitButt.text,
		(* (struct MenuData *)moduleData).exitButt.textPos.x,
		(* (struct MenuData *)moduleData).exitButt.textPos.y,
		(* (struct MenuData *)moduleData).exitButt.curFontSize, MAROON);
		
	EndDrawing();
}
