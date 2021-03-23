//
// Created by Isa on 3/23/21.
//

#ifndef _MAP_NODE_H
#define _MAP_NODE_H
#include <vector>

namespace isa
{
	namespace details
	{
		struct map_node_base
		{
			std::vector<map_node_base*> m_next;
			map_node_base* m_prev;

			map_node_base()
				: m_next(1)
				, m_prev(nullptr)
			{
			}

			// total levels including bottom
			inline size_t m_levels() const noexcept
			{
				return m_next.size();
			}

			inline map_node_base* next_at(size_t lvl)
			{
				if(lvl >= m_next.size())
					return nullptr;
				return m_next[lvl];
			}

			map_node_base* set_next_at(size_t lvl, map_node_base* set_node)
			{
				if(!set_node)
					return set_node;

				if(lvl < m_next.size())
				{
					m_next[lvl] = set_node;
				}
				else
				{
					m_next.push_back(set_node);
				}
				return m_next[lvl];
			}
		};


		template<typename Key, typename Tp>
		struct map_node : public map_node_base
		{
			std::pair<Key, Tp> m_data;

			map_node(const std::pair<Key, Tp>& arg)
				: m_data(arg)
			{
			}

			map_node(std::pair<Key, Tp>&& rval)
				: m_data(std::move(rval))
			{
			}

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
			Key const* m_keyptr() const
			{
				return std::addressof(m_data.first);
			}

			Tp* m_valptr()
			{
				return std::addressof(m_data.second);
			}
			Tp const* m_valptr() const
			{
				return std::addressof(m_data.second);
			}

		};



		struct map_node_header : public map_node_base
		{
		private:
			size_t m_length;

			// count additional levels (not counting bottom level)
			size_t _m_height;

		public:
			map_node_header()
				: m_length(0)
				, _m_height(0)
			{
			}

			size_t m_size() const noexcept
			{
				return m_length;
			}

			void m_inc_size(size_t n) noexcept
			{
				m_length += n;
			}

			void m_dec_size(size_t n) noexcept
			{
				m_length -= n;
			}

			size_t m_height() const noexcept
			{
				return _m_height;
			}

			void m_inc_height(size_t n) noexcept
			{
				_m_height += n;
			}

			void m_dec_height(size_t n) noexcept
			{
				_m_height -= n;
			}

			template<typename Key>
			map_node_base* m_find_node(const Key& key, map_node_base* update[], bool (*Compare)(map_node_base*, const Key&))
			{
				map_node_base* curr = this;
				ssize_t curr_lvl = _m_height;

				while(curr_lvl >= 0)
				{
					while(curr_lvl < curr->m_levels() &&
							curr->next_at(curr_lvl) != nullptr &&
							Compare(curr->next_at(curr_lvl), key)
						)
					{
						curr = curr->next_at(curr_lvl);
					}
					update[curr_lvl] = curr;
					curr_lvl--;
				}

				curr = curr->next_at(0);

				return curr;
			}

			void m_complete_update(size_t new_height, map_node_base* update[]) noexcept
			{
				const size_t list_height = _m_height;

				while(new_height > list_height)
				{
					update[new_height] = this;
					new_height--;
					_m_height++;
				}

			}

			map_node_base* m_link_node(map_node_base* node, size_t new_h, map_node_base* update[])
			{
				node->m_prev = update[0];

				const size_t list_height = _m_height;
				size_t lvl = 0;
				while(lvl <= list_height)
				{
					node->set_next_at(lvl, update[lvl]->next_at(lvl));

					update[lvl]->set_next_at(lvl, node);
					lvl++;
				}

//				if(node->next_at(0) != nullptr)
//				{
//					node->next_at(0)->m_prev = node;
//				}


				m_length++;
				return node;
			}

			map_node_base* m_unlink_node(map_node_base* node, map_node_base* update[])
			{
				size_t lvl = 0;

				// If we made list-heights steps down during search
				// and if curr_lvl runs from 0 to list->height
				// Then it is 100% that step_down_point[curr_lvl] != NULL
				ssize_t height = _m_height;
				while(lvl <= height && update[lvl]->next_at(lvl) == node)
				{
					update[lvl]->set_next_at(lvl, node->next_at(lvl));
					lvl++;

					if(_m_height > 0 && next_at(_m_height) == NULL)
					{
						_m_height--;
					}
				}

				m_length--;
				return update[0]->next_at(0);
			}

			void swap(map_node_header& a, map_node_header& b)
			{

			}

		};




	}
}


#endif //_MAP_NODE_H
