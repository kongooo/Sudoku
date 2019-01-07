#include "stdafx.h"
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<time.h>

#define GRID_COUNT 9
#define GRID_DISTANCE 60
#define WINDOW_WIDTH		900
#define WINDOW_HEIGHT		750
#define VERTICAL_SPACE		150
#define HORIZONTAL_SAPCE	180
#define WORD_WIDTH			8

#define PUSH(x)		stack.Arrays[++stack.top] = x
#define POP			stack.Arrays[stack.top--]

#define EASY  20

#define BK_COLOR		0xfeffef
#define LINE_COLOR		0xe4e400
#define WORD_COLOR		0xe4e400
#define BLANK_COLOR		WHITE
#define HINT_COLOR		0x5ac9f8
#define WRONG_COLOR		0x5b79f6

#define LINE_BOLD		4

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

typedef struct {
	int Numdata;
	bool chosen;
} Numbers;

Numbers NumbersArray[GRID_COUNT][GRID_COUNT];
int UserNumber[GRID_COUNT][GRID_COUNT];
structNode XList[GRID_COUNT];
structNode YList[GRID_COUNT];
structNode NList[GRID_COUNT];
structNode GridList[GRID_COUNT][GRID_COUNT];
int NumCount[3][GRID_COUNT][GRID_COUNT];

bool Click = false;
int mouseX, mouseY;
int CurrentNumber;

void initScene();

void initPanel();

structNode findNodeByData(structNode structList, int Data);
structNode findNodeByIndex(structNode structList, int index);
structNode copyList(structNode structList);
structNode mergeList(structNode listX, structNode listY, structNode list);
int calculateIndex(int x, int y);
int getLength(structNode list);
void clearList(structNode structList);
void addNode(structNode structList, int Data);
void deleteNode(structNode structList);

structNode createEmptyList();
structNode createList();
void initGridLists();
void initLists();
void createArray();
void initNumberState();
void createHole();
void refresh();

void updateState();

void initNumCount();
void AddNumCount(int data, int x, int y);

void outUserNumbers(int x, int y);
void DrawGridLineColor(COLORREF color, int x, int y);

void MouseInput();
void keyInput();

void saveState();
bool checkUseful(int Data, int x, int y);

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, true);
	createArray();
	createHole();
	saveState();
	for (int i = 0; i < GRID_COUNT; i++)
	{
		structNode templist = XList[i];
		while (templist->next!=NULL)
		{
			printf("%d ",templist->next->data);
			templist = templist->next;
		}
		printf("\n");
	}
	initPanel();
	initScene();
	while (true)
	{
		MouseInput();
		keyInput();
	}
	_getch();
	closegraph();
    return 0;
}

void initScene()
{
	setlinecolor(LINE_COLOR);
	for (int i = 0; i < GRID_COUNT + 1; i++)
	{
		int y = VERTICAL_SPACE + i * GRID_DISTANCE;
		if (i == 0 || i == GRID_COUNT || i == 3 || i == 6)
			setlinestyle(PS_SOLID, LINE_BOLD, NULL, 0);
		else 
			setlinestyle(PS_SOLID, 1, NULL, 0);
		line(HORIZONTAL_SAPCE, y, GRID_COUNT * GRID_DISTANCE + HORIZONTAL_SAPCE, y);
	}
	for (int i = 0; i < GRID_COUNT + 1; i++)
	{
		int x = HORIZONTAL_SAPCE + i * GRID_DISTANCE;
		if (i == 0 || i == GRID_COUNT || i == 3 || i == 6)
			setlinestyle(PS_SOLID, LINE_BOLD, NULL, 0);
		else
			setlinestyle(PS_SOLID, 1, NULL, 0);
		line(x, VERTICAL_SPACE, x, GRID_COUNT * GRID_DISTANCE + VERTICAL_SPACE);
	}
}

void initPanel()
{
	setbkcolor(BK_COLOR);
	cleardevice();
	settextstyle(25, 0, _T("黑体"), 0, 0, FW_HEAVY, false, false, false);
	settextcolor(WORD_COLOR);
	for(int y = 0;y < GRID_COUNT ; y++)
		for (int x = 0; x < GRID_COUNT; x++)
		{
			if (!NumbersArray[y][x].chosen)
			{
				UserNumber[y][x] = NumbersArray[y][x].Numdata;
				outtextxy(HORIZONTAL_SAPCE - WORD_WIDTH + GRID_DISTANCE / 2 + GRID_DISTANCE * x, VERTICAL_SPACE - WORD_WIDTH + GRID_DISTANCE / 2 + GRID_DISTANCE * y, (char)(NumbersArray[y][x].Numdata + 48));
			}			
			else
			{
				UserNumber[y][x] = 0;
				setfillcolor(BLANK_COLOR);
				solidrectangle(HORIZONTAL_SAPCE + x * GRID_DISTANCE, VERTICAL_SPACE + y * GRID_DISTANCE, HORIZONTAL_SAPCE + GRID_DISTANCE + x * GRID_DISTANCE, VERTICAL_SPACE + GRID_DISTANCE + y * GRID_DISTANCE);
			}				
		}
}
void refresh()
{
	setbkcolor(BK_COLOR);
	cleardevice();
	for (int y = 0; y < GRID_COUNT; y++)
		for (int x = 0; x < GRID_COUNT; x++)
		{
			if (UserNumber[y][x] != 0)
			{
				if (NumbersArray[y][x].chosen)
				{
					solidrectangle(HORIZONTAL_SAPCE + x * GRID_DISTANCE, VERTICAL_SPACE + y * GRID_DISTANCE, HORIZONTAL_SAPCE + GRID_DISTANCE + x * GRID_DISTANCE, VERTICAL_SPACE + GRID_DISTANCE + y * GRID_DISTANCE);
					outUserNumbers(x, y);
				}
				else
				{
					setbkcolor(BK_COLOR);
					outtextxy(HORIZONTAL_SAPCE - WORD_WIDTH + GRID_DISTANCE / 2 + GRID_DISTANCE * x, VERTICAL_SPACE - WORD_WIDTH + GRID_DISTANCE / 2 + GRID_DISTANCE * y, (char)(NumbersArray[y][x].Numdata + 48));
				}					
			}				
			else
			{
				setfillcolor(BLANK_COLOR);
				setlinestyle(PS_SOLID, 1, NULL, 0);
				solidrectangle(HORIZONTAL_SAPCE + x * GRID_DISTANCE, VERTICAL_SPACE + y * GRID_DISTANCE, HORIZONTAL_SAPCE + GRID_DISTANCE + x * GRID_DISTANCE, VERTICAL_SPACE + GRID_DISTANCE + y * GRID_DISTANCE);
			}
		}
	initScene();
	for (int y = 0; y<GRID_COUNT; y++)
		for (int x = 0; x < GRID_COUNT; x++)
		{
			if (UserNumber[y][x] != 0 && NumbersArray[y][x].chosen && !checkUseful(UserNumber[y][x], x, y))
			{
				DrawGridLineColor(WRONG_COLOR, x, y);
				outUserNumbers(x, y);
			}
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
void initNumberState()
{
	for (int i = 0; i<GRID_COUNT; i++)
		for (int j = 0; j < GRID_COUNT; j++)
		{
			NumbersArray[i][j].chosen = false;
		}
}
void createArray()
{
	initLists();
	initGridLists();
	initNumberState();
	srand(time(NULL));
	stack.top = -1;
	int index;
	int specialNodeData;
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
				NumbersArray[y][x].Numdata = 0;
				//outtextxy(172 + GRID_DISTANCE / 2 + GRID_DISTANCE * x, 22 + GRID_DISTANCE / 2 + GRID_DISTANCE * y, (char)(NumbersArray[y][x].Numdata + 48));
				//Sleep(100);
				length = getLength(mergedList);
				if (length == 0)
					goto Again;
			}
			index = (rand() % length) + 1;
			specialNodeData = findNodeByIndex(mergedList, index)->data;
			clearList(mergedList);
			PUSH(specialNodeData);
			addNode(GridList[y][x], specialNodeData);
			NumbersArray[y][x].Numdata = specialNodeData;
			
			deleteNode(findNodeByData(XList[y], specialNodeData));
			deleteNode(findNodeByData(YList[x], specialNodeData));
			deleteNode(findNodeByData(NList[NIndex], specialNodeData));
			//outtextxy(172 + GRID_DISTANCE / 2 + GRID_DISTANCE * x, 22 + GRID_DISTANCE / 2 + GRID_DISTANCE * y, (char)(NumbersArray[y][x].Numdata + 48));
			//Sleep(100);
		}
	}
}

void createHole()
{
	int RandomX, RandomY;
	for (int i = 0; i < EASY; i++)
	{
RAND:	RandomX = rand() % GRID_COUNT ;
		RandomY = rand() % GRID_COUNT ;
		if (NumbersArray[RandomY][RandomX].chosen)
			goto RAND;
		else
		{
			NumbersArray[RandomY][RandomX].chosen = true;
		}
	}
}

void saveState()
{
	initNumCount();
	for (int y = 0; y < GRID_COUNT; y++)
		for (int x = 0; x < GRID_COUNT; x++)
			if (!NumbersArray[y][x].chosen)
			{
				AddNumCount(NumbersArray[y][x].Numdata, x, y);
				UserNumber[y][x] = NumbersArray[y][x].Numdata;
			}
			else
				UserNumber[y][x] = 0;
}
void updateState()
{
	initNumCount();
	for (int y = 0; y < GRID_COUNT; y++)
		for (int x = 0; x < GRID_COUNT; x++)
			if (UserNumber[y][x] != 0)
				AddNumCount(UserNumber[y][x], x, y);
}
void AddNumCount(int data, int x, int y)
{
	NumCount[0][y][data - 1]++;
	NumCount[1][x][data - 1]++;
	NumCount[2][calculateIndex(x, y)][data - 1]++;
}
void initNumCount()
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < GRID_COUNT; y++)
			for (int x = 0; x < GRID_COUNT; x++)
				NumCount[i][y][x] = 0;
}
bool checkUseful(int Data, int x, int y)
{
	if (NumCount[0][y][Data - 1] <= 1 && NumCount[1][x][Data - 1] <= 1 && NumCount[2][calculateIndex(x, y)][Data - 1] <= 1)
		return true;
	return false;
}

void MouseInput()
{
	MOUSEMSG mouse = GetMouseMsg();
	if (mouse.mkLButton && mouse.x >= HORIZONTAL_SAPCE && mouse.x <= HORIZONTAL_SAPCE + GRID_COUNT * GRID_DISTANCE && mouse.y >= VERTICAL_SPACE && mouse.y <= VERTICAL_SPACE + GRID_COUNT * GRID_DISTANCE) 
	{
		refresh();
		mouseX = (mouse.x - HORIZONTAL_SAPCE) / GRID_DISTANCE;
		mouseY = (mouse.y - VERTICAL_SPACE) / GRID_DISTANCE;
		if (NumbersArray[mouseY][mouseX].chosen)
		{
			DrawGridLineColor(HINT_COLOR, mouseX, mouseY);
			if (UserNumber[mouseY][mouseX] != 0)
				outUserNumbers(mouseX, mouseY);
			Click = true;
		}
		else
			Click = false;
	}
}

void keyInput()
{
	if (Click)
	{
		for (int n = 0; n < GRID_COUNT; n++)
		{
			if (GetAsyncKeyState(n + 1 + 48) && 0x8000)
			{				
				if (UserNumber[mouseY][mouseX] == n + 1)
					return;
				UserNumber[mouseY][mouseX] = n + 1;
				updateState();
				refresh();
			}
		}
	}
}

void outUserNumbers(int x, int y)	
{
	setbkcolor(BLANK_COLOR);
	outtextxy(HORIZONTAL_SAPCE - WORD_WIDTH + GRID_DISTANCE / 2 + GRID_DISTANCE * x, VERTICAL_SPACE - WORD_WIDTH + GRID_DISTANCE / 2 + GRID_DISTANCE * y, (char)(UserNumber[y][x] + 48));
}

void DrawGridLineColor(COLORREF color, int x, int y)
{
	setfillcolor(BLANK_COLOR);
	setlinecolor(color);
	setlinestyle(PS_JOIN_MITER, 3, NULL, 0);
	fillrectangle(HORIZONTAL_SAPCE + x * GRID_DISTANCE, VERTICAL_SPACE + y * GRID_DISTANCE, HORIZONTAL_SAPCE + GRID_DISTANCE + x * GRID_DISTANCE, VERTICAL_SPACE + GRID_DISTANCE + y * GRID_DISTANCE);
}
