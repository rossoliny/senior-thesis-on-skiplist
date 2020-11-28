#include <stdlib.h>
#include <stdio.h>

#define false 0
#define true 1

#define bool char

#define BITS_IN_RANDOM 31
#define ALLOW_DUPLICATES
#define MAX_NUMBER_OF_LEVELS 16
#define MAX_LEVEL (MAX_NUMBER_OF_LEVELS - 1)
#define new_node_at_lvl(i) (node*) malloc(sizeof(struct node) + (i) * sizeof(node**))

typedef int key_t;
typedef int val_t;

#ifdef ALLOW_DUPLICATES
	void insert();
#else
	bool insert();
#endif
bool search(), delete();

typedef struct node* node_p;

typedef struct node
{
	key_t key;
	val_t val;
	struct node* forward[1];
} node;

typedef struct list
{
	int level;
	struct node* header;
} list;

typedef list* list_p;

node* NIL;

int randoms_left;
int random_bits;

void init(void) 
{
	NIL = new_node_at_lvl(0);
	NIL->key = 0x7fffffff;
	random_bits = rand();
	randoms_left = BITS_IN_RANDOM / 2;
};

list* new_list(void)
{
	list* l = (list*) malloc(sizeof(struct list));
	l->level = 0;
	l->header = new_node_at_lvl(MAX_NUMBER_OF_LEVELS);

	int i = 0;
	while(i < MAX_NUMBER_OF_LEVELS)
	{
		l->header->forward[i] = NIL;
		i++;
	}

	return(l);
};

void free_list(list* list)
{
	register node* curr;
	register node* next;

	next = list->header;

	do
	{
		next = curr->forward[0];
		free(curr);
		curr = next;
	} 
	while(curr != NIL);

	free(list);
};

int random_level(void)
{
	register int level = 0;
	register int b;

	do
	{
		b = random_bits & 3;
		if(!b)
		{
			level++;
		}
		random_bits >>= 2;

		if(--randoms_left == 0)
		{
			random_bits = rand();
			randoms_left = BITS_IN_RANDOM / 2;
		}
	} while(!b);

	return (level > MAX_LEVEL ? MAX_LEVEL : level);
};



#ifdef ALLOW_DUPLICATES
	#define insert_ret_t void
#else
	#define insert_ret_t bool
#endif

insert_ret_t insert(register list* list, register key_t key, register val_t val)
{
	register int curr_lvl;
	register node* curr;
	register node* next;

	node* update[MAX_NUMBER_OF_LEVELS];

	curr 	 = list->header;
	curr_lvl = list->level;

	do
	{
		while(next = curr->forward[curr_lvl], next->key < key)
		{
			curr = next;
		}
		update[curr_lvl] = curr;
	} while(--curr_lvl >= 0);

	#ifndef ALLOW_DUPLICATES
	if(next->key == key)
	{
		next->val = val;
		return false;
	}
	#endif

	#define new_lvl curr_lvl
	new_lvl = random_level();

	if(new_lvl > list->level)
	{
		new_lvl = ++(list->level);
		update[new_lvl] = list->header;
	}

	#define new_node next
	new_node = new_node_at_lvl(new_lvl);
	new_node->key = key;
	new_node->val = val;
	
	do
	{
		new_node->forward[curr_lvl] = update[curr_lvl]->forward[curr_lvl];
		update[curr_lvl]->forward[curr_lvl] = new_node;
	} while(--new_lvl >= 0);

	#ifndef ALLOW_DUPLICATES
	return (true);
	#endif
	
	#undef new_node
	#undef new_lvl
}

bool search(register list* list, register key_t key, register val_t* res)
{
	register int curr_lvl;
	register node* curr;
	register node* next;
	curr_lvl = list->level;
	curr = list->header;

	do
	{
		while(next = curr->forward[curr_lvl], next->key < key)
		{
			curr = next;
		}
	} while(--curr_lvl >= 0);

	if(next->key != key)
	{
		return (false);
	}
	*res = next->val;
	return (true);
}

bool delete(register list* list, register key_t key)
{
	register int curr_lvl, height;
	register node* curr;
	register node* next;

	node* update[MAX_NUMBER_OF_LEVELS];

	curr_lvl = height = list->level;
	curr = list->header;

	do
	{
		while(next = curr->forward[curr_lvl], next->key < key)
		{
			curr = next;
		}
		update[curr_lvl] = curr;
	} while(--curr_lvl >= 0);

	if(next->key == key)
	{
		curr = next;
		curr_lvl = 0;
		while(	curr_lvl <= height && 
			(update[curr_lvl]->forward[curr_lvl] == curr) )
		{
			update[curr_lvl]->forward[curr_lvl] = curr->forward[curr_lvl];
			curr_lvl++;
		}

		free(curr);

		while(list->header->forward[height] == NIL && height > 0)
		{
			height--;
		}
		list->level = height;

		return (true);
	}

	return (false);
}

#define SAMLE_SIZE 65536
int main() {
    list* l;
    register int i, k;
    key_t keys[SAMLE_SIZE];
    val_t v;

    init();

    l = new_list();

    for (k = 0; k < SAMLE_SIZE; k++) {
        keys[k] = rand();
        insert(l, keys[k], keys[k]);
    };

    for (i = 0; i < 4; i++) {
        for (k = 0; k < SAMLE_SIZE; k++) {
            if (!search(l, keys[k], & v)) printf("error in search #%d,#%d\n", i, k);
            if (v != keys[k]) printf("search returned wrong value\n");
        };
        for (k = 0; k < SAMLE_SIZE; k++) {
            if (!delete(l, keys[k])) printf("error in delete\n");
            keys[k] = rand();
            insert(l, keys[k], keys[k]);
        };
    };

    free_list(l);

};
