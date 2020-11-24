
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
		// malloc(sz(skiplist_node_t) + key_size +value_size - sz(key) - sz(value))
		// where sz = sizeof.
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
	// Impossible to work with list if there is no logic
	// of comparison of elements.
	if(cmp == NULL)
	{
		return NULL;
	}

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
	// Never NULL.
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
		skiplist_node_destroy(&to_free);
	}
	
	free((*list)->head);

	free(*list);
	*list = NULL;
}

skiplist_node_t* skiplist_insert(skiplist_t* list, void* key, size_t key_sz, void* value, size_t value_sz)
{
	// Not possible to insert anything into non-existing list
	// and not possible to insert NULL key.
	if(list == NULL || key == NULL)
	{
		return NULL;
	}

	// Must test if cmp is not NULL because
	// even though it was checked on the construction of a list
	// it may be changed directly by the user.
	

	// Array to save pointers to all NODES where algorithm decided to go 1 lvl down.
	//
	// It is not possible to do more than MAX_SKIPLIST_HEIGHT steps down.
	// But then why not to use list->height instead?
	// We can't go down more than list->height times, right?
	//
	// Right, but there's a probability that coin will be Heads more than list->height
	// times. And this array is used to create connections on all old and new levels.
	// If it's lenght will be less than MAX_SKIPLIST_HEIGHT it is possible that this
	// function will fail creating connections on new levels.
	skiplist_node_t* step_down_point_at[MAX_SKIPLIST_HEIGHT];
	memset(step_down_point_at, 0, sizeof(step_down_point_at));
	
	// By the way, maximum possible value for list->height is MAX_SKIPLIST_LVL - 1.
	// Not MAX_SKIPLIST_LVL!!!
	//
	// Because this variable indicates the number of additional layers on top of
	// original linked list. Maybe I must rename it.
	ssize_t	curr_lvl = list->height;
	skiplist_node_t* curr = (skiplist_node_t*) list->head;

	// Outter loop is used to make vertical steps.
	while(curr_lvl >= 0)
	{
		// Inner loop is used to make horizontal steps.
		// It will stop only when curr->key >= key.
		// Or if curr is NULL. It means the end of the list at curr_lvl.
		while(curr->next_at_lvl[curr_lvl] != NULL && list->cmp(curr->next_at_lvl[curr_lvl]->key, key) < 0) 
		{
			curr = curr->next_at_lvl[curr_lvl];
		}
		// Save the pointer to a current node before goind 1 lvl down.
		step_down_point_at[curr_lvl] = curr;

		// Go level down.
		curr_lvl--;
	}

	// If Outter loop stops at lvl 0 and Inner stop when curr->key >= key
	// then curr is the last NODE with key that is less than required key.
	// Then curr->next_at_lvl[0] is the node with key that is equal or greater
	// than the required key.
	
	// If curr is NULL than this is the very first element.
	curr = curr->next_at_lvl[0];

	// If curr == NULL then this is the end of the lvl 0 list 
	// and we must create new node.
	// If curr != NULL then check if curr->key == key, and if it's TRUE
	// then the required key already exists and no actions should be taken,
	// as this function must not overwrite old value.
	if(curr == NULL || list->cmp(curr->key, key) != 0)
	{
		skiplist_node_t* new_node;
		new_node = skiplist_node_init(key, key_sz, value, value_sz);
		list->length++;
		
		// if(new_node == NULL) is always false, so no checks.

		size_t upto_which_lvl_to_insert = 0;
		while(	upto_which_lvl_to_insert < MAX_SKIPLIST_HEIGHT - 1 &&
			rand() < RAND_MAX / 2 ) 
		{
			upto_which_lvl_to_insert++;
		}
		
		// If must create new levels and insert new_node there
		// Insertions at new levels is done by setting 
		// new_node as next for head at all new levels.
		curr_lvl = upto_which_lvl_to_insert;
		while(curr_lvl > list->height)
		{
			// Set new_node as next node of head at curr_lvl

			// list->head->next_at_lvl[curr_lvl] = new_node;
			step_down_point_at[curr_lvl] = (skiplist_node_t*) list->head;
			curr_lvl--;
		}
		// Set new height.
		list->height = upto_which_lvl_to_insert;

		// step_down_point_at contains all node at which alogorithm went down,
		// because key was greater or equal to required key.
		// That is why these are all the nodes at all level that must be
		// right before new_node. 
		//
		// All the other cells are equal to NULL because they were not
		// assigned during search. That's why algo must stop when it first sees NULL
		
		curr_lvl = 0;
		while(curr_lvl <= list->height)
		{
			new_node->next_at_lvl[curr_lvl] = 
				step_down_point_at[curr_lvl]->next_at_lvl[curr_lvl];

			step_down_point_at[curr_lvl]->next_at_lvl[curr_lvl] = new_node;
			curr_lvl++;
		}
	}
}


















/* END OF FILE */
