#include "backend.h"
#include "utils.h"

int HashFunc(elem_t key)
{
    return key.merhandise.price;
}

bool KeyEquiv(elem_t a, elem_t b)
{
    return !strcmp(a.str, b.str);
}

bool ValEquiv(elem_t key, elem_t val, void* extra)
{
    elem_t next = *((elem_t*)extra);
    
    return !strcmp(val.merhandise.name, next.merhandise.name);
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
}

int ReadInt()
{
    return ask_question_int("");
}