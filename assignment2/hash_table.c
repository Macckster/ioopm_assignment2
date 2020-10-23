#include "hash_table.h"

#define FREE(x) { free(x); x = NULL; } 

struct hash_table_entry
{
    elem_t key;
    elem_t value;
    hash_table_entry_t* next;
    hash_table_entry_t* prev;
};

struct hash_table
{
    size_t size;
    hash_table_entry_t** buckets;
    ioopm_hash_function hfunc;
    ioopm_elem_equiv_function key_equivfunc;
    ioopm_predicate val_equivfunc;
    
    size_t capacity;

    float load_fac;
};

ioopm_hash_table_t* ioopm_hash_table_create(ioopm_hash_function hfunc, ioopm_elem_equiv_function key_equivfunc, ioopm_predicate val_equivfunc)
{
    return ioopm_hash_table_create_size(hfunc, key_equivfunc, val_equivfunc, 17, 0.75f);
}

ioopm_hash_table_t* ioopm_hash_table_create_size(ioopm_hash_function hfunc, ioopm_elem_equiv_function key_equivfunc, ioopm_predicate val_equivfunc, size_t initial_cap, float load_fac)
{
    ioopm_hash_table_t* result = calloc(1, sizeof(ioopm_hash_table_t));
    if (result)
    {
        result->hfunc = hfunc;
        result->key_equivfunc = key_equivfunc;
        result->val_equivfunc = val_equivfunc;
        result->size = 0;
        result->load_fac = load_fac;
        result->capacity = initial_cap;

        result->buckets = calloc(initial_cap, sizeof(hash_table_entry_t));
    }
    return result;
}

hash_table_entry_t* ioopm_find_hash_table_entry(ioopm_hash_table_t* ht, elem_t key)
{
    if (ht->hfunc(key) < 0)
        return NULL;
        
    hash_table_entry_t* e = ht->buckets[ht->hfunc(key) % ht->capacity];
    
    while (e) {
        if (ht->key_equivfunc(e->key, key))
            return e;
            
        e = e->next;
    }
    
    return NULL;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t* ht)
{
    for (size_t i = 0; i < ht->capacity; i++)
    {
        hash_table_entry_t* e = ht->buckets[i];
        while (e) 
        {
            hash_table_entry_t* next = e->next;
            FREE(e);
            e = next;
        }
    }
    
    FREE(ht->buckets);
    FREE(ht);
}

IOOPM_RESULT ioopm_hash_table_lookup(ioopm_hash_table_t* ht, elem_t key, elem_t* result)
{
    if (ht->hfunc(key) < 0)
        return IOOPM_EINVALKEY;
    
    hash_table_entry_t* it = ioopm_find_hash_table_entry(ht, key);

    if (it)
    {
        if (result)
        {
            *result = it->value;
        }
        return IOOPM_SOK;
    }
    
    return IOOPM_ENOTFOUND;
}

size_t get_new_size(size_t old)
{
    size_t primes[] = { 17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381 };

    for (size_t i = 0; i < 11; i++)
    {
        if (primes[i] > old)
            return primes[i];
    }

    return primes[10];
}

void insert_at(ioopm_hash_table_t* ht, size_t buck_idx, hash_table_entry_t* e)
{
    hash_table_entry_t* start = ht->buckets[buck_idx];

    if (!start)
    {
        ht->buckets[buck_idx] = e;
    }
    else
    {
        hash_table_entry_t* prev = NULL;
        while (start)
        {
            prev = start;
            start = start->next;
        }

        if(prev)
            prev->next = e;
        e->prev = prev;
    }
}

void resize(ioopm_hash_table_t* ht)
{
    size_t old_size = ht->capacity;
    size_t new_size = get_new_size(ht->capacity);
    ht->capacity = new_size;

    hash_table_entry_t** old_buckets = ht->buckets;
    ht->buckets = calloc(new_size, sizeof(hash_table_entry_t));
    ht->capacity = new_size;

    for (size_t i = 0; i < old_size; i++)
    {
        hash_table_entry_t* entry = old_buckets[i];

        while (entry)
        {
            hash_table_entry_t* old_next = entry->next;
            entry->next = NULL;
            size_t buck_idx = (size_t)(ht->hfunc(entry->key) % (int)new_size);;
            insert_at(ht, buck_idx, entry);
            entry = old_next;
        }
    }

    free(old_buckets);
}

IOOPM_RESULT ioopm_hash_table_insert(ioopm_hash_table_t* ht, elem_t key, elem_t value)
{
    if ((ht->load_fac * (float)(ht->capacity)) < ht->size)
    {
        resize(ht);
    }

    if (ht->hfunc(key) % (int)(ht->capacity) < 0)
        return IOOPM_EINVALKEY;
    
    hash_table_entry_t* new_hash_table_entry = calloc(1, sizeof(hash_table_entry_t));
    if (new_hash_table_entry)
    {
        new_hash_table_entry->key = key;
        new_hash_table_entry->value = value;
        new_hash_table_entry->next = NULL;
        new_hash_table_entry->prev = NULL;
    }
    ht->size++; // Increase size since we have one more element

    hash_table_entry_t* e = ht->buckets[ht->hfunc(key) % ht->capacity];
    if (!e)
    {
        ht->buckets[ht->hfunc(key) % ht->capacity] = new_hash_table_entry;

        return IOOPM_SOK;
    }
    else if (ht->key_equivfunc(e->key, key)) 
    {
        FREE(e);
        ht->buckets[ht->hfunc(key) % ht->capacity] = new_hash_table_entry;
        ht->size--;
        return IOOPM_SOK;
    }
    
    while (e->next) {
        if (ht->key_equivfunc(e->next->key, key))
        {
            if(new_hash_table_entry)
                new_hash_table_entry->next = e->next->next;
            FREE(e->next);
            e->next = new_hash_table_entry;
            if(new_hash_table_entry)
                new_hash_table_entry->prev = e;
            
            // we replaced an existing hash_table_entry, revert the size increment
            ht->size--;
            
            return IOOPM_SOK;
        }
        e = e->next;
    }
    
    e->next = new_hash_table_entry;
    if(new_hash_table_entry)
        new_hash_table_entry->prev = e;
    return IOOPM_SOK;
}

IOOPM_RESULT ioopm_hash_table_remove(ioopm_hash_table_t* ht, elem_t key, elem_t* result)
{
    if(ht->hfunc(key) < 0)
        return IOOPM_EINVALKEY;
    
    hash_table_entry_t* e = ht->buckets[ht->hfunc(key) % ht->capacity];
    if (!e)
        return IOOPM_ENOTFOUND;
    
    if (ht->key_equivfunc(e->key, key))
    {
        ht->buckets[ht->hfunc(key) % ht->capacity] = e->next;
        
        // this will be the new first hash_table_entry, remove the 'prev'
        if (e->next)
            e->next->prev = NULL;
            
        elem_t val = e->value;

        FREE(e);
        if (result)
            *result = val;

        ht->size--;
        return IOOPM_SOK;
    }
    
    e = ioopm_find_hash_table_entry(ht, key);
    if (e) 
    {
        // this hash_table_entry should ALWAYS have a prev hash_table_entry since we already checked specifically if it was the first element in the list
        // link the previous hash_table_entry in the list with the next hash_table_entry, skipping the current one
        if(e->prev)
            e->prev->next = e->next;
        if (e->next)
            e->next->prev = e->prev;
            
        elem_t val = e->value;

        FREE(e);
        if (result)
            *result = val;
            
        ht->size--;
        return IOOPM_SOK;
    }
    
    return IOOPM_ENOTFOUND;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t* ht)
{
    return ht->size == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t* ht)
{
    for (size_t i = 0; i < ht->capacity; i++)
    {
        hash_table_entry_t* e = ht->buckets[i];
        while (e) 
        {
            hash_table_entry_t* next = e->next;

            FREE(e);
            e = next;
        }
        
        ht->buckets[i] = NULL;
    }
    
    ht->size = 0;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t* ht)
{
    return ht->size;
}

ioopm_list_t* ioopm_hash_table_keys(ioopm_hash_table_t* ht)
{
    ioopm_list_t* keys = ioopm_linked_list_create(ht->key_equivfunc);
    
    for (size_t i = 0; i < ht->capacity; i++)
    {
        hash_table_entry_t* e = ht->buckets[i];

        while (e)
        {
            ioopm_linked_list_append(keys, e->key);
            e = e->next;
        }
    }

    return keys;
}

ioopm_list_t* ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
    ioopm_list_t* values = ioopm_linked_list_create(ht->key_equivfunc);
    
    for (size_t i = 0; i < ht->capacity; i++) {
        hash_table_entry_t* e = ht->buckets[i];
        while (e) 
        {
            ioopm_linked_list_append(values, e->value);
            e = e->next;
        }
    }
    
    return values;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *h, elem_t key)
{
    return ioopm_find_hash_table_entry(h, key);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t* ht, elem_t value)
{
    return ioopm_hash_table_any(ht, ht->val_equivfunc, &value);
}

bool ioopm_hash_table_all(ioopm_hash_table_t* ht, ioopm_predicate pred, void* arg)
{
    bool ret = true;
    for (size_t i = 0; i < ht->capacity; i++)
    {
        hash_table_entry_t* e = ht->buckets[i];
        while (e) 
        {
            ret = pred(e->key, e->value, arg);
            if (!ret)
                break;
                
            e = e->next;
        }
    }
    
    return ret;
}

bool ioopm_hash_table_any(ioopm_hash_table_t* ht, ioopm_predicate pred, void* arg)
{
    for (size_t i = 0; i < ht->capacity; i++)
    {
        hash_table_entry_t* e = ht->buckets[i];
        while (e) 
        {
            if(pred(e->key, e->value, arg))
                return true;
                
            e = e->next;
        }
    }
    return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t* ht, ioopm_apply_function apply_fn, void* arg) 
{
    for (size_t i = 0; i < ht->capacity; i++)
    {
        hash_table_entry_t* e = ht->buckets[i];
        while (e) 
        {
            apply_fn(e->key, &e->value, arg);
            e = e->next;
        }
    }
}