#include "linked_list.h"
#include "iterator.h"

#define FREE(x) { free(x); x = NULL; }

struct list_entry
{
    elem_t val;
    list_entry_t* next;
    list_entry_t* prev;
};

struct list
{
    size_t size;
    list_entry_t* sentinel;
    list_entry_t* last;

    ioopm_eq_function eqfunc;
};

struct iterator
{
    ioopm_list_t* list;
    list_entry_t* current;
    int idx;
};

typedef struct pair
{
    ioopm_eq_function func;
    elem_t val;
} pair_t;

list_entry_t* entry_new(elem_t element, list_entry_t* next, list_entry_t* prev)
{
    list_entry_t* result = malloc(sizeof(list_entry_t));
    
    if(result)
    {
        result->val = element;
        result->next = next;
        result->prev = prev;
    }
    
    return result;
}

list_entry_t* get_entry_from_index(ioopm_list_t* list, size_t idx)
{
    size_t cur_idx = 0;
    list_entry_t* e = list->sentinel->next;

    while (e) 
    {
        if (idx == cur_idx) 
        {
            return e;
        }
        
        e = e->next;
        cur_idx++;
    }
    
    return NULL;
}

ioopm_list_t* ioopm_linked_list_create(ioopm_eq_function eqfunc)
{
    ioopm_list_t* ls = calloc(1, sizeof(ioopm_list_t));
    list_entry_t* sentinel = calloc(1, sizeof(list_entry_t));

    if (ls)
    {
        ls->sentinel = sentinel;
        ls->last = sentinel;
        ls->eqfunc = eqfunc;
        ls->size = 0;
    }

    return ls;
}


void ioopm_linked_list_destroy(ioopm_list_t* list) 
{
    list_entry_t* e = list->sentinel;
    
    while(e)
    {
        list_entry_t* next = e->next;
        FREE(e);
        e = next;
    }
    
    FREE(list);
}


void ioopm_linked_list_append(ioopm_list_t* list, elem_t value) 
{
    list_entry_t* new_entry = entry_new(value, NULL, list->last);
    
    list->size++;
    
    list->last->next = new_entry;
    list->last = new_entry;
}

void ioopm_linked_list_prepend(ioopm_list_t* list, elem_t value) 
{
    list_entry_t* old_head = list->sentinel->next;

    list_entry_t* new_entry = entry_new(value, old_head,NULL);

    list->sentinel->next = new_entry;
    list->size++;
}

IOOPM_RESULT ioopm_linked_list_insert(ioopm_list_t* list, size_t index, elem_t value) 
{
    if (index == 0)
    {
        ioopm_linked_list_prepend(list, value);
        return IOOPM_SOK;
    }

    if (index >= list->size && (index != 0))
        return IOOPM_EINVALIDX;
    
    list_entry_t* e = get_entry_from_index(list, index);
    
    if(!e)
    {
        return IOOPM_EINVALIDX;
    }
    
    list_entry_t* new_entry = entry_new(value, e, e->prev);

    e->prev->next = new_entry;
    e->prev = new_entry;

    list->size++;
            
    return IOOPM_SOK;
}


IOOPM_RESULT ioopm_linked_list_remove(ioopm_list_t* list, size_t index, elem_t* valptr) 
{
    list_entry_t* e = get_entry_from_index(list, index);
    
    if(!e)
        return IOOPM_EINVALIDX;
        
    if(valptr)
        *valptr = e->val;
    
    if(e == list->sentinel)
        list->sentinel = NULL;
    
    if(e->prev)
        e->prev->next = e->next;
        
    if(e->next)
        e->next->prev = e->prev;
    
    free(e);
    
    list->size--;
    
    return IOOPM_SOK;
}


IOOPM_RESULT ioopm_linked_list_get(ioopm_list_t* list, size_t index, elem_t* valptr)
{
    if (index >= list->size)
        return IOOPM_EINVALIDX;

    list_entry_t* e = get_entry_from_index(list, index);

    if (e)
    {
        if (valptr)
            *valptr = e->val;

        return IOOPM_SOK;
    }

    return IOOPM_ENOTFOUND;
}


bool eq_elem(int idx, elem_t a, void* extra)
{
    pair_t p = *(pair_t*)extra;

    return p.func(a, p.val);
}

bool ioopm_linked_list_contains(ioopm_list_t* list, elem_t element) 
{
    pair_t p;
    p.func = list->eqfunc;
    p.val = element;
    return ioopm_linked_list_any(list, eq_elem, &p);
}


size_t ioopm_linked_list_size(ioopm_list_t* list) 
{
    return list->size;
}


bool ioopm_linked_list_is_empty(ioopm_list_t* list) 
{
    return list->size == 0;
}


void ioopm_linked_list_clear(ioopm_list_t* list) 
{
    list_entry_t* e = list->sentinel;
    
    while(e)
    {
        list_entry_t* next = e->next;
        free(e);
        e = next;
    }
    
    list->sentinel = NULL;
    list->last = NULL;
    
    list->size = 0;
}


bool ioopm_linked_list_all(ioopm_list_t* list, ioopm_char_predicate prop, void *extra) 
{
    bool ret = false;
    
    list_entry_t* e = list->sentinel->next;
    
    int idx = 0;
    
    while (e) 
    {
        ret = prop(idx, e->val, extra);
        if (!ret)
            break;
                
        e = e->next;
        idx++;
    }
    
    return ret;
}


bool ioopm_linked_list_any(ioopm_list_t* list, ioopm_char_predicate prop, void *extra) 
{
    bool ret = false;
    
    list_entry_t* e = list->sentinel;
    
    int idx = 0;
    
    while (e) 
    {
        ret = prop(idx, e->val, extra);
        if (ret)
            return true;
                
        e = e->next;
        idx++;
    }
    
    return ret;
}


void ioopm_linked_apply_to_all(ioopm_list_t* list, ioopm_apply_char_function fun, void *extra) 
{
    list_entry_t* e = list->sentinel;
    
    int idx = 0;
    
    while (e) 
    {
        fun(idx, &(e->val), extra);
                
        e = e->next;
        idx++;
    }
}

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t* list)
{
    ioopm_list_iterator_t* ls = malloc(sizeof(ioopm_list_iterator_t));
    
    if (ls)
    {
        ls->list = list;
        ls->current = list->sentinel->next;
        ls->idx = 0;
    }
    return ls;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t* it)
{
    if (it->current)
        return it->current->next != NULL;

    return false;
}

IOOPM_RESULT ioopm_iterator_current(ioopm_list_iterator_t* it, elem_t* val)
{
    if (it->current && val)
        *val = it->current->val;

    return IOOPM_SOK;
}

IOOPM_RESULT ioopm_iterator_next(ioopm_list_iterator_t* it, elem_t* val)
{
    if (!ioopm_iterator_has_next(it))
        return IOOPM_ENONEXT;

    it->current = it->current->next;
    it->idx++;
    if (it->current && val)
        *val = it->current->val;

    return IOOPM_SOK;
}

void ioopm_iterator_reset(ioopm_list_iterator_t* it)
{
    it->current = it->list->sentinel;
    it->idx = 0;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t* it)
{
    free(it);
}

void ioopm_iterator_insert(ioopm_list_iterator_t* it, elem_t element)
{
    ioopm_linked_list_insert(it->list, it->idx, element);

    if (!it->current && it->list->sentinel)
    {
        it->current = it->list->sentinel;
    }
}

IOOPM_RESULT ioopm_iterator_remove(ioopm_list_iterator_t* iter, elem_t* val)
{
    if (iter->list->size == 0)
        return IOOPM_ELISTEMPTY;

    list_entry_t* to_remove = iter->current;

    iter->current->prev->next = iter->current->next;

    if(iter->current->next)
        iter->current->next->prev = iter->current->prev;

    iter->current = iter->current->next;

    if(val)
        *val = to_remove->val;

    free(to_remove);

    iter->list->size--;

    return IOOPM_SOK;
}