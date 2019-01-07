#include "stdafx.h"
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<time.h>
#include "Header.h"
#include "ListHeader.h"

Stack stack;
Numbers NumbersArray[GRID_COUNT][GRID_COUNT];
int UserNumber[GRID_COUNT][GRID_COUNT];
int NumCount[3][GRID_COUNT][GRID_COUNT];

structNode XList[GRID_COUNT];
structNode YList[GRID_COUNT];
structNode NList[GRID_COUNT];
structNode GridList[GRID_COUNT][GRID_COUNT];

bool Click;
int mouseX, mouseY;

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, true);
	createArray();
	createHole();
	saveState();
	initPanel();
	initScene();
	while (true)
	{
		if(MouseHit())
			MouseInput();
		keyInput();
		Sleep(1);
	}
	_getch();
	closegraph();
    return 0;
}
//init operation************************************************************************************************
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

void initNumberState()
{
	for (int i = 0; i<GRID_COUNT; i++)
		for (int j = 0; j < GRID_COUNT; j++)
		{
			NumbersArray[i][j].chosen = false;
		}
}

void initNumCount()
{
	for (int i = 0; i < 3; i++)
		for (int y = 0; y < GRID_COUNT; y++)
			for (int x = 0; x < GRID_COUNT; x++)
				NumCount[i][y][x] = 0;
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

void initGridLists()
{
	for (int i = 0; i<GRID_COUNT; i++)
		for (int j = 0; j < GRID_COUNT; j++)
		{
			GridList[i][j] = createEmptyList();
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

//****************************************************************************************************************

//create sudoku numbers
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
//choose display numers
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
//update numbers state
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
bool checkUseful(int Data, int x, int y)
{
	if (NumCount[0][y][Data - 1] <= 1 && NumCount[1][x][Data - 1] <= 1 && NumCount[2][calculateIndex(x, y)][Data - 1] <= 1)
		return true;
	return false;
}

//input************************************************************************************************************

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
	for (int n = 0; n < GRID_COUNT; n++)
	{
		if ((GetAsyncKeyState(n + 1 + 48) & 1 ) && Click)
		{
			if (UserNumber[mouseY][mouseX] == n + 1)
				return;
			UserNumber[mouseY][mouseX] = n + 1;
			updateState();
			refresh();
			keepHint();
		}
	}
	if (Click && UserNumber[mouseY][mouseX] != 0) {
		if (GetAsyncKeyState('\b') && 0x8000)
		{
			UserNumber[mouseY][mouseX] = 0;
			updateState();
			refresh();
			keepHint();
		}
	}
}

//convenient*******************************************************************************************************
void keepHint()
{
	if (checkUseful(UserNumber[mouseY][mouseX], mouseX, mouseY) && NumbersArray[mouseY][mouseX].chosen)
	{
		DrawGridLineColor(HINT_COLOR, mouseX, mouseY);
		if (UserNumber[mouseY][mouseX] != 0)
			outUserNumbers(mouseX, mouseY);
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
