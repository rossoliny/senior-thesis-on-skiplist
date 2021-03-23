#include <vector>
#include <string>
#include <iostream>

using namespace std;
template<typename Key, typename Tp>
class smap
{
	class node_base
	{
	public:
		vector<node_base*> next_at;

		node_base()
			: next_at(1)
		{
			next_at[0] = nullptr;
		}
	};


	class node : public node_base
	{
	public:
		node_base* prev;
		Key key;
		Tp value;

		node(const Key& k, Tp val)
			: prev(nullptr)
			, key(k)
			, value(std::move(val))
		{
		}
	};

	node_base head = node_base();
	size_t list_height = 0;
	size_t list_length = 0;

	size_t random_height() const
	{
		size_t res = 1;
		while (res <= list_height && rand() < RAND_MAX / 2)
		{
			res++;
		}
		return res;
	}

	inline Tp& key(node_base* n)
	{
		return ((node*) n)->key;
	}

public:
	smap() = default;

	node* insert(const Key& k, const Tp& val)
	{
		node_base* curr = &head;
		ssize_t lvl = list_height;
		const size_t new_h = random_height();
		node_base* update[new_h+1];
		memset(update, 0, sizeof(update));

		while(lvl >= 0)
		{
			while(curr->next_at.size() > lvl &&
					curr->next_at[lvl] != nullptr &&
					((node*)curr->next_at[lvl])->key < k)
			{
				curr = (node*) curr->next_at[lvl];
			}
			update[lvl] = curr;
			lvl--;
		}
		curr = (node*) curr->next_at[0];

		if(curr == nullptr || ((node*)curr)->key != k)
		{
			list_length++;
			node* new_node = new node(k, val);
			new_node->prev = update[0];

			lvl = new_h;
			while(lvl > list_height)
			{
				update[lvl] = &head;
				lvl--;
				list_height++;
			}

			lvl = 0;
			while(lvl <= list_height)
			{
				if(lvl < update[lvl]->next_at.size() && update[lvl]->next_at[lvl] != nullptr)
				{
					if(lvl >= new_node->next_at.size())
						new_node->next_at.push_back(update[lvl]->next_at[lvl]);
					else
						new_node->next_at[lvl] = update[lvl]->next_at[lvl];

					((node*)new_node->next_at[lvl])->prev = new_node;
				}

				if(lvl >= update[lvl]->next_at.size())
				{
					update[lvl]->next_at.push_back(new_node);
				}
				else
				{
					update[lvl]->next_at[lvl] = new_node;
				}

				lvl++;
			}

			return new_node;
		}
		return (node*) curr;
	}
};


int main()
{
	srand(time(0));
	cout << sizeof(smap<int, string>) << endl;
	smap<int, string> m;

	auto p5 = m.insert(5, "isa5");
	auto p6 = m.insert(6, "isa6");
	auto p7 = m.insert(7, "isa7");
	auto p1 = m.insert(1, "isa");
	auto p2 = m.insert(3, "isa3");
	auto p10 = m.insert(10, "isa10");
	auto p3 = m.insert(2, "isa2");
	auto p12 = m.insert(12, "isa12");
	auto p4 = m.insert(4, "isa4");
	auto p8 = m.insert(8, "isa8");
	auto p9 = m.insert(9, "isa9");
	auto p13 = m.insert(13, "isa13");
	auto p11 = m.insert(11, "isa11");


}