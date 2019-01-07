#include "stdafx.h"
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<time.h>
#include "ListHeader.h"

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
	while (i != index && list->next != NULL)
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
	while (MinList != NULL && MinList->next != NULL)
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
	while (list != NULL)
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

void clearList(structNode structList)
{
	while (structList != NULL)
	{
		structNode temp = structList;
		structList = structList->next;
		free(temp);
	}
}

structNode createEmptyList()
{
	structNode head = (structNode)malloc(sizeof(StructNode));
	head->data = 0;
	head->next = NULL;
	return head;
}

void addNode(structNode structList, int Data)
{
	structNode list = structList;
	while (list->next != NULL)
		list = list->next;
	structNode newNode = (structNode)malloc(sizeof(StructNode));
	newNode->data = Data;
	newNode->next = NULL;
	list->next = newNode;
}

