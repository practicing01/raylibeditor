#include "raylib.h"
#include "moduleLoop.h"
#include "raylibeditor.h"
#include "gameplay.h"
#include "drawgui.h"
#include <string.h>
#include "drawnodes.h"
#include "raymath.h"

void DuplicateSelection()
{
	if ( IsKeyReleased(KEY_V) )
	{
		if ( IsKeyDown(KEY_LEFT_CONTROL) )
		{
			struct selectedNode *curNode;
			curNode = selectedNodesListStart;
			
			while (curNode != NULL)
			{
				struct nodeProperties *node = (*curNode).node;
				
				struct nodeProperties* dupNode = AddNodeProps( (*node).nodeType );
				
				(*dupNode).selected = false;
				(*dupNode).loc = (*node).loc;
				(*dupNode).rot = (*node).rot;
				(*dupNode).scale = (*node).scale;
				(*dupNode).trigger = (*node).trigger;
				(*dupNode).colScale = (*node).colScale;
				(*dupNode).colShape = (*node).colShape;
				(*dupNode).colLayer = (*node).colLayer;
				(*dupNode).LayerCol = (*node).LayerCol;
				(*dupNode).visible = (*node).visible;
				(*dupNode).hidden = (*node).hidden;
				
				if ( (*node).nodeType == NODE)
				{
					struct nodeTypeData *data = (struct nodeTypeData*)( (*node).nodeData );
					struct nodeTypeData *dupData = (struct nodeTypeData*)( (*dupNode).nodeData );
					//
				}
				else if ( (*node).nodeType == MODEL)
				{
					if (modelListActive != -1)
					{
					 	struct modelTypeData *data = (struct modelTypeData*)( (*node).nodeData );
						struct modelTypeData *dupData = (struct modelTypeData*)( (*dupNode).nodeData );
						
						strcpy( (*dupData).filepath, (*data).filepath );
						
						(*dupData).model = LoadModel( (*dupData).filepath );
						
						(*dupData).animated = (*data).animated;
						(*dupData).frames = (*data).frames;
					}
				}
				
				curNode = (*curNode).next;
			}
		}
	}
}

void DeleteSelection()
{
	if ( IsKeyReleased(KEY_DELETE) )
	{
		struct selectedNode *curNode;
		curNode = selectedNodesListStart;
		
		while (curNode != NULL)
		{
			struct nodeProperties *node = (*curNode).node;
			
			RemoveNodeProps(node);
			
			(*curNode).node = NULL;
			
			curNode = (*curNode).next;
		}
		
		FreeSelectedList();
	}
}

void PopulateSelectionProps(struct nodeProperties *node)
{
	selProp[1].value.v3.x = (*node).loc.x;
	selProp[1].value.v3.y = (*node).loc.y;
	selProp[1].value.v3.z = (*node).loc.z;
	
	selProp[2].value.v3.x = (*node).rot.x;
	selProp[2].value.v3.y = (*node).rot.y;
	selProp[2].value.v3.z = (*node).rot.z;
	
	selProp[3].value.v3.x = (*node).scale.x;
	selProp[3].value.v3.y = (*node).scale.y;
	selProp[3].value.v3.z = (*node).scale.z;
	
	selProp[5].value.vbool = (*node).trigger;
	
	selProp[6].value.vselect.active = (*node).colShape;
	
	selProp[7].value.v3.x = (*node).colScale.x;
	selProp[7].value.v3.y = (*node).colScale.y;
	selProp[7].value.v3.z = (*node).colScale.z;
	
	for (int x = 0; x < 32; x++)
	{
		selProp[9 + x].value.vbool = ( (*node).colLayer ) & ( 1 << x );
	}
	
	for (int x = 0; x < 32; x++)
	{
		selProp[42 + x].value.vbool = ( (*node).LayerCol ) & ( 1 << x );
	}
	
	if ( (*node).nodeType == MODEL)
	{
		struct modelTypeData *data = (*node).nodeData;
		
		selProp[75].value.vbool = (*data).animated;
		
		selProp[76].value.v2.x = (*data).frames.x;
		selProp[76].value.v2.y = (*data).frames.y;
	}
	
	selProp[77].value.vbool = (*node).visible;
	
	selProp[78].value.vbool = (*node).hidden;
	
}

void ApplyTransform(struct selectedNode *node, void *param)
{
	struct nodeProperties *nodeProps = (*node).node;
	
	(*nodeProps).loc.x = selProp[1].value.v3.x;
	(*nodeProps).loc.y = selProp[1].value.v3.y;
	(*nodeProps).loc.z = selProp[1].value.v3.z;
	
	(*nodeProps).rot.x = selProp[2].value.v3.x;
	(*nodeProps).rot.y = selProp[2].value.v3.y;
	(*nodeProps).rot.z = selProp[2].value.v3.z;
	
	(*nodeProps).scale.x = selProp[3].value.v3.x;
	(*nodeProps).scale.y = selProp[3].value.v3.y;
	(*nodeProps).scale.z = selProp[3].value.v3.z;
	
	(*nodeProps).trigger = selProp[5].value.vbool;
	
	(*nodeProps).colShape = selProp[6].value.vselect.active;
	
	(*nodeProps).colScale.x = selProp[7].value.v3.x;
	(*nodeProps).colScale.y = selProp[7].value.v3.y;
	(*nodeProps).colScale.z = selProp[7].value.v3.z;
	
	for (int x = 0; x < 32; x++)
	{
		(*nodeProps).colLayer |= ( ( selProp[9 + x].value.vbool ) << x );
	}
	
	for (int x = 0; x < 32; x++)
	{
		(*nodeProps).LayerCol |= ( ( selProp[42 + x].value.vbool ) << x );
	}
	
	if ( (*nodeProps).nodeType == MODEL)
	{
		struct modelTypeData *data = (*nodeProps).nodeData;
		
		(*data).animated = selProp[75].value.vbool;
		
		(*data).frames.x = selProp[76].value.v2.x;
		(*data).frames.y = selProp[76].value.v2.y;
	}
	
	(*nodeProps).visible = selProp[77].value.vbool;
	
	(*nodeProps).hidden = selProp[78].value.vbool;
}

void TranslateNode(struct selectedNode *node, void *param)
{
	struct nodeProperties *nodeProps = (*node).node;
	Vector3 dir = *( (Vector3*) param);
	Vector3 offset = Vector3Multiply(genProps.locInt, dir);
	
	(*nodeProps).loc = Vector3Add((*nodeProps).loc, offset);
}

void RotateNode(struct selectedNode *node, void *param)
{
	struct nodeProperties *nodeProps = (*node).node;
	Vector3 dir = *( (Vector3*) param);
	Vector3 offset = Vector3Multiply(genProps.rotInt, dir);
	
	(*nodeProps).rot = Vector3Add((*nodeProps).rot, offset);
	
	(*nodeProps).rot.x = fmod( (*nodeProps).rot.x, 360.0f);
	(*nodeProps).rot.y = fmod( (*nodeProps).rot.y, 360.0f);
	(*nodeProps).rot.z = fmod( (*nodeProps).rot.z, 360.0f);
}

void ScaleNode(struct selectedNode *node, void *param)
{
	struct nodeProperties *nodeProps = (*node).node;
	Vector3 dir = *( (Vector3*) param);
	Vector3 offset = Vector3Multiply(genProps.scaleInt, dir);
	
	(*nodeProps).scale = Vector3Add((*nodeProps).scale, offset);
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
	if(IsKeyReleased(KEY_ENTER))
	{
		if( IsKeyDown(KEY_LEFT_SHIFT) )
		{
			ApplyFuncToList( ApplyTransform, NULL);
			return;
		}
	}
	
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
	
	if ( IsKeyDown(KEY_I) )//+Z
	{
		canTransform = false;
		
		dir.z = 1.0f;
	}
	else if ( IsKeyDown(KEY_K) )//-Z
	{
		canTransform = false;
		
		dir.z = -1.0f;
	}
	
	if ( IsKeyDown(KEY_L) )//+X
	{
		canTransform = false;
		
		dir.x = 1.0f;
	}
	else if ( IsKeyDown(KEY_J) )//-X
	{
		canTransform = false;
		
		dir.x = -1.0f;
	}
	
	if ( IsKeyDown(KEY_O) )//+Y
	{
		canTransform = false;
		
		dir.y = 1.0f;
	}
	else if ( IsKeyDown(KEY_U) )//-Y
	{
		canTransform = false;
		
		dir.y = -1.0f;
	}
	
	if (!canTransform)
	{
		if( IsKeyDown(KEY_LEFT_CONTROL) )//scale
		{
			ApplyFuncToList( ScaleNode, &dir);
		}
		else if( IsKeyDown(KEY_LEFT_SHIFT) )//rotate
		{
			ApplyFuncToList( RotateNode, &dir);
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
		if(IsKeyDown(KEY_CAPS_LOCK))
		{
			FreeSelectedList();
			return;
		}
		
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
				//populate selection properties
				PopulateSelectionProps(closestNode);
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
			
			(*data).animated = false;
			(*data).frames = Vector2Zero();
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
	
	selectedNodesListStart = NULL;
	selectedNodesListEnd = NULL;
}

void FreeSelectedList()
{
	struct selectedNode *curNode;
	struct selectedNode *nextNode;
	curNode = selectedNodesListStart;
	
	while (curNode != NULL)
	{
		struct nodeProperties *node = (*curNode).node;
		
		if (node != NULL)
		{
			(*node).selected = false;
		}
		
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
	
	DuplicateSelection();
	DeleteSelection();
	
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
