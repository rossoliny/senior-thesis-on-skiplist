
/************************************************/
/*												*/
/*	File:		skiplist_test.c					*/
/*	Date:		23.11.2020						*/
/*	Author:		Isa Dzhumabaev					*/
/*												*/
/************************************************/


#include "skiplist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

#define tstart(name) 	puts("\nTest Case:\t"  name); fflush(stdout)
#define tdone(name) 	puts("Test Pass:\t"  name); fflush(stdout)
#define tfail(msg) 	fputs("Test Failed:\t"  msg, stderr); fflush(stderr); exit(1)

#ifdef TDEBUG
	#define tmsg(msg)	puts("\t\t" msg); fflush(stdout)
	#define print_skiplist(list) print_skiplist_func(list)
#else
	#define tmsg(msg)
	#define print_skiplist(list)
#endif

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


void print_skiplist_func(skiplist_t* list)
{
	skiplist_node_t* node = list->head->next_at_lvl[0];
	while(node != NULL)
	{
		char* k = (char*) node->key;
		int*  v	= (int*) node->value;
		printf("\t\t(%s,\t%d)\n", k, *v);
		node = node->next_at_lvl[0];
	}
	puts("");
	fflush(stdout);
}

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
	if(list != NULL)
	{
		tfail("Error, list initialized without cmp function.");
	}
	list = skiplist_init(test_cmp_true);

	skiplist_destroy(&list);

	if(list != NULL)
	{
		tfail("List was not destroyed.");
	}

	tdone("test_skiplist_destroy_1");
}

int cmp_1(void* a, void* b)
{
	return strcmp((char*)a, (char*)b);
}

void test_skiplist_insert_1()
{
	tstart("test_skiplist_insert_1");	

	skiplist_t* list = skiplist_init(cmp_1);
	
	char name[] 	= "isa";
	int age 	= 23;

	tmsg("\n\t\ttrying to insert 1st element.");
	skiplist_insert(list, name, sizeof(name), &age, sizeof(age));
	tmsg("inserted.\n");

	int res_age = *((int*) list->head->next_at_lvl[0]->value);
	if(res_age != age)
	{
		tfail("res_age is not equal to age.");
	}
	


	char name2[] 	= "ali";
	int age2 	= 228;
	tmsg("trying to insert 2nd element at position before 1st.");
	skiplist_insert(list, name2, sizeof(name2), &age2, sizeof(age2));
	tmsg("inserted.\n");

	res_age = *((int*) list->head->next_at_lvl[0]->next_at_lvl[0]->value);
	if(res_age != age)
	{
		tfail("Not iserted at right position, res_age != age.");
	}



	char name3[] 	= "isz";
	int age3 	= -1;

	tmsg("trying to insert 3rd element at last position.");
	skiplist_insert(list, name3, sizeof(name3), &age3, sizeof(age3));
	tmsg("inserted.\n");

	int res_age3 = *((int*) list->head->next_at_lvl[0]->next_at_lvl[0]->next_at_lvl[0]->value);
	if(res_age3 != age3)
	{
		tfail("Not inserted at last position.");
	}



	char name4[] 	= "isb";
	int age4 	= 124532;

	tmsg("trying to insert 4th element before last.");
	skiplist_insert(list, name4, sizeof(name4), &age4, sizeof(age4));
	tmsg("inserted.");
	
	int res_age4 = *(int*) list->head->next_at_lvl[0]->next_at_lvl[0]->next_at_lvl[0]->value;
	if(res_age4 != age4)
	{
		tfail("Not inserted at correct position.");
	}

	tmsg("");
	tmsg("Insertions are finished. Printing skiplist.");
	print_skiplist(list);

	skiplist_destroy(&list);
	tdone("test_skiplist_insert_1");
}


int cmp_2(void* a, void* b)
{
	return strcmp((char*) a, (char*) b);
}

void test_skiplist_search_1()
{
	tstart("test_skiplist_search_1");
	

	skiplist_t* list = skiplist_init(cmp_2);

	if(list == NULL)
	{
		tfail("List failed to initialize.");
	}

	// preparation
	char* names[] = { "isa", "ali", "isaz", "isab", "isac", "isad" };
	int ages[] = { 23, 432, 35, 562 ,345234, 3452 };

	int i = 0;
	while(i < sizeof(names)/sizeof(names[0]))
	{
		skiplist_insert(list, names[i], strlen(names[i])+1, ages + i, sizeof(ages[i]));
		i++;
	}


	// tests
	skiplist_node_t* node = skiplist_search(list, "isa");
	if(node == NULL)
	{
		tfail("Search attempt returned NULL.");
	}
	if(strcmp(node->key, "isa") != 0)
	{
		printf("\n\t\tExpected:\t%s.\tActual:\t%s\n", "isa", (char*)node->key);
		tfail("Search attempt found wrong element.");
	}

	node = skiplist_search(list, "does not exit");
	if(node != NULL)
	{
		printf("\n\t\tSearch result: %s", (char*) node->key);
		tfail("Search for non existing element returned not NULL.");
	}

	skiplist_destroy(&list);
	tdone("test_skiplist_search_1");
}

int cmp_3(void* a, void* b)
{
	return strcmp( (char*) a, (char*) b );
}

void test_skiplist_remove_1()
{
	tstart("test_skiplist_remove_1");


	skiplist_t* list = skiplist_init(cmp_3);

	char name1[] = "isa";
	char surn1[] = "dzhumabaev";

	skiplist_insert(list, name1, sizeof(name1), surn1, sizeof(surn1));

	skiplist_node_t* res = skiplist_remove(list, name1);

	if(res != NULL)
	{
		tfail("After deletion of last element, runction did not return NULL.");
	}
	if(list->length != 0)
	{
		printf("\t\tlist->length = %d\n", (int) list->length);
		tfail("List is not empty after removing last element.");
	}

	char* names[] = { "isa", "ali", "isaz", "isab", "isac", "isad" };
	int ages[] = { 23, 432, 35, 562 ,345234, 3452 };

	int i = 0;
	while(i < sizeof(names)/sizeof(names[0]))
	{
		skiplist_insert(list, names[i], strlen(names[i]) + 1, ages + i, sizeof(ages[i]));
		i++;
	}

	
	res = skiplist_remove(list, "isa");
	if(strcmp(res->key, "isab") != 0)
	{
		tfail("After removing element, function did not return next element after it.");
	}

	while(list->length > 0)
	{
		skiplist_remove(list, list->head->next_at_lvl[0]->key);
	}
	if(list->length != 0)
	{
		tfail("List not empty after removing all its elements.");
	}


	skiplist_destroy(&list);
	tdone("test_skiplist_remove_1");

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
	test_skiplist_insert_1();
	test_skiplist_search_1();
	test_skiplist_remove_1();
	puts("\n****\tSKIPLIST Tests Done\t****");

	puts("\n****\tAll Tests Are Passed\t****");
}






/* END OF FILE */
