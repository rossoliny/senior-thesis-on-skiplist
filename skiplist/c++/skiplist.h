#pragma once
#include <vector>

namespace skiplist {

	template<class Key, class Val>
	class skiplist
	{
		template<size_t HEIGHT>
		class node
		{
			friend skiplist;
			std::vector<node*> next(HEIGHT);

			node() = delete;
			node(Key key, Val val) : key(key), val(val)
			{
				std::fill(next.begin(), next.end(), NIL);
			}

			Key key;
		public:
			Val val;
		};

		template<>
		class node<1>
		{
			friend skiplist;
			node* next[1];

			node() = delete;
			node(Key key, Val val) : key(key), val(val)
			{
				next[0] = NIL;
			}
		};

		template<size_t H>
		class header : node<H> { };

		inline size_t random_lvl()
		{
			size_t res = 0;
			while(rand() < 1/2)
			{
				res++;
			}

			return res;
		}

		static const node* const NIL = new node();

		size_t count;
		size_t height;
		header<1> header;
	public:	
		skiplist() : count(0), layers(0) { }

		bool insert(Key key, Val val)
		{
			ssize_t curr_lvl = layers;
			node* curr = header;

			node* step_down_point_at[layers];
			// no memset cause NIL

			while(curr_lvl >= 0) {
				while(	curr->next[curr_lvl] != NIL &&
					curr->next[curr_lvl]->key < key)
				{
					curr = curr->next[curr_lvl];
				}
				step_down_point_at[curr_lvl] = curr;
				--curr_lvl;
			}
			curr = curr->next[0];

			if(curr != NIL || curr->key == key)
			{
				curr->val = val;
				return false;
			}

			#define new_node curr

			size_t new_lvl = random_lvl();
			new_node = new node<new_lvl>(key, val);
			
			header.next.resize(new_lvl, new_node);

			curr_lvl = 0;
			while(curr_lvl < layers)
			{
				new_node->next[curr_lvl] = step_down_point_at[curr_lvl]->next[curr_lvl];
				step_down_point_at[curr_lvl]->next[curr_lvl] = new_node;
				curr_lvl++;
			}

			layers = new_lvl;
			#undef new_node
			return true;
		}

	};


}








