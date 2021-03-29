//
// Created by Isa on 3/26/21.
//

#ifndef _SKIPLIST_NODE_H
#define _SKIPLIST_NODE_H
#include <cstddef>
#include <memory>

namespace isa
{
	namespace detail
	{
		class skiplist_node_base
		{
		public:
			enum constants { MIN_NEXT_SIZE = 11, MAX_ADDITIONAL_LEVELS = 10 };

			skiplist_node_base* m_next[MIN_NEXT_SIZE];

			skiplist_node_base() = default;

			explicit skiplist_node_base(std::nullptr_t)
				: m_next()
			{
			}

			void set_next(size_t level, skiplist_node_base* next)
			{
				m_next[level] = next;
			}

			inline skiplist_node_base* get_next(size_t level)
			{
				return level < MIN_NEXT_SIZE ? m_next[level] : nullptr;
			}

			inline skiplist_node_base const* get_next(size_t level) const
			{
				return level < MIN_NEXT_SIZE ? m_next[level] : nullptr;
			}

			inline skiplist_node_base** m_next_data()
			{
				return m_next;
			}

		};


		template<typename Key, typename Tp>
		class skiplist_node : public skiplist_node_base
		{
			using pair_t = std::pair<Key const, Tp>;

		protected:

			skiplist_node_base* m_prev[1]; // TEMP FIX so end() Iterator could move back: same offset as head's m_tail;
			pair_t m_data; // min 2 bytes <char, char>

		public:

			inline void set_prev(skiplist_node_base* prev) noexcept
			{
				m_prev[0] = prev;
			}

			inline skiplist_node_base const* get_prev() const noexcept
			{
				return m_prev[0];
			}

			inline skiplist_node_base* get_prev() noexcept
			{
				return m_prev[0];
			}

			inline pair_t* dataptr()
			{
				return std::addressof(m_data);
			}

			inline pair_t const* dataptr() const
			{
				return std::addressof(m_data);
			}

			inline Key const* keyptr() const
			{
				return std::addressof(m_data.first);
			}
		};



		template<typename Key, typename Tp>
		class skiplist_impl : public skiplist_node_base
		{
		protected:
			using node_base = skiplist_node_base;
			using node = skiplist_node<Key, Tp>;

			void init()
			{
				for(size_t i = 0; i < MIN_NEXT_SIZE; ++i)
				{
					m_next[i] = m_tail[i] = this;
				}
				m_size = m_height = 0;
			}

		public:
			skiplist_node_base* m_tail[MIN_NEXT_SIZE];
			size_t m_size;
			size_t m_height; // count additional levels

			skiplist_impl()
				: m_size(0)
				, m_height(0)
			{
				init();
			}

			skiplist_impl(skiplist_impl&& rval)
				: m_size(rval.m_size)
				, m_height(rval.m_height)
			{
				if(rval.m_next[0] == std::addressof(rval)) // empty
				{
					init();
				}
				else
				{
					steal_nodes(std::move(rval));
				}
			}

			void steal_nodes(skiplist_impl&& rval)
			{
				for(size_t i = 0; i <= MAX_ADDITIONAL_LEVELS; ++i)
				{
					set_next(i, rval.get_next(i));

					m_tail[i] = rval.m_tail[i];
					m_tail[i]->set_next(i, this);
				}
				static_cast<node*> (m_next[0])->set_prev(this);

				rval.init();
			}

			inline node_base const* npos() const
			{
				return this;
			}

			inline skiplist_node_base const* get_prev() const noexcept
			{
				return m_tail[0];
			}

			inline skiplist_node_base* get_prev() noexcept
			{
				return m_tail[0];
			}

			template<typename Comparator>
			node_base* find_node(const Key& key, const Comparator& comp, node_base** update) const
			{
				node_base const* head = static_cast<node_base const*> (this);
				node_base* curr = const_cast<node_base*> (head);
				ssize_t lvl = m_height;

				while(lvl >= 0)
				{
					while(curr->get_next(lvl) != npos() &&
						comp(*static_cast<node*>(curr->get_next(lvl))->keyptr(), key))
					{
						curr = curr->get_next(lvl);
					}
					update[lvl] = curr;
					--lvl;
				}

				return curr->m_next[0];
			}

			// not needed if head->m_prev is initialized to point to head
			void append_first(node* node, size_t height) // height must be max 10
			{
				// 1 case first node
				node->set_prev(this);
				node->set_next(0, this);
				m_tail[0] = m_next[0] = node;

				size_t lvl = 1;
				while(lvl <= height)
				{
					this->set_next(lvl, node);
					node->set_next(lvl, this);
					m_tail[lvl] = node;
					lvl++;
				}
				m_height = height;
			}

			void append_node(node* new_node, size_t height)
			{
				// 2 case not first node
				this->m_tail[0]->m_next[0] = new_node;
				new_node->set_prev(this->m_tail[0]);
				new_node->m_next[0] = this;
				this->m_tail[0] = new_node;

				size_t lvl = 1;
				while(lvl <= height)
				{
					this->m_tail[lvl]->set_next(lvl, new_node);
					new_node->set_next(lvl, this);
					this->m_tail[lvl] = new_node;
					lvl++;
				}
				if(height > m_height)
				{
					m_height = height;
				}
			}

			void insert_node(node* node, size_t node_height, node_base** update)
			{
				//node_base* head = const_cast<node_base*> (this);
				size_t list_height = m_height;
				size_t lvl = node_height;

				while(lvl > list_height)
				{
					update[lvl] = this;
					lvl--;
					m_height++;
				}

				node->m_next[0] = update[0]->m_next[0];
				node->set_prev(update[0]);
				node->get_prev()->m_next[0] = node; // update[0]->m_next[0] = node;

				lvl = 1;
				while(lvl <= m_height)
				{
					node->set_next(lvl, update[lvl]->get_next(lvl));
					update[lvl]->set_next(lvl, node);
					lvl++;
				}
			}


			void remove_node(node* node, size_t height, node_base** update)
			{
				node_base const* head = static_cast<node_base const*> (this);
				node->m_prev->m_next[0] = node->m_next[0];

				size_t lvl = 0;
				while(lvl <= m_height && update[lvl]->m_next[lvl] == node)
				{
					update[lvl]->set_next(lvl, node->get_next(lvl));
					lvl++;
				}

				while(m_height > 0 && head->get_next(m_height) == nullptr)
				{
					m_height--;
				}
			}

			// remove [first, last)
			void remove_range(node* first, node* last, node_base** update_first, node_base** update_last)
			{
				last->m_prev = first->m_prev;
				first->m_prev->m_next[0] = last; // update_first[0]->m_next[0] = update_last[0]->m_next[0];

				size_t lvl = 1;
				while(lvl <= m_height)
				{
					update_first[lvl]->set_next(lvl, update_last[lvl]->get_next(lvl));
					lvl++;
				}
			}

			void swap(skiplist_impl<Key, Tp>& b)
			{
				std::swap(*this, b);
			}

		};

	}
}


#endif //_SKIPLIST_NODE_H
