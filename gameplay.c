#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"
#include "gameplay.h"
#include "drawgui.h"
#include <string.h>
#include "drawnodes.h"
#include "raymath.h"

void TranslateNode(struct selectedNode *node, void *param)
{
	struct nodeProperties *nodeProps = (*node).node;
	Vector3 dir = *( (Vector3*) param);
	Vector3 offset = Vector3Multiply(genProps.locInt, dir);
	
	(*nodeProps).loc = Vector3Add((*nodeProps).loc, offset);
}

void ApplyFuncToList( void (*fun)(struct selectedNode *node, void *param), void *param )
{
	struct selectedNode *curNode;
	curNode = selectedNodesListStart;
	
	while (curNode != NULL)
	{
		fun(curNode, param);
		
		curNode = (*curNode).next;
	}
}

void TransformNodes()
{
	transformElapsedTime += dt.deltaTime;
	
	if (transformElapsedTime >= transformInterval)
	{
		transformElapsedTime = 0.0f;
		canTransform = true;
	}
	
	if (!canTransform)
	{
		return;
	}
	
	Vector3 dir = (Vector3){ 0.0f, 0.0f, 0.0f };
	
	if ( IsKeyDown(KEY_U) )//+Z
	{
		canTransform = false;
		
		dir.z = 1.0f;
		
		if( IsKeyDown(KEY_LEFT_CONTROL) )//scale
		{
			//
		}
		else if( IsKeyDown(KEY_LEFT_SHIFT) )//rotate
		{
			//
		}
		else//translate
		{
			ApplyFuncToList( TranslateNode, &dir);
		}
	}
	else if ( IsKeyDown(KEY_J) )//-Z
	{
		canTransform = false;
		
		dir.z = -1.0f;
		
		if( IsKeyDown(KEY_LEFT_CONTROL) )//scale
		{
			//
		}
		else if( IsKeyDown(KEY_LEFT_SHIFT) )//rotate
		{
			//
		}
		else//translate
		{
			ApplyFuncToList( TranslateNode, &dir);
		}
	}
	
	if ( IsKeyDown(KEY_K) )//+X
	{
		canTransform = false;
		
		dir.x = 1.0f;
		
		if( IsKeyDown(KEY_LEFT_CONTROL) )//scale
		{
			//
		}
		else if( IsKeyDown(KEY_LEFT_SHIFT) )//rotate
		{
			//
		}
		else//translate
		{
			ApplyFuncToList( TranslateNode, &dir);
		}
	}
	else if ( IsKeyDown(KEY_H) )//-X
	{
		canTransform = false;
		
		dir.x = -1.0f;
		
		if( IsKeyDown(KEY_LEFT_CONTROL) )//scale
		{
			//
		}
		else if( IsKeyDown(KEY_LEFT_SHIFT) )//rotate
		{
			//
		}
		else//translate
		{
			ApplyFuncToList( TranslateNode, &dir);
		}
	}
	
	if ( IsKeyDown(KEY_I) )//+Y
	{
		canTransform = false;
		
		dir.y = 1.0f;
		
		if( IsKeyDown(KEY_LEFT_CONTROL) )//scale
		{
			//
		}
		else if( IsKeyDown(KEY_LEFT_SHIFT) )//rotate
		{
			//
		}
		else//translate
		{
			ApplyFuncToList( TranslateNode, &dir);
		}
	}
	else if ( IsKeyDown(KEY_Y) )//-Y
	{
		canTransform = false;
		
		dir.y = -1.0f;
		
		if( IsKeyDown(KEY_LEFT_CONTROL) )//scale
		{
			//
		}
		else if( IsKeyDown(KEY_LEFT_SHIFT) )//rotate
		{
			//
		}
		else//translate
		{
			ApplyFuncToList( TranslateNode, &dir);
		}
	}
}

void SelectNode()
{
	if (IsCursorHidden()){return;}
	
	if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
	{
		struct nodeProperties *curNode, *closestNode;
		curNode = nodePropListStart;
		
		float curDist = 1000.0f;
		
		bool hit = false;
		
		float radius = 1.0f;
		Vector3 curLoc;
		
		while (curNode != NULL)
		{
			radius = (*curNode).colScale.x;
			curLoc = (*curNode).loc;
			
			if ( CheckCollisionRaySphere( GetMouseRay(GetMousePosition(), drawNodesCam), curLoc, radius) )
			{
				hit = true;
				
				float dist = Vector3Distance(drawNodesCam.position, curLoc);
				
				if (dist < curDist)
				{
					curDist = dist;
					closestNode = curNode;
				}
			}
			
			curNode = (*curNode).next;
		}
		
		if (hit && closestNode != NULL)
		{			
			if(IsKeyDown(KEY_LEFT_CONTROL))//deselect
			{
				if ( (*closestNode).selected )
				{
					//remove from selection list
					RemoveSelectedNode(closestNode);
				}
			}
			else if(IsKeyDown(KEY_LEFT_SHIFT))//add
			{
				if ( (*closestNode).selected == false)
				{
					//add to selection list
					AddSelectedNode(closestNode);
				}
			}
			else
			{
				//clear selection list and add
				FreeSelectedList();
				AddSelectedNode(closestNode);
			}
		}
	}
}

void AddSelectedNode(struct nodeProperties* node)
{
	(*node).selected = true;
	
	struct selectedNode *newNode;
	
	newNode = (struct selectedNode *)malloc( sizeof(struct selectedNode) );
	
	(*newNode).node = node;
	
	(*newNode).prev = NULL;
	(*newNode).next = NULL;
	
	if (selectedNodesListStart == NULL)
	{
		selectedNodesListStart = newNode;
		selectedNodesListEnd = newNode;
	}
	else
	{
		(*selectedNodesListEnd).next = newNode;
		(*newNode).prev = selectedNodesListEnd;
		selectedNodesListEnd = newNode;
	}
}

void RemoveSelectedNode(struct nodeProperties *node)
{
	(*node).selected = false;
	
	struct selectedNode *curNode;
	curNode = selectedNodesListStart;
	
	while (curNode != NULL)
	{
		if ( (*curNode).node == node)
		{
			if ( (*curNode).prev != NULL )
			{
				if ( (*curNode).next != NULL )
				{
					struct selectedNode *prevNode = (*curNode).prev;
					struct selectedNode *nextNode = (*curNode).next;
					(*prevNode).next = nextNode;
					(*nextNode).prev = prevNode;
				}
				else//this was the end node
				{
					struct selectedNode *prevNode = (*curNode).prev;
					(*prevNode).next = NULL;
					selectedNodesListEnd = prevNode;
				}
			}
			else//this was the start node
			{
				if ( (*curNode).next != NULL )
				{
					selectedNodesListStart = (*curNode).next;
					(*selectedNodesListStart).prev = NULL;
				}
				else//this was both start and end
				{
					selectedNodesListStart = NULL;
					selectedNodesListEnd = NULL;
				}
			}
			
			free(curNode);
			break;
		}
		else
		{
			curNode = (*curNode).next;
		}
	}
}

void AddNode()
{
	struct nodeProperties *node = AddNodeProps(nodeTypeActive);
	
	(*node).selected = false;
	(*node).loc = drawNodesCam.position;
	(*node).rot = Vector3Zero();
	(*node).scale = Vector3One();
	(*node).trigger = false;
	(*node).colScale = Vector3One();
	(*node).colShape = SPHERE;
	(*node).colLayer = 0;
	(*node).LayerCol = 0;
	(*node).visible = true;
	(*node).hidden = false;
	
	if (nodeTypeActive == NODE)
	{
		struct nodeTypeData *data = (struct nodeTypeData*)( (*node).nodeData );
		//
	}
	else if (nodeTypeActive == MODEL)
	{
		if (modelListActive != -1)
		{
			struct modelTypeData *data = (struct modelTypeData*)( (*node).nodeData );
			
			//TraceLog(LOG_INFO, GetWorkingDirectory());
			const char *workDir = GetWorkingDirectory();
			int workDirLen = strlen(workDir);
			char *offset = strstr(modelFiles[modelListActive], workDir);
			
			if (offset != NULL)
			{
				strcpy((*data).filepath, offset + workDirLen + 1);//chop off home path
			}
			else
			{
				strcpy((*data).filepath, modelFiles[modelListActive]);
			}
			
			TraceLog(LOG_INFO, (*data).filepath);
						
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

void FreeSelectedList()
{
	struct selectedNode *curNode;
	struct selectedNode *nextNode;
	curNode = selectedNodesListStart;
	
	while (curNode != NULL)
	{
		struct nodeProperties *node = (*curNode).node;
		(*node).selected = false;
		
		nextNode = (*curNode).next;
		
		free(curNode);
		
		curNode = nextNode;
	}
	
	selectedNodesListStart = NULL;
	selectedNodesListEnd = NULL;
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
	
	genProps.locInt.x = 1.0f;
	genProps.locInt.y = 1.0f;
	genProps.locInt.z = 1.0f;
	
	genProps.rotInt.x = 1.0f;
	genProps.rotInt.y = 1.0f;
	genProps.rotInt.z = 1.0f;
	
	genProps.scaleInt.x = 1.0f;
	genProps.scaleInt.y = 1.0f;
	genProps.scaleInt.z = 1.0f;
	
	nodePropListStart = NULL;
	nodePropListEnd = NULL;
	selectedNodesListStart = NULL;
	selectedNodesListEnd = NULL;
	
	transformElapsedTime = 0.0f;
	transformInterval = 0.25f;
	canTransform = true;
}

void GameplayExit()
{
	FreeNodePropsList();
	FreeSelectedList();
}

void GameplayLoop()
{
	SelectNode();
	
	TransformNodes();
	
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
