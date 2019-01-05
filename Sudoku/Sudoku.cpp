#include "stdafx.h"
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<time.h>

#define GRID_COUNT 9
#define GRID_DISTANCE 60

#define PUSH(x)		stack.Arrays[++stack.top] = x
#define POP			stack.Arrays[stack.top--]

typedef struct StructNode* structNode;
struct StructNode
{
	int data;
	structNode next;
};

typedef struct {
	int Arrays[GRID_COUNT * GRID_COUNT];
	int top;
} Stack;

Stack stack;

int NumbersArray[GRID_COUNT][GRID_COUNT];
structNode XList[GRID_COUNT];
structNode YList[GRID_COUNT];
structNode NList[GRID_COUNT];
structNode GridList[GRID_COUNT][GRID_COUNT];

void initScene();

void initPanel();

structNode createList();
void deleteNode(structNode structList);
structNode mergeList(structNode listX, structNode listY, structNode list);
int getLength(structNode list);
structNode findNodeByData(structNode structList, int Data);
structNode findNodeByIndex(structNode structList, int index);
structNode copyList(structNode structList);
int calculateIndex(int x, int y);
void clearList(structNode structList);
void addNode(structNode structList, int Data);
structNode createEmptyList();
void initGridLists();

void initLists();
void createArray();

int main()
{
	initgraph(900, 600);
	initScene();
	createArray();
	//initPanel();
	_getch();
	closegraph();
    return 0;
}

void initScene()
{
	for (int i = 0; i < GRID_COUNT + 1; i++)
	{
		int y = 30 + i * GRID_DISTANCE;
		line(180, y, 720, y);
	}
	for (int i = 0; i < GRID_COUNT + 1; i++)
	{
		int x = 180 + i * GRID_DISTANCE;
		line(x, 30, x, 570);
	}
}

void initPanel()
{
	for(int y=0;y<GRID_COUNT;y++)
		for (int x = 0; x < GRID_COUNT; x++)
		{
			outtextxy(180 + GRID_DISTANCE / 2 + GRID_DISTANCE * x, 30 + GRID_DISTANCE / 2 + GRID_DISTANCE * y, (char)(NumbersArray[x][y] + 48));
		}
}

//delete the node after the given node
void deleteNode(structNode list)
{
	if (list != NULL)
	{
		structNode temp = list->next;
		list->next = temp->next;
		free(temp);
	}	
}

structNode createList()
{
	structNode node = (structNode)malloc(sizeof(StructNode));
	node->data = 0;
	node->next = NULL;
	structNode first = node;
	for (int i = 1; i <= GRID_COUNT; i++)
	{
		structNode newNode = (structNode)malloc(sizeof(StructNode));
		newNode->data = i;
		newNode->next = NULL;
		node->next = newNode;
		node = newNode;
	}
	return first;
}

int getLength(structNode structList)
{
	int length = 0;
	structNode list = structList;
	while (list->next != NULL)
	{
		length++;
		list = list->next;
	}
	return length;
}
//find the special node before the data, start from the first
structNode findNodeByData(structNode structList, int Data)
{
	structNode list = structList;
	while (list->next != NULL && list->next->data != Data)
	{
		list = list->next;
	}
	if (list->next == NULL)
		return NULL;
	return list;
}
//find the special node correctly
structNode findNodeByIndex(structNode structList, int index)
{
	int i = 0;
	structNode list = structList;
	while (i!=index && list->next!=NULL)
	{
		i++;
		list = list->next;
	}
	return list;
}
structNode mergeList(structNode listX, structNode listY, structNode list)
{
	structNode minList;
	minList = getLength(listX) < getLength(listY) ? listX : listY;
	minList = getLength(minList) < getLength(list) ? minList : list;
	structNode MinList = copyList(minList);
	structNode first = MinList;
	bool sameX = false, sameY = false, same = false;
	if (listX == minList) sameX = true;
	else if (listY == minList) sameY = true;
	else if (list == minList)same = true;
	while (MinList!=NULL && MinList->next != NULL)
	{
		if (!sameX && findNodeByData(listX, MinList->next->data) == NULL)
			deleteNode(MinList);
		else if (!sameY && findNodeByData(listY, MinList->next->data) == NULL)
			deleteNode(MinList);
		else if (!same && findNodeByData(list, MinList->next->data) == NULL)
			deleteNode(MinList);
		else 
			MinList = MinList->next;
	}
	return first;
}

structNode copyList(structNode structList)
{
	structNode list = structList->next;
	structNode newList = (structNode)malloc(sizeof(StructNode));
	newList->data = 0;
	newList->next = NULL;
	structNode first = newList;
	while (list!=NULL)
	{
		structNode temp = (structNode)malloc(sizeof(StructNode));
		temp->data = list->data;
		temp->next = NULL;
		newList->next = temp;
		newList = temp;
		list = list->next;
	}
	return first;
}

int calculateIndex(int x, int y)
{
	int index;
	if (x >= 0 && x < 3 && y >= 0 && y < 3)
		index = 0;
	if (x >= 3 && x < 6 && y >= 0 && y < 3)
		index = 1;
	if (x >= 6 && x < 9 && y >= 0 && y < 3)
		index = 2;
	if (x >= 0 && x < 3 && y >= 3 && y < 6)
		index = 3;
	if (x >= 3 && x < 6 && y >= 3 && y < 6)
		index = 4;
	if (x >= 6 && x < 9 && y >= 3 && y < 6)
		index = 5;
	if (x >= 0 && x < 3 && y >= 6 && y < 9)
		index = 6;
	if (x >= 3 && x < 6 && y >= 6 && y < 9)
		index = 7;
	if (x >= 6 && x < 9 && y >= 6 && y < 9)
		index = 8;
	return index;
}

void initLists()
{
	for (int i = 0; i < GRID_COUNT; i++)
	{
		XList[i] = createList();
		YList[i] = createList();
		NList[i] = createList();
	}
}

void clearList(structNode structList)
{
	while (structList!=NULL)
	{
		structNode temp = structList;
		structList = structList->next;
		free(temp);
	}
}

void addNode(structNode structList, int Data)
{
	structNode list = structList;
	while (list->next!=NULL)
		list = list->next;
	structNode newNode = (structNode)malloc(sizeof(StructNode));
	newNode->data = Data;
	newNode->next = NULL;
	list->next = newNode;
}
structNode createEmptyList()
{
	structNode head = (structNode)malloc(sizeof(StructNode));
	head->data = 0;
	head->next = NULL;
	return head;
}
void initGridLists()
{
	for(int i=0;i<GRID_COUNT;i++)
		for (int j = 0; j < GRID_COUNT; j++)
		{
			GridList[i][j] = createEmptyList();
		}
}
void createArray()
{
	initLists();
	initGridLists();
	srand(time(NULL));
	stack.top = -1;
	int index;
	int specialNodeData;
	settextstyle(25, 0, _T("黑体"));
	for (int y = 0; y < GRID_COUNT; y++)
	{
		for (int x = 0; x < GRID_COUNT; x++)
		{
			int NIndex = calculateIndex(x, y);
			structNode mergedList = mergeList(XList[y], YList[x], NList[NIndex]);
			int length = getLength(mergedList);
			bool flag = true;
Again:		if (length == 0)
			{					
				specialNodeData = POP;
				if (x == 0)
				{
					y--;
					x = 8;
				}
				else
				{
					x--;
				}
				addNode(XList[y], specialNodeData);
				addNode(YList[x], specialNodeData);
				NIndex = calculateIndex(x, y);
				addNode(NList[NIndex], specialNodeData);
				clearList(mergedList);
				mergedList = mergeList(XList[y], YList[x], NList[NIndex]);
				structNode tempList = GridList[y][x];
				while (tempList->next!=NULL)
				{
					deleteNode(findNodeByData(mergedList, tempList->next->data));
					tempList = tempList->next;
				}
				NumbersArray[y][x] = 0;
				outtextxy(172 + GRID_DISTANCE / 2 + GRID_DISTANCE * x, 22 + GRID_DISTANCE / 2 + GRID_DISTANCE * y, (char)(NumbersArray[y][x] + 48));
				Sleep(100);
				length = getLength(mergedList);
				if (length == 0)
					goto Again;
			}
			index = (rand() % length) + 1;
			specialNodeData = findNodeByIndex(mergedList, index)->data;
			clearList(mergedList);
			PUSH(specialNodeData);
			addNode(GridList[y][x], specialNodeData);
			NumbersArray[y][x] = specialNodeData;
			
			deleteNode(findNodeByData(XList[y], specialNodeData));
			deleteNode(findNodeByData(YList[x], specialNodeData));
			deleteNode(findNodeByData(NList[NIndex], specialNodeData));
			outtextxy(172 + GRID_DISTANCE / 2 + GRID_DISTANCE * x, 22 + GRID_DISTANCE / 2 + GRID_DISTANCE * y, (char)(NumbersArray[y][x] + 48));
			Sleep(100);
		}
	}
}
