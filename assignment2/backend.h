#pragma once

#include "common.h"

//Reads an integer from stdin
int ReadInt();
//Read string from stdin
char* ReadString();

//Read a shelf from stdin
char* ReadShelf();

warehouse_t* CreateWarehouse();
void DestroyWarehouse(warehouse_t* wh);

void PrintMerch(merch_t m);

void Sort(merch_t* items, size_t size);