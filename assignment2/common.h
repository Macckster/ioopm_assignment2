#pragma once

#define StringValue(x) (elem_t) { .str = x}
#define MerchValue(x) (elem_t) { .merch = x}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/types.h>

#include "hash_table.h"
#include "linked_list.h"

typedef struct list ioopm_list_t; /// Meta: struct definition goes in C file
typedef struct hash_table ioopm_hash_table_t; /// Meta: struct definition goes in C file

typedef enum
{
    IOOPM_ADD = 1,
    IOOPM_REMOVE = 2,
    IOOPM_LIST = 3,
    IOOPM_EDIT = 4,
    IOOPM_STOCK = 5,
    IOOPM_REPLENISH = 6,
    IOOPM_CREATE_CART = 7,
    IOOPM_REMOVE_CART = 8,
    IOOPM_ADD_TOO_CART = 9,
    IOOPM_REMOVE_FROM_CART = 10,
    IOOPM_EDIT_CART = 11,
    IOOPM_CALCULATE_COST = 12,
    IOOPM_CHECKOUT = 13,
    IOOPM_QUIT = 14
} IOOPM_CHOICE;

typedef struct merch
{
    char* name;
    char* desc;
    int price;
    char* location;
    size_t amount;
} merch_t;

typedef struct warehouse
{
    ioopm_hash_table_t* items;
} warehouse_t;

typedef struct cart
{
    ioopm_list_t* list;
}cart_t;


enum IOOPM_RES
{
    IOOPM_SOK        = (0 << 0), // Success
    IOOPM_EINVALKEY  = (1 << 0), // Invalid key
    IOOPM_ENOTFOUND  = (1 << 1), // Hash_table_entry not found for key
    IOOPM_EINVALIDX  = (1 << 2), // Invalid index
    IOOPM_ELISTEMPTY = (1 << 3), // Empty list
    IOOPM_ENONEXT    = (1 << 4), // No next in the iterator
    IOOPM_EUNKNOWN   = (1 << 5), // Unspecified error
};

union element
{
	/// TODO: update the names of the fields to something better? 
	int integer;
	unsigned int u_integer;
	bool boolean;
	float floating_point;
	void* pointer;
	char* str;
	merch_t merch;
	/// other choices certainly possible
};