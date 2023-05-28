/*
 * Uses generic linkedlist to store keyvalue pairs.
 * All the operations of the linkedlist, being generic, can be used to search and free the list key-values as well, no change 
 * is needed in the linkedlist.c file.
 * 
 * 16.10.14
 */

#ifndef LL_C_h
#define LL_C_h

#include "linkedlist.h"

#define MAX_HEADDER_KEY_LENGTH 100
#define MAX_HEADDER_VALUE_LENGTH 400

struct keyvalue
{
    char value[MAX_HEADDER_VALUE_LENGTH];
    char key[MAX_HEADDER_KEY_LENGTH];
};

struct linkedlist *CreateKeyValueList();
void AddToList(struct linkedlist *ll, char *key, char *value);

#endif