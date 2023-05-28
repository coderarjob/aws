#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

    struct llnode
    {
	void *item;
	struct llnode *next;
	struct llnode *prev;
    };
    
    struct linkedlist
    {
	int (*ismatch)(void *in, void*key);
	void (*free)(void *item);
	char *(*tostring)(void *item);
	
	struct llnode *head;
    };
    
    void remove_node(struct llnode *node,struct linkedlist *ll);
    void *search_ll(struct linkedlist *ll, void *key);
    void free_ll(struct linkedlist *ll);
    struct llnode *insert_to_ll(struct linkedlist *ll, void *what, size_t length);
    struct linkedlist *create_ll( int (*ismatch)(void *in, void*what), 
				 void (*free)(void *item), 
				  char *(*tostring)(void *item));
    
#endif  //LINKEDLIST_H