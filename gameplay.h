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
void SelectNode();

enum NodeType {NODE = 0, MODEL, SPAWN, SFX, GFX, SPRITE, TERRAIN, CAMERA};
enum ColShape {SPHERE = 0, BOX};

void AddNode();

struct generalProperties
{
	bool camLock;
	float camSpeed;
	Vector3 locInt;
	Vector3 rotInt;
	Vector3 scaleInt;
};

struct generalProperties genProps;

struct nodeProperties
{
	enum NodeType nodeType;
	void *nodeData;
	
	Vector3 loc;
	Vector3 rot;
	Vector3 scale;
	bool trigger;
	enum ColShape colShape;
	Vector3 colScale;
	int colLayer;//node belongs to these layers.//1<<x = x position
	int LayerCol;//node collides with these layers.
	bool visible;
	bool hidden;
	bool selected;
	int objectID;
	int childCount;
	int parentCount;
	int misc;
	
	struct nodeProperties *prev;
	struct nodeProperties *next;
};

struct nodeProperties *nodePropListStart;
struct nodeProperties *nodePropListEnd;

struct nodeProperties* AddNodeProps(enum NodeType);
void FreeNodeData(struct nodeProperties*);
void RemoveNodeProps(struct nodeProperties*);
void FreeNodePropsList();

struct nodeTypeData
{
	//
};

struct modelTypeData
{
	char filepath[1024];
	Model model;
	bool animated;
	Vector2 frames;
};

struct selectedNode
{
	struct nodeProperties *node;
	
	struct selectedNode *prev;
	struct selectedNode *next;
};

struct selectedNode *selectedNodesListStart, *selectedNodesListEnd;
void AddSelectedNode(struct nodeProperties* node);
void RemoveSelectedNode(struct nodeProperties *node);
void FreeSelectedList();
void ApplyFuncToList( void (*fun)(struct selectedNode *node, void *param), void *param );
void PopulateSelectionProps(struct nodeProperties *node);
void DuplicateSelection();
void DeleteSelection();

void TransformNodes();
void ApplyTransform(struct selectedNode *node, void *param);
void TranslateNode(struct selectedNode *node, void *param);
void RotateNode(struct selectedNode *node, void *param);
void ScaleNode(struct selectedNode *node, void *param);
float transformElapsedTime;
float transformInterval;
bool canTransform;

int objectIDCounter;
int nodeCount;
void SaveScene();//todo add filename to general properties list
void LoadScene();

#endif
