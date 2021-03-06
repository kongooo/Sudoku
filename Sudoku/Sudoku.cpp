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

structNode RankList;

bool Click;
bool start = false;
bool win = false;
int mouseX, mouseY;
int level;
int timer;

time_t Tstart, Tend;
time_t Sstart, Send;
bool STime = true;

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, true);
	initStartPanel();
	initLevelPanel();
	
	createArray();
	createHole();
	saveState();
	initPanel();
	initScene();

	Tstart = time(NULL);

	while (true)
	{			
		if (GetAsyncKeyState(VK_ESCAPE))
			exit(0);
		if (MouseHit())
			MouseInput();
		keyInput();
		if (STime)
		{
			Sstart = time(NULL);
			clockTime();
			Sleep(1);
			STime = false;
		}
		Send = time(NULL);
		if ((int)(difftime(Send, Sstart)) >= 1)
			STime = true;
		if (win)
			break;
	}
	showWinWord();
	FileOperate();
	while (!GetAsyncKeyState(VK_RETURN))
	{
		Sleep(1);
	}
	initRankPanel();
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		Sleep(1);
	}
	closegraph();
    return 0;
}
//init operation**************************************
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
	clearrectangle(HORIZONTAL_SAPCE, VERTICAL_SPACE, HORIZONTAL_SAPCE + GRID_COUNT * GRID_DISTANCE, VERTICAL_SPACE + GRID_COUNT * GRID_DISTANCE);
	settextstyle(25, 0, _T("黑体"), 1, 1, FW_HEAVY, false, false, false);
	win = true;
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
				win = false;
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
				win = false;
				DrawGridLineColor(WRONG_COLOR, x, y);
				outUserNumbers(x, y);
			}
		}
}

//****************************************************

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
	for (int x = 0; x < GRID_COUNT; x++)
		for (int y = 0; y < GRID_COUNT; y++)
			clearList(GridList[y][x]);
}
//choose display numers
void createHole()
{
	int RandomX, RandomY;
	for (int i = 0; i < level; i++)
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

//input***********************************************

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
			if(!win)
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

//convenient******************************************
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

void initStartPanel()
{
	setbkcolor(STARTBG_COLOR);
	cleardevice();
	settextstyle(190, 90, _T("Comic Sans MS"), 1, 1, 1300, false, false, false);
	settextcolor(STARTTEXT_COLOR);
	outtextxy(WINDOW_WIDTH / 6, WINDOW_HEIGHT / 6, _T("Sudoku"));
	settextstyle(35, 15, _T("Comic Sans MS"), 1, 1, FW_LIGHT, false, false, false);
	settextcolor(STARTTEXT_COLOR);
	outtextxy(WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2, _T("Press space to start."));
	while (!GetAsyncKeyState(VK_SPACE))
	{
		GetAsyncKeyState(VK_RETURN);
		Sleep(1);
	}
}

void initLevelPanel()
{
	LPCTSTR easy = _T("Easy");
	LPCTSTR normal = _T("Normal");
	LPCTSTR hard = _T("Hard");
	int EasyW, EasyH, NormalW, NormalH, HardW, HardH;
	
	setbkcolor(STARTBG_COLOR);
	cleardevice();
	drawLevel(EASY_X_SAPCE, EASY_Y_SAPCE, EASY_COLOR, easy);
	drawLevel(NORMAL_X_SPACE, NORMAL_Y_SPACE, NORMAL_COLOR, normal);
	drawLevel(HARD_X_SAPCE, HARD_Y_SAPCE, HARD_COLOR, hard);

	EasyW = textwidth(easy) + EASY_X_SAPCE;
	EasyH = textheight(easy) + EASY_Y_SAPCE;
	NormalW = textwidth(normal) + NORMAL_X_SPACE;
	NormalH = textheight(normal) + NORMAL_Y_SPACE;
	HardW = textwidth(hard) + HARD_X_SAPCE;
	HardH = textheight(hard) + HARD_Y_SAPCE;

	while (!start)
	{
		MOUSEMSG m = GetMouseMsg();
		if (m.mkLButton)
		{
			if (m.x >= EASY_X_SAPCE && m.x <= EasyW && m.y >= EASY_Y_SAPCE && m.y <= EasyH)
			{
				start = true;
				level = EASY;
			}
			if (m.x >= NORMAL_X_SPACE && m.x <= NormalW && m.y >= NORMAL_Y_SPACE && m.y <= NormalH)
			{
				start = true;
				level = NORMAL;
			}
			if (m.x >= HARD_X_SAPCE && m.x <= HardW && m.y >= HARD_Y_SAPCE && m.y <= HardH)
			{
				start = true;
				level = HARD;
			}
		}
		//Sleep(1);
	}
}

void drawLevel(int Xspace, int Yspace, COLORREF textcolor, LPCTSTR level)
{
	settextstyle(100, 40, _T("Comic Sans MS"), 1, 1, FW_BOLD, false, false, false);
	settextcolor(textcolor);
	outtextxy(Xspace, Yspace, level);
}

void clockTime()
{
	Tend = time(NULL);
	int hl = 0, hr = 0, ml = 0, mr = 0, sl = 0, sr = 0;
	timer = difftime(Tend, Tstart);

	hl = (timer / 3600) / 10;
	hr = (timer / 3600) % 10;
	ml = ((timer % 3600) / 60) / 10;
	mr = ((timer % 3600) / 60) % 10;
	sl = ((timer % 3600) % 60) / 10;
	sr = ((timer % 3600) % 60) % 10;
	
	settextcolor(WORD_COLOR);
	settextstyle(40, 0, _T("Comic Sans MS"), 1, 1, 0, false, false, false);
	setbkcolor(BK_COLOR);
	clearrectangle(310, 50, 630, 90);
	outtextxy(310, 50, (char)(hl + 48));
	outtextxy(350, 50, (char)(hr + 48));
	outtextxy(390, 50, ':');
	outtextxy(430, 50, (char)(ml + 48));
	outtextxy(470, 50, (char)(mr + 48));
	outtextxy(510, 50, ':');
	outtextxy(550, 50, (char)(sl + 48));
	outtextxy(590, 50, (char)(sr + 48));
}

void showWinWord()
{
	LPCTSTR winWord = _T("Win!  Press Enter to continue.");
	clearrectangle(310, 50, 630, 90);
	settextstyle(40, 0, _T("Comic Sans MS"), 1, 1, 0, false, false, false);
	setbkcolor(BK_COLOR);
	outtextxy((WINDOW_WIDTH - textwidth(winWord)) / 2, 50, winWord);
}

void FileOperate()
{
	ReadRankToList();
	SaveTimeToList(timer);
	WriteRankToFile();
}

void initRankPanel()
{
	setbkcolor(BK_COLOR);
	cleardevice();
	settextstyle(100, 0, _T("Comic Sans MS"), 1, 1, FW_BOLD, false, false, false);
	settextcolor(STARTTEXT_COLOR);
	outtextxy(WINDOW_WIDTH / 4 + 35, 20, _T("Ranking List"));
	ReadRankToPanel();
}

void ReadRankToList()
{
	RankList = createEmptyList();
	FILE* RankFile;
	fopen_s(&RankFile, "RankData.txt", "r+");
	if (RankFile == NULL)
	{
		printf("can't open file\n");
		exit(0);
	}
	for (int i = 0; i < 10; i++)
	{
		int data = 0;
		fscanf_s(RankFile, "%d", &data);
		addNode(RankList, data);
	}
	fclose(RankFile);
}

void ReadRankToPanel()
{
	structNode list = RankList->next;
	settextstyle(40, 0, _T("Comic Sans MS"), 1, 1, 0, false, false, false);
	setbkcolor(BK_COLOR);
	settextcolor(HINT_COLOR);
	showRankTime(list->data, 0);
	list = list->next;
	settextcolor(WORD_COLOR);
	for (int i = 1; i < 10; i++)
	{
		if (list->data != 0)
			showRankTime(list->data, i);
		if (list != NULL)
			list = list->next;
	}
}

void showRankTime(int seconds, int index)
{
	int hl = 0, hr = 0, ml = 0, mr = 0, sl = 0, sr = 0;
	int height = 140 + index * 50;

	hl = (seconds / 3600) / 10;
	hr = (seconds / 3600) % 10;
	ml = ((seconds % 3600) / 60) / 10;
	mr = ((seconds % 3600) / 60) % 10;
	sl = ((seconds % 3600) % 60) / 10;
	sr = ((seconds % 3600) % 60) % 10;

	outtextxy(310, height, (char)(hl + 48));
	outtextxy(350, height, (char)(hr + 48));
	outtextxy(390, height, ':');
	outtextxy(430, height, (char)(ml + 48));
	outtextxy(470, height, (char)(mr + 48));
	outtextxy(510, height, ':');
	outtextxy(550, height, (char)(sl + 48));
	outtextxy(590, height, (char)(sr + 48));
}

void WriteRankToFile()
{
	structNode list = RankList->next;
	FILE* RankFile;
	fopen_s(&RankFile, "RankData.txt", "w");
	
	if (RankFile == NULL)
	{
		printf("fail to open the file\n");
		exit(0);
	}
	for (int i = 0; i < 10; i++)
	{
		fprintf_s(RankFile, "%d\n", list->data);
		list = list->next;
	}

	fclose(RankFile);
}

void SaveTimeToList(int thetime)
{
	if (RankList->next->data == 0 || thetime < RankList->next->data)
		addFirstNode(RankList, thetime);
	else if(thetime >= RankList->next->data)
		addSecondNode(RankList, thetime);
	deleteLastNode(RankList);
}

