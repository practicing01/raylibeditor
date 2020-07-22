#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"
#include "gameplay.h"
#include "drawgui.h"
#include <string.h>
#include "drawnodes.h"

void AddNode()
{
	struct nodeProperties *node = AddNodeProps(nodeTypeActive);
	
	if (nodeTypeActive == NODE)
	{
		struct nodeTypeData *data = (struct nodeTypeData*)( (*node).nodeData );
		(*data).loc = drawNodesCam.position;
	}
	else if (nodeTypeActive == MODEL)
	{
		if (modelListActive != -1)
		{
			struct modelTypeData *data = (struct modelTypeData*)( (*node).nodeData );
			(*data).nodeProps.loc = drawNodesCam.position;
			
			strcpy((*data).filepath, modelFiles[modelListActive]);
			
			(*data).model = LoadModel(modelFiles[modelListActive]);
		}
	}
	
}

struct nodeProperties* AddNodeProps(enum NodeType nodeType)
{
	struct nodeProperties *newNode;
	
	newNode = (struct nodeProperties *)malloc( sizeof(struct nodeProperties) );
	
	(*newNode).nodeType = nodeType;
	
	(*newNode).prev = NULL;
	(*newNode).next = NULL;
	
	if (nodePropListStart == NULL)
	{
		nodePropListStart = newNode;
		nodePropListEnd = newNode;
	}
	else
	{
		(*nodePropListEnd).next = newNode;
		(*newNode).prev = nodePropListEnd;
		nodePropListEnd = newNode;
	}
	
	if (nodeType == NODE)
	{
		(*newNode).nodeData = malloc( sizeof(struct nodeTypeData) );
	}
	else if (nodeType == MODEL)
	{
		(*newNode).nodeData = malloc( sizeof(struct modelTypeData) );
	}
	
	return newNode;
}

void RemoveNodeProps(struct nodeProperties* node)
{
	struct nodeProperties *curNode;
	curNode = nodePropListStart;
	
	while (curNode != NULL)
	{
		if (curNode == node)
		{
			if ( (*curNode).prev != NULL )
			{
				if ( (*curNode).next != NULL )
				{
					struct nodeProperties *prevNode = (*curNode).prev;
					struct nodeProperties *nextNode = (*curNode).next;
					(*prevNode).next = nextNode;
					(*nextNode).prev = prevNode;
				}
				else//this was the end node
				{
					struct nodeProperties *prevNode = (*curNode).prev;
					(*prevNode).next = NULL;
					nodePropListEnd = prevNode;
				}
			}
			else//this was the start node
			{
				if ( (*curNode).next != NULL )
				{
					nodePropListStart = (*curNode).next;
					(*nodePropListStart).prev = NULL;
				}
				else//this was both start and end
				{
					nodePropListStart = NULL;
					nodePropListEnd = NULL;
				}
			}
			
			FreeNodeData(curNode);
			free(curNode);
			break;
		}
		else
		{
			curNode = (*curNode).next;
		}
	}
}

void FreeNodeData(struct nodeProperties *node)
{
	if ( (*node).nodeType == NODE )
	{
		//free data
	}
	else if ((*node).nodeType == MODEL)
	{
		struct modelTypeData *data = (struct modelTypeData*)( (*node).nodeData );
		UnloadModel((*data).model);
	}
	
	free( (*node).nodeData );
}

void FreeNodePropsList()
{
	struct nodeProperties *curNode;
	struct nodeProperties *nextNode;
	curNode = nodePropListStart;
	
	while (curNode != NULL)
	{
		nextNode = (*curNode).next;
		
		FreeNodeData(curNode);
		free(curNode);
		
		curNode = nextNode;
	}
}

void UpdateGuiValues()
{
	if(IsKeyReleased(KEY_ENTER))
	{
		genProps.camLock = genProp[0].value.vbool;
		
		genProps.camSpeed = genProp[1].value.vfloat.val;
		
		genProps.locInt.x = genProp[2].value.v3.x;
		genProps.locInt.y = genProp[2].value.v3.y;
		genProps.locInt.z = genProp[2].value.v3.z;
		
		genProps.rotInt.x = genProp[3].value.v3.x;
		genProps.rotInt.y = genProp[3].value.v3.y;
		genProps.rotInt.z = genProp[3].value.v3.z;
		
		genProps.scaleInt.x = genProp[4].value.v3.x;
		genProps.scaleInt.y = genProp[4].value.v3.y;
		genProps.scaleInt.z = genProp[4].value.v3.z;
		
		drawNodesCam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	}
}

void ToggleCursor()
{
	if(IsMouseButtonReleased(MOUSE_RIGHT_BUTTON))
	{
		if (IsCursorHidden())
		{
			EnableCursor();
			previousMousePosition = GetMousePosition();
		}
		else
		{
			DisableCursor();
			SetMousePosition(previousMousePosition.x, previousMousePosition.y);
		}
	}
}

void GameplayInit()
{
	SetWindowSize(1024, 768);
	
	dt.elapsedTime = 0.0f;
	ModuleLoop = GameplayLoop;
	
	fileDialogState = InitGuiFileDialog(512, 384, GetWorkingDirectory(), false);
	
	GuiLoadStyleDefault();
	GuiSetStyle(LISTVIEW, LIST_ITEMS_HEIGHT, 24);
	GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 12);
	
	modelCount = 0;
	const char *workDir = GetWorkingDirectory();
	char curDir[1024];
	strcpy(curDir, workDir );
	strcat(curDir, "/models");
	GetModelNames(curDir);
	
	drawNodesCam.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    drawNodesCam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    drawNodesCam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    drawNodesCam.fovy = 45.0f;
    drawNodesCam.type = CAMERA_PERSPECTIVE;
    
    SetCameraModeEditor(drawNodesCam, CAMERA_FIRST_PERSON);
    
    SetCameraMoveControls(KEY_W, KEY_S, KEY_D, KEY_A, KEY_E, KEY_Q);
    
    //genProps
    genProps.camLock = false;
		
	genProps.camSpeed = 1.0;
	
	genProps.locInt.x = 0.0f;
	genProps.locInt.y = 0.0f;
	genProps.locInt.z = 0.0f;
	
	genProps.rotInt.x = 0.0f;
	genProps.rotInt.y = 0.0f;
	genProps.rotInt.z = 0.0f;
	
	genProps.scaleInt.x = 0.0f;
	genProps.scaleInt.y = 0.0f;
	genProps.scaleInt.z = 0.0f;
	
	nodePropListStart = NULL;
	nodePropListEnd = NULL;
}

void GameplayExit()
{
	FreeNodePropsList();
}

void GameplayLoop()
{
	if(IsKeyReleased(KEY_SPACE))
	{
		AddNode();
	}
	
	ToggleCursor();
	
	UpdateGuiValues();
	
	if (IsCursorHidden())
	{
		UpdateEditorCamera(&drawNodesCam);
		//UpdateEditorCameraCustom(&drawNodesCam);
	}
	
	BeginDrawing();
		ClearBackground(RAYWHITE);
		
		DrawNodes();
		
		DrawGui();
            
	EndDrawing();
	
	if (WindowShouldClose())
	{
		ExitGame();
	}
}
