#pragma once

#include "common.h"

bool merch_compare(elem_t a, elem_t b);

void print_line(char* msg);

//Reads an integer from stdin
int read_int();
//Read string from stdin
char* read_string();

//Read a shelf from stdin
char* read_shelf();

//Read a bool from stdin
bool read_bool();

int read_int_range(int lo, int hi);

warehouse_t* create_warehouse();
void destroy_warehouse(warehouse_t* wh);

void print_merch(merch_t m);

void sort(merch_t* items, size_t size);
void sort_strings(char* keys[], size_t no_keys);

char* ask_user_for_name(warehouse_t* wh);