//
// Created by Isa on 3/23/21.
//

#ifndef _SKIPLIST_NODE_H
#define _SKIPLIST_NODE_H
#include <vector>

namespace isa
{
	namespace detail
	{
		struct skiplist_node_base
		{
			std::vector<skiplist_node_base*> m_next;
			skiplist_node_base* m_prev;

			skiplist_node_base()
				: m_next(1, nullptr)
				, m_prev(nullptr)
			{
			}

			// must only called on header
			skiplist_node_base(skiplist_node_base&& rval)
				: m_next(std::move(rval.m_next))
				, m_prev(rval.m_prev)
			{
				rval.m_prev = nullptr;
				rval.m_next.emplace_back(nullptr);
			}

			// max height list ever had
			inline size_t m_max_height() const noexcept
			{
				return m_next.size();
			}

			inline skiplist_node_base* next_at(size_t lvl)
			{
				return lvl < m_next.size() ? m_next[lvl] : nullptr;
			}

			void set_next_at(size_t lvl, skiplist_node_base* set_node)
			{
/*
				if(set_node)
					return ;
*/
				if(lvl < m_next.size())
				{
					m_next[lvl] = set_node;
				}
				else
				{
					m_next.emplace_back(set_node);
				}
			}
		};


		template<typename Key, typename Tp>
		struct skiplist_node : public skiplist_node_base
		{
			std::pair<Key, Tp> m_data;

			inline std::pair<Key, Tp>* m_dataptr()
			{
				return std::addressof(m_data);
			}

			inline std::pair<Key, Tp>* m_dataptr() const
			{
				return std::addressof(m_data);
			}

			inline Key* m_keyptr()
			{
				return std::addressof(m_data.first);
			}
			inline Key const* m_keyptr() const
			{
				return std::addressof(m_data.first);
			}

/*
			inline Tp* m_valptr()
			{
				return std::addressof(m_data.second);
			}
			inline Tp const* m_valptr() const
			{
				return std::addressof(m_data.second);
			}
*/

		};



		struct map_node_header : public skiplist_node_base
		{
		private:
			std::vector<skiplist_node_base*> m_prev_at;
			size_t m_length;

			// count additional levels (not counting bottom level)
			size_t m_height;

		public:
			map_node_header()
				: m_prev_at(0, nullptr)
				, m_length(0)
				, m_height(0)
			{
			}

			map_node_header(map_node_header&& rval)
				: skiplist_node_base(std::move(rval))
				, m_length(rval.m_length)
				, m_height(rval.m_height)
			{
				rval.m_length = 0;
				rval.m_height = 0;
			}

			inline size_t m_size() const noexcept
			{
				return m_length;
			}

/*
			inline void m_inc_size(size_t n) noexcept
			{
				m_length += n;
			}

			inline void m_dec_size(size_t n) noexcept
			{
				m_length -= n;
			}
*/
			inline size_t m_curr_height() const noexcept
			{
				return m_height;
			}
/*
			inline void m_inc_height(size_t n) noexcept
			{
				m_height += n;
			}

			inline void m_dec_height(size_t n) noexcept
			{
				m_height -= n;
			}
*/

			// requires Comparator to accept skiplist_node_base* as 1st argument and const Key& as 2nd
			template<typename Key, typename Comparator>
			skiplist_node_base* m_find_node(const Key& key, skiplist_node_base* update[], Comparator& less/*const Key& (*get_key)(skiplist_node_base*), bool (*is_less)(const Key&, const Key&)*/)
			{
				skiplist_node_base* curr = this;
				ssize_t curr_lvl = m_height;

				while(curr_lvl >= 1)
				{
					while(/*curr_lvl < curr->m_max_height() &&*/
							curr->next_at(curr_lvl) != nullptr &&
							less(curr->next_at(curr_lvl), key)
						)
					{
						curr = curr->next_at(curr_lvl);
					}
					update[curr_lvl] = curr;
					curr_lvl--;
				}
				// for lvl 0
				while(curr->m_next[0] != nullptr && less(curr->m_next[0], key))
				{
					curr = curr->m_next[0];
				}
				update[0] = curr;


				curr = curr->m_next[0];

				return curr;
			}

			skiplist_node_base* m_insert_node(skiplist_node_base* node, size_t node_height, skiplist_node_base** update)
			{
				// complete initialize update
				const size_t old_height = m_height;

				while(node_height > old_height)
				{
					update[node_height] = this;
					node_height--;
					m_height++;
				}

				// link at lvl 0
				node->m_prev = update[0];
				node->m_next[0] = update[0]->m_next[0];
				update[0]->m_next[0] = node;
				if(node->m_next[0] != nullptr)
				{
					node->m_next[0]->m_prev = node;
				}

				// link at lvls >= 1
				size_t lvl = 1;
				while(lvl <= old_height)
				{
					node->set_next_at(lvl, update[lvl]->next_at(lvl));
					update[lvl]->set_next_at(lvl, node);
					lvl++;
				}

				m_length++;
				return node;
			}

			// removes nodes in range [F, L)
			void m_remove_range(skiplist_node_base* F, skiplist_node_base* L)
			{
//				skiplist_node_base* f_upd[m_max_height() + 1];
//				skiplist_node_base* l_upd[m_max_height() + 1];
//				skiplist_node_base* first = m_find_node(F, f_upd);
//				skiplist_node_base* last = m_find_node(L, l_upd);

//				last->m_prev = first->m_prev;	//
//
//				size_t lvl = 0;
//				while(lvl < m_height)
//				{
//					f_upd[lvl]->m_next[lvl] = l_upd[lvl]->m_next[lvl];
//					lvl++;
//				}
//				f_upd[0]->m_next[0] = l_upd[0]->m_next[0];
//				f_upd[1]->m_next[1] = l_upd[1]->m_next[1];
//				f_upd[2]->m_next[2] = l_upd[2]->m_next[2];
//				f_upd[3]->m_next[3] = l_upd[3]->m_next[3];
			}

			skiplist_node_base* m_append_node(skiplist_node_base* node, size_t node_height)
			{
				this->m_next[0] = this->m_next[0] ? this->m_next[0] : node; // improve (first append)

				node->m_prev = this->m_prev ? this->m_prev : this; // improve
				this->m_prev = node;

				for(int i = 1; i < m_prev_at.size() && i <= node_height; ++i)
				{
					m_prev_at[i]->set_next_at(i, node);
					m_prev_at[i] = node;
				}
				while(node_height > m_prev_at.size())
				{
					m_prev_at.emplace_back(node);
					--node_height;
				}
				return node;
			}

			skiplist_node_base* m_remove_node(skiplist_node_base* node, skiplist_node_base** update)
			{
				update[0]->m_next[0] = node->m_next[0];

				if(update[0]->m_next[0] != nullptr)
				{
					update[0]->m_next[0]->m_prev = update[0];
				}

				size_t lvl = 1;
				ssize_t height = m_height;
				while(lvl <= height && update[lvl]->next_at(lvl) == node)
				{
					update[lvl]->set_next_at(lvl, node->next_at(lvl));

					if(update[lvl]->next_at(lvl) != nullptr)
					{
						update[lvl]->next_at(lvl)->m_prev = update[lvl];
					}

					lvl++;
				}

				while(m_height > 0 && this->next_at(m_height) == nullptr)
				{
					m_height--;
				}

				m_length--;
				return update[0]->m_next[0];
			}

			void swap(map_node_header& a, map_node_header& b)
			{

			}

		};




	}
}


#endif //_SKIPLIST_NODE_H
