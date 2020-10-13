#pragma once
#include "linked_list.h"
#include "common.h"

/**
 * @file hash_table.h
 * @author Marcus Jansson, Max Sahlholm
 * @date 14 Sep 2020
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see http://wrigstad.com/ioopm19/assignments/assignment1.html
 */
 
#define STANDARD_CAP 17

typedef union element elem_t;
typedef enum IOOPM_RES IOOPM_RESULT;

typedef struct hash_table ioopm_hash_table_t;
typedef struct hash_table_entry hash_table_entry_t;
typedef struct list ioopm_list_t; /// Meta: struct definition goes in C file

typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void* extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t* value, void* extra);

typedef int (*ioopm_hash_function)(elem_t key);
typedef bool (*ioopm_elem_equiv_function)(elem_t keyA, elem_t keyB);

/// @brief Create a new hash table
/// @param hfunc the hashfunction to be used
/// @param key_equiv a function to compare keys
/// @param val_equiv a function to compare values
/// @return A new empty hash table
ioopm_hash_table_t* ioopm_hash_table_create(ioopm_hash_function hfunc, ioopm_elem_equiv_function key_equivfunc, ioopm_predicate val_equivfunc);

/// @brief Create a new hash table
/// @param hfunc the hashfunction to be used
/// @param key_equiv a function to compare keys
/// @param val_equiv a function to compare values
/// @param initial_cap the initial capacity of the hashtable
/// @param load_fac the load factor which causes the table to increase its size.
/// @return A new empty hash table
ioopm_hash_table_t* ioopm_hash_table_create_size(ioopm_hash_function hfunc, ioopm_elem_equiv_function key_equivfunc, ioopm_predicate val_equivfunc, size_t initial_cap, float load_fac);

/// @brief Delete a hash table and free its memory
/// param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t* ht);

/// @brief add key => value hash_table_entry in hash table ht
/// @param ht hash table operated upon
/// @param key the key to insert
/// @param value value to insert
/// @return IOOPM_RESULT code indicating success or error
IOOPM_RESULT ioopm_hash_table_insert(ioopm_hash_table_t* ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key the key to lookup
/// @param result pointer to the value mapped to by key. If NULL, this parameter is ignored.
/// @return IOOOPM_RESULT code indicating success or error
IOOPM_RESULT ioopm_hash_table_lookup(ioopm_hash_table_t* ht, elem_t key, elem_t* result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key the key to remove
/// @param result pointer to the value mapped to by key. If NULL, this parameter is ignored.
/// @return IOOOPM_RESULT code indicating success or error
IOOPM_RESULT ioopm_hash_table_remove(ioopm_hash_table_t* ht, elem_t key, elem_t* result);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true if size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t* ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t* ht);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t* ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return a list of the keys in the hash table ht
ioopm_list_t* ioopm_hash_table_keys(ioopm_hash_table_t* ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return a list of the values in the hash table ht
ioopm_list_t* ioopm_hash_table_values(ioopm_hash_table_t* ht);

/// @brief check if a hash table has an hash_table_entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t* ht, elem_t key);

/// @brief check if a hash table has an hash_table_entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t* ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t* ht, ioopm_predicate pred, void* arg);

/// @brief check if a predicate is satisfied by any hash_table_entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t* ht, ioopm_predicate pred, void* arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t* ht, ioopm_apply_function apply_fun, void* arg);