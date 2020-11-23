
/************************************************/
/*						*/
/*	File:		skiplist.c		*/
/*	Date:		23.11.2020		*/
/*	Author:		Isa Dzhumabaev		*/
/*						*/
/************************************************/


#include "skiplist.h"
#include <stdlib.h>
#include <string.h>


// NODE

skiplist_node_t* skiplist_node_init(void* key, size_t key_size, void* value, size_t value_size)
{
	if(key != NULL)
	{
		// Allocate memort for one NODE and all pointers
		// in new_node->next_at_lvl[MAX_SKIPLIST_HEIGHT] to NULL
		skiplist_node_t* new_node = calloc(1, sizeof(skiplist_node_t));

		// Possible optimization is to allocate all memory at once in above calloc
		// something like 
		// calloc(sizeof(skiplist_node_t) + key_size + value_size - sizeof(key) - sizeof(value))
		//
		// But this may lead to errors when accessing value.
		// If key_size will be more than 8 bytes than access to value
		// will return 9-th byte of actuall key instead of 1-st byte of value.
		//
		// MAYBE USE union

		// Copy key into new memory
		new_node->key = malloc(key_size);
		memcpy(new_node->key, key, key_size);

		if(value != NULL)
		{
			// Copy value into new memory
			new_node->value = malloc(value_size);
			memcpy(new_node->value, value, value_size);
		}

		return new_node;
	}

	return NULL;
}
// void skiplist_node_set(skiplist_node_t* node, void* value, size_t value_size);
void skiplist_node_destroy(skiplist_node_t** node)
{
	if(node == NULL || *node == NULL)
	{
		return;
	}

	if((*node)->key != NULL)
	{
		// Was allocated dynamically in skiplist_node_init()
		free((*node)->key);
	}

	if((*node)->value != NULL)
	{
		// Was allocated dynamically in skiplist_node_init()
		free((*node)->value);
	}

	// Was allocated dynamically in skiplist_node_init()
	free(*node);
	
	// Clean pointer
	*node = NULL;
}


// SKIPLIST

skiplist_t* skiplist_init(compare_t cmp)
{
	// Allocate memory for skiplist struct
	skiplist_t* list = calloc(1, sizeof(skiplist_t));
	if(list == NULL)
	{
		return NULL;
	}

	// Allocate memory for head of skiplist
	list->head = calloc(1, sizeof(skiplist_head_t));
	if(list->head == NULL)
	{
		return NULL;
	}
	// Assign privided function pointer
	list->cmp = cmp;

	return list;
}

void skiplist_destroy(skiplist_t** list)
{
	// If list is NULL then nothing to destroy
	if(list == NULL || *list == NULL)
	{
		return;
	}

	// list->head is not a node, so first elemnt is located after list->head.
	skiplist_node_t* curr = (*list)->head->next_at_lvl[0];
	skiplist_node_t* to_free;

	while(curr != NULL)
	{
		// Current node is the node to destroy.
		to_free = curr;
		// But must save node after current to delete in next iteration.
		curr = curr->next_at_lvl[0];
		free(to_free);
	}
	
	free((*list)->head);

	free(*list);
	*list = NULL;
}




















/* END OF FILE */
