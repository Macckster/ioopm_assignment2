#include "backend.h"
#include "utils.h"

int HashFunc(elem_t key)
{
    return key.merch.price;
}

bool KeyEquiv(elem_t a, elem_t b)
{
    return !strcmp(a.str, b.str);
}

bool ValEquiv(elem_t key, elem_t val, void* extra)
{
    elem_t next = *((elem_t*)extra);
    
    return !strcmp(val.merch.name, next.merch.name);
}

warehouse_t* CreateWarehouse()
{
    warehouse_t* wh = calloc(1, sizeof(warehouse_t));
    
    if(wh)
    {
        wh->items = ioopm_hash_table_create(HashFunc, KeyEquiv, ValEquiv  );
    }
    
    return wh;
}

void DestroyWarehouse(warehouse_t* wh)
{
    ioopm_hash_table_destroy(wh->items);
    free(wh);
}

int ReadInt()
{
    return ask_question_int("");
}

char* ReadString()
{
    return ask_question_string("");
}

char* ReadShelf()
{
    return ask_question_shelf("");
}

void PrintMerch(merch_t m)
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

void Sort(merch_t* items, size_t size)
{
    qsort(items, size, sizeof(merch_t), merch_comp_func);
}