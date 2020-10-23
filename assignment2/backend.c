#include "backend.h"
#include "utils.h"

int hash_func(elem_t key)
{
    return key.merch.price;
}

bool key_equiv(elem_t a, elem_t b)
{
    return !strcmp(a.str, b.str);
}

bool val_equiv(elem_t key, elem_t val, void* extra)
{
    elem_t next = *((elem_t*)extra);
    
    return !strcmp(val.merch.name, next.merch.name);
}

bool merch_compare(elem_t a, elem_t b)
{
    if(a.merch.name && b.merch.name)
        return !strcmp(a.merch.name, b.merch.name);
    else
        return false;
}

warehouse_t* create_warehouse()
{
    warehouse_t* wh = calloc(1, sizeof(warehouse_t));
    
    if(wh)
    {
        wh->items = ioopm_hash_table_create(hash_func, key_equiv, val_equiv);
    }
    
    return wh;
}

void destroy_warehouse(warehouse_t* wh)
{
    ioopm_hash_table_destroy(wh->items);
    free(wh);
}

int read_int()
{
    return ask_question_int("");
}

char* read_string()
{
    return ask_question_string("");
}

char* read_shelf()
{
    return ask_question_shelf("");
}

bool read_bool()
{
    return ask_question_bool("");
}

int read_int_range(int lo, int hi)
{
    int choice = 0;
    
    do
    {
        choice = ask_question_int("");
    } 
    while(choice < lo || choice > hi);
    
    return choice;
}

void print_merch(merch_t m)
{
    printf("Name: %s\nDesc: %s\nPrice: %d\nShelf: %s\n", m.name, m.desc, m.price, m.location);
}

int merch_comp_func(const void* ptrA, const void* ptrB)
{
    merch_t a = *((merch_t*)ptrA);
    merch_t b = *((merch_t*)ptrB);
    
    //TODO: REFACTOR THIS IS GARBO
    
    char aisleA = a.location[0];
    char aisleB = b.location[0];
    
    int shelfA = atoi(a.location + sizeof(char));
    int shelfB = atoi(a.location + sizeof(char));
    
    if(aisleA == aisleB)
    {
        if(shelfA == shelfB)
            return 0;
        else if (shelfA > shelfB)
            return -1;
        else
            return 1;
    }
    else
    {
        return aisleA > aisleB ? -1 : 1;
    }
}

char* ask_user_for_name(warehouse_t* wh)
{
    size_t size = ioopm_hash_table_size(wh->items);
    
    if(size == 0)
    {
        print_line("You have no items");
        return "";
    }
    
    ioopm_list_t* keys = ioopm_hash_table_keys(wh->items);
    
    char* arrkeys[size];
    
    for (size_t i = 0; i < size; i++) 
    {
        elem_t tmp;
        ioopm_linked_list_get(keys, i, &tmp);
        
        arrkeys[i] = tmp.str;
    }
    
    sort_strings(arrkeys, size);
    
    for (int i = 0; i < size; i++) 
    {
        printf("(%d) %s\n", i, arrkeys[i]);
    }
    
    print_line("Which item do you want to remove");
    
    int idx = -1;
    while(idx < 0 || idx >= size)
    {
        idx = read_int();
    }
    
    return arrkeys[idx];
}

void print_line(char* msg)
{
    printf("%s\n", msg);
}

void sort(merch_t* items, size_t size)
{
    qsort(items, size, sizeof(merch_t), merch_comp_func);
}

static int cmpstringp(const void* p1, const void* p2)
{
	return strcmp(*(char* const*)p1, *(char* const*)p2);
}

void sort_strings(char* keys[], size_t no_keys)
{
	qsort(keys, no_keys, sizeof(char*), cmpstringp);
}