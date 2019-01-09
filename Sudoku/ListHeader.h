#pragma once

#define GRID_COUNT 9

typedef struct StructNode* structNode;
struct StructNode
{
	int data;
	structNode next;
};

structNode findNodeByData(structNode structList, int Data);
structNode findNodeByIndex(structNode structList, int index);
structNode copyList(structNode structList);
structNode mergeList(structNode listX, structNode listY, structNode list);

int calculateIndex(int x, int y);
int getLength(structNode list);

void clearList(structNode structList);
void addNode(structNode structList, int Data);
void deleteNode(structNode structList);
void addFirstNode(structNode structList, int Data);
void deleteLastNode(structNode structList);
void addSecondNode(structNode structList, int Data);

structNode createEmptyList();
structNode createList();
