#pragma once

#include<graphics.h>
#include<conio.h>

#define EASY	20
#define NORMAL	35
#define HARD	50

#define GRID_COUNT 9
#define GRID_DISTANCE 60
#define WINDOW_WIDTH		900
#define WINDOW_HEIGHT		750
#define VERTICAL_SPACE		150
#define HORIZONTAL_SAPCE	180
#define WORD_WIDTH			8
#define EASY_X_SAPCE		(WINDOW_WIDTH * 4) / 11 + 25
#define	EASY_Y_SAPCE		WINDOW_HEIGHT / 5
#define NORMAL_X_SPACE		(WINDOW_WIDTH * 4) / 11 - 20
#define NORMAL_Y_SPACE		(WINDOW_HEIGHT * 2) / 5
#define HARD_X_SAPCE		(WINDOW_WIDTH * 4) / 11 + 25
#define HARD_Y_SAPCE		(WINDOW_HEIGHT * 3) / 5

#define PUSH(x)		stack.Arrays[++stack.top] = x
#define POP			stack.Arrays[stack.top--]

#define BK_COLOR		0xfeffef
#define LINE_COLOR		0xe4e400
#define WORD_COLOR		0xe4e400
#define BLANK_COLOR		WHITE
#define HINT_COLOR		0x5ac9f8
#define WRONG_COLOR		0x5b79f6
#define STARTBG_COLOR	0xfcf8db
#define STARTTEXT_COLOR 0x98bc00
#define EASY_COLOR		0xd8f200
#define NORMAL_COLOR	0xaccf00
#define HARD_COLOR		0x668c00

#define LINE_BOLD		4

typedef struct {
	int Arrays[GRID_COUNT * GRID_COUNT];
	int top;
} Stack;

typedef struct {
	int Numdata;
	bool chosen;
} Numbers;

void initScene();

void initPanel();

void initGridLists();
void initLists();
void createArray();
void initNumberState();
void createHole();
void refresh();

void initNumCount();
void AddNumCount(int data, int x, int y);

void outUserNumbers(int x, int y);
void DrawGridLineColor(COLORREF color, int x, int y);

void MouseInput();
void keyInput();

void saveState();
void updateState();
bool checkUseful(int Data, int x, int y);
void keepHint();

void initStartPanel();
void initLevelPanel();
void initRankPanel();

void drawLevel(int Xspace, int Yspace, COLORREF textcolor, LPCTSTR level);
void showWinWord();

void clockTime();

void ReadRankToList();
void ReadRankToPanel();
void showRankTime(int seconds, int index);
void WriteRankToFile();
void SaveTimeToList(int thetime);

void FileOperate();


