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

enum NodeType {NODE, MODEL};
enum ColShape {SPHERE, BOX};

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
	
	struct nodeProperties *prev;
	struct nodeProperties *next;
};

struct nodeProperties *nodePropListStart;
struct nodeProperties *nodePropListEnd;

struct nodeProperties* addNodeProps(enum NodeType);
void removeNodeProps(struct nodeProperties*);
void freeNodePropsList();

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
	bool animated;
	Vector2 frames;
};

#endif
