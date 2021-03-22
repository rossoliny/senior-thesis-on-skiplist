
/************************************************/
/*												*/
/*	File:		skiplist.h						*/
/*	Date:		23.11.2020						*/
/*	Author:		Isa Dzhumabaev					*/
/*												*/
/************************************************/


/* Basic C implementation of skiplist data structure.
 * Limited in maximum possible height and lenght.
 *
 * Time complexity for this implementation
 *	Search:	
 * 	Insert:
 * 	Delete:
 *
 * Space complexity for this implementation
 * 	(sizeof(struct skiplist_node) * N * lg(N) ) + sizeof(struct skiplist_node)
 */





#pragma once
#include <stddef.h>

/* Right bound for skiplist. 
 */
#define MAX_SKIPLIST_LENGTH 65536

/* Upper bound for skiplist.
 *
 * lg(65536) = 16	
 * On platform with 8-byte pointers
 * memory overhead per node is 8 * 16 = 128 bytes.
 *
 * Total memory overhead is 128 * N + sizeof(struct skiplist)
 *
 */

#define MAX_SKIPLIST_HEIGHT 16


// NODE
typedef struct skiplist_node
{
	struct skiplist_node* next_at_lvl[MAX_SKIPLIST_HEIGHT];

	void* 	key;
	void* 	value;
} skiplist_node_t;

// Interface for NODE

// TODO: Add documentation
skiplist_node_t* skiplist_node_init(void* key, size_t key_size, void* value, size_t value_size);

// May be usefull in future versions.
// void skiplist_node_set(skiplist_node_t* node, void* value, size_t value_size);

// TODO: Add documentation
void skiplist_node_destroy(skiplist_node_t** node);


// SKIPLIST

/* Type of function that will be used to compare elements of skiplist.
 */
typedef int (*compare_t)(void* a, void* b);

/* Head of skiplist doesn't need key and value 
 * as it will never be used in comparisons.
 */
typedef struct skiplist_head
{
	skiplist_node_t* next_at_lvl[MAX_SKIPLIST_HEIGHT];
} skiplist_head_t;


typedef struct skiplist
{
	skiplist_head_t* head;
	compare_t cmp;

	size_t	height;
	size_t	length;
} skiplist_t;

// Interface for SKIPLIST

/* FUNCTION
 *	skiplist_t* skiplist_init();
 *
 * DESCRIPTION
 *	Allocates memory for skiplist_t and initializes all its members to zero.
 *
 * PARAMETERS
 *	compare_t cmp:
 *		typedef (*compare_t)(void* a, void* b);
 *		Pointer to a function that will be used to compare elements of skiplist.
 *
 * RETURN VALUE:
 *	Pointer to newly created empty skiplist.
 */
skiplist_t* skiplist_init(compare_t cmp);


/* FUNCTION
 *	skiplist_t* skiplist_destroy(skiplist_t* list);
 *
 * DESCRIPTION
 *	1) Deallocates memory of each element in the list.
 *	2) Deallocates memory of pointed by list.
 *
 * PARAMETERS
 *	skiplist_t* list:
 *		Pointer to skiplist_t created by skiplist_init().
 *
 * RETURN VALUE:
 *	None.
 */
void skiplist_destroy(skiplist_t** list);


/* FUNCTION 
 * 	skiplist_node_t* skiplist_search(skiplist_t* list, void* key)
 *
 * DESCRIPTION
 * 	Searches for key in the list using list->cmp.
 *
 * PARAMET
 * ERS
 * 	skiplist_t* list:
 * 		Pointer to skiplist to search in.
 *
 * 	void* key:
 * 		Key to look for in the list.
 *
 * RETURN VALUE:
 * 	If key exists in list then returns pointer to corresponding node_t
 * 	If Key is not found then returns NULL
 */
skiplist_node_t* skiplist_search(skiplist_t* list, void* key);

// TODO: Add documentation.
skiplist_node_t* skiplist_insert(skiplist_t* list, void* key, size_t key_sz, void* value, size_t value_sz);

// TODO: Add documentation.
skiplist_node_t* skiplist_remove(skiplist_t* list, void* key);

// TODO: Add documentation.
void skiplist_clear(skiplist_t* list);



/* END OF FILE */
