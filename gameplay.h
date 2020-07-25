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
enum ColShape {SPHERE, BOX};

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
	bool selected;
	
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
};

struct modelTypeData
{
	struct nodeTypeData nodeProps;
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

#endif
