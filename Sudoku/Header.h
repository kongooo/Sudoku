#pragma once

#include<graphics.h>
#include<conio.h>

#define GRID_COUNT 9
#define GRID_DISTANCE 60
#define WINDOW_WIDTH		900
#define WINDOW_HEIGHT		750
#define VERTICAL_SPACE		150
#define HORIZONTAL_SAPCE	180
#define WORD_WIDTH			8

#define PUSH(x)		stack.Arrays[++stack.top] = x
#define POP			stack.Arrays[stack.top--]

#define EASY  30

#define BK_COLOR		0xfeffef
#define LINE_COLOR		0xe4e400
#define WORD_COLOR		0xe4e400
#define BLANK_COLOR		WHITE
#define HINT_COLOR		0x5ac9f8
#define WRONG_COLOR		0x5b79f6

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




