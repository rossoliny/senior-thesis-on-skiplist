
/************************************************/
/*						*/
/*	File:		skiplist_test.c		*/
/*	Date:		23.11.2020		*/
/*	Author:		Isa Dzhumanbaev		*/
/*						*/
/************************************************/


#include "skiplist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tstart(name) 	puts("\nTest Case:\t"  name); fflush(stdout)
#define tdone(name) 	puts("Test Pass:\t"  name); fflush(stdout)
#define tfail(msg) 	fputs("Test Failed:\t"  msg, stderr); fflush(stderr); exit(1)
#define tmsg(msg)	puts("\t\t" msg); fflush(stdout)

// NODE TESTS
void test_skiplist_node_init_1()
{
	tstart("test_skiplist_node_init_1");
	char name[] = "isa";
	int age = 23;
	unsigned memory_to_use = sizeof(skiplist_node_t) + sizeof(age) + sizeof(name);

	skiplist_node_t* node = skiplist_node_init(name, sizeof(name), &age, sizeof(age));
	
	if(node == NULL)
	{
		tfail("node is NULL.");
	}
	if(strcmp(node->key, name) != 0)
	{
		tfail("node->key != \"isa\".");
	}
	if(*(int*)node->value != age)
	{
		tfail("node->value != 23");
	}
	unsigned memory_used = sizeof(*node) + sizeof(*(int*)node->value) + strlen(node->key) + 1;
	if(memory_used != memory_to_use)
	{
		fprintf(stderr, "\t\tmemory_used != memory_to_use: %u != %u\n", memory_used, memory_to_use);
		tfail("memory usage requirements are not met");
	}
	int i = 0;
	while(i < MAX_SKIPLIST_HEIGHT)
	{
		if(node->next_at_lvl[i] != NULL)
		{
			tfail("node->next_at_lvl is not clean.");
		}
		i++;
	}

	skiplist_node_destroy(&node);
	tdone("test_skiplist_node_init_1");
}

void test_skiplist_node_init_2()
{
	tstart("test_skiplist_node_init_2");

	skiplist_node_t* node = skiplist_node_init(NULL, 0, (void*)123, 4);
	if(node != NULL)
	{
		tfail("node initialized with key equal to NULL.");
	}

	skiplist_node_destroy(&node);
	tdone("test_skiplist_node_init_2");
}

void test_skiplist_node_destroy_1()
{
	tstart("test_skiplist_node_destroy_1");
	char name[] = "isa";
	int age = 23;

	skiplist_node_t* node = skiplist_node_init(name, sizeof(name), &age, sizeof(age));
	skiplist_node_destroy(&node);

	if(node != NULL)
	{
		tfail("node is not destroyed.");
	}
	tdone("test_skiplist_node_destroy_1");
}

// SKIPLIST TESTS
int test_cmp_true(void* a, void* b)
{
	return 1;
}

void test_skiplist_init_1()
{
	tstart("test_skiplist_init_1");

	skiplist_t* skiplist = skiplist_init(test_cmp_true);

	if(skiplist == NULL)
	{
		tfail("skiplist_init() returned NULL.");
	}
	if(skiplist->cmp != test_cmp_true)
	{
		tfail("skiplist->cmp != test_cmp_true.");
	}
	if(skiplist->head == NULL)
	{
		tfail("skiplist->head is NULL.");
	}
	size_t i = 0;
	while(i < MAX_SKIPLIST_HEIGHT)
	{
		if(skiplist->head->next_at_lvl[i] != NULL)
		{
			tfail("list->head->next_at_lvl is not clean.");
		}
		i++;
	}

	skiplist_destroy(&skiplist);
	tdone("test_skiplist_init_1");
}

void test_skiplist_destroy_1()
{
	tstart("test_skiplist_destroy_1");
	
	skiplist_t* list = skiplist_init(NULL);
	if(list == NULL)
	{
		tfail("Could not initialize skiplist.");
	}

	skiplist_destroy(&list);

	if(list != NULL)
	{
		tfail("List was not destroyed.");
	}

	tdone("test_skiplist_destroy_1");
}




int main() 
{
	// NODE
	puts("****\t     NODE Tests\t\t****");
	test_skiplist_node_init_1();
	test_skiplist_node_init_2();
	test_skiplist_node_destroy_1();
	puts("\n****\t   NODE Tests Done\t****\n\n");

	// SKIPLIST	
	puts("\n****\t   SKIPLIST Tests\t****");
	test_skiplist_init_1();
	test_skiplist_destroy_1();
	puts("\n****\tSKIPLIST Tests Done\t****");
}






/* END OF FILE */
