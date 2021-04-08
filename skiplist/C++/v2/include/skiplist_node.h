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
			enum constants { MAX_ADDITIONAL_LEVELS = 10 };

#ifdef SKIPLIST_DEBUG_INFO
			size_t height = 0;
#endif
			skiplist_node_base* m_next[1 + MAX_ADDITIONAL_LEVELS];

			skiplist_node_base() = default;

			explicit skiplist_node_base(std::nullptr_t)
				: m_next()
			{
			}

			// use only for levels >= 1
			inline void set_next(size_t level, skiplist_node_base* next)
			{
				m_next[level] = next;
			}

			inline skiplist_node_base* get_next(size_t level)
			{
				return level <= MAX_ADDITIONAL_LEVELS ? m_next[level] : nullptr;
			}

		};


		template<typename Key, typename Tp>
		class skiplist_node : public skiplist_node_base
		{
			using pair_t = std::pair<Key, Tp>;
			using pair_const_key_t = std::pair<Key const, Tp>;

		protected:

			skiplist_node_base* m_prev[1]; // TEMP FIX so end() Iterator could move back: same offset as head's m_tail;
			union
			{
				pair_t m_data; // min 2 bytes <char, char>
				pair_const_key_t m_const_key_data;
			};

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

			inline pair_const_key_t* dataptr()
			{
				return std::addressof(m_const_key_data);
			}

			inline pair_const_key_t const* dataptr() const
			{
				return std::addressof(m_const_key_data);
			}

			inline Key const* keyptr() const
			{
				return std::addressof(m_data.first);
			}

			inline pair_t* mutable_dataptr()
			{
				return std::addressof(m_data);
			}

			inline pair_t const* mutable_dataptr() const
			{
				return std::addressof(m_data);
			}

			inline skiplist_node_base* baseptr()
			{
				return this;
			}

			inline skiplist_node_base const* baseptr() const
			{
				return this;
			}
		};



		template<typename Key, typename Tp>
		class skiplist_impl : public skiplist_node_base
		{
		protected:
			using node_base = skiplist_node_base;
			using node = skiplist_node<Key, Tp>;
			using ssize_t = signed long long int;

			void steal_nodes(skiplist_impl&& rval)
			{
				for(size_t i = 0; i <= MAX_ADDITIONAL_LEVELS; ++i)
				{
					if(rval.m_next[i] == std::addressof(rval))
					{
						this->set_next(i, m_tail[i] = this);
					}
					else
					{
						this->set_next(i, rval.get_next(i));

						m_tail[i] = rval.m_tail[i];
						m_tail[i]->set_next(i, this);
					}
				}
				static_cast<node*> (m_next[0])->set_prev(this);
			}

		public:
			skiplist_node_base* m_tail[1 + MAX_ADDITIONAL_LEVELS];

#if defined(__GNUC__) && !defined(__clang__) && !defined(_MSC_VER)
			volatile size_t m_length;
			volatile size_t m_height;
#else
			size_t m_length;
			size_t m_height; // count additional levels
#endif

			skiplist_impl()
			{
				init_full();
			}

			explicit skiplist_impl(skiplist_impl&& rval)
				: m_length(rval.m_length)
				, m_height(rval.m_height)
			{
				if(rval.m_next[0] == std::addressof(rval)) // empty
				{
					init_full();
				}
				else
				{
					steal_nodes(std::move(rval));

					rval.init_full();
				}
			}

			skiplist_impl& operator=(skiplist_impl&& rval)
			{
				if(rval.m_next[0] == std::addressof(rval)) // empty
				{
					init_full();
				}
				else
				{
					steal_nodes(std::move(rval));
					m_length = rval.m_length;
					m_height = rval.m_height;

					rval.init_full();
				}
				return *this;
			}

			void init_full() noexcept
			{
				for(size_t i = 0; i <= MAX_ADDITIONAL_LEVELS; ++i)
				{
					this->set_next(i, m_tail[i] = this);
				}
				m_length = m_height = 0;
			}

			inline node_base* npos()
			{
				return this;
			}

			inline node_base const* npos() const
			{
				return this;
			}

			template<typename Comparator>
			node* find(const Key& key, const Comparator& comp) const
			{
				std::equal_to<Key> equals;
				node_base const* head = static_cast<node_base const*> (this);
				node_base* curr = const_cast<node_base*> (head);
				ssize_t lvl = m_height;

				while(lvl >= 0)
				{
					while(true)
					{
						if(curr->get_next(lvl) == npos())
							break;

						Key const& k = *static_cast<node*>(curr->get_next(lvl))->keyptr();
						if(!comp(k, key))
						{
							if(equals(k, key))
							{
								return static_cast<node*> (curr->get_next(lvl));
							}
							break;
						}
						curr = curr->get_next(lvl);
					}
					--lvl;
				}

				return static_cast<node*> (curr->m_next[0]);
			}

			template<typename Comparator>
			node_base* find_node(const Key& key, const Comparator& comp, node_base** update) const
			{
				node_base* curr = const_cast<node_base*>(static_cast<node_base const*> (this));
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

			void append_node(node* new_node, size_t height)
			{
				m_tail[0]->m_next[0] = new_node;
				new_node->set_prev(m_tail[0]);
				new_node->m_next[0] = this;
				m_tail[0] = new_node;

				size_t lvl = 1;
				while(lvl <= height)
				{
					m_tail[lvl]->set_next(lvl, new_node);
					new_node->set_next(lvl, this);
					this->m_tail[lvl] = new_node;
					lvl++;
				}
				if(height > m_height)
				{
					m_height = height;
				}
				++m_length;
			}

			void insert_node(node* new_node, size_t const node_height, node_base** update)
			{
				size_t list_height = m_height;
				size_t lvl = node_height;

				while(lvl > list_height)
				{
					update[lvl] = this;
					lvl--;
					m_height++;
				}

				new_node->m_next[0] = update[0]->m_next[0];
				new_node->set_prev(update[0]);
				new_node->get_prev()->m_next[0] = new_node; // update[0]->m_next[0] = new_node;
				static_cast<node*> (new_node->m_next[0])->set_prev(new_node);

				lvl = 1;
				while(lvl <= node_height)
				{
					new_node->set_next(lvl, update[lvl]->get_next(lvl));
					update[lvl]->set_next(lvl, new_node);
					if(new_node->get_next(lvl) == this)
					{
						this->m_tail[lvl] = new_node;
					}

					lvl++;
				}
				++m_length;
			}

			void remove_node(node_base* pos, node_base** update)
			{

				node* nod = static_cast<node*> (pos);

				update[0]->m_next[0] = nod->m_next[0];
				static_cast<node*> (update[0]->m_next[0])->set_prev(update[0]);

				size_t lvl = 1;
				while(lvl <= m_height && update[lvl]->get_next(lvl) == nod)
				{
					update[lvl]->set_next(lvl, nod->get_next(lvl));
					if(update[lvl]->get_next(lvl) == this)
					{
						m_tail[lvl] = update[lvl];
					}
					lvl++;
				}

				while(m_height > 0 && this->get_next(m_height) == npos())
				{
//					m_tail[m_height] = this;
					m_height--;
				}
				--m_length;
			}

			// remove [first, last)
			void remove_range(node_base* begin, node_base* end, node_base** update_first, node_base** update_last)
			{
				node* first = static_cast<node*> (begin);
				node* last = static_cast<node*> (end);

				last->set_prev(first->get_prev());
				first->get_prev()->m_next[0] = last; // update_first[0]->m_next[0] = update_last[0]->m_next[0];

				size_t lvl = 1;
				while(lvl <= m_height)
				{
					update_first[lvl]->set_next(lvl, update_last[lvl]->get_next(lvl));
					if(update_first[lvl]->get_next(lvl) == this)
					{
						this->m_tail[lvl] = update_first[lvl];
					}
					lvl++;
				}
				while(m_height > 0 && this->get_next(m_height) == npos())
				{
//					m_tail[m_height] = this;
					m_height--;
				}
			}

			void swap(skiplist_impl& _other)
			{
				skiplist_impl* other = std::addressof(_other);

				for(int i = 0; i <= MAX_ADDITIONAL_LEVELS; ++i)
				{
					node_base* this_tail = this->m_tail[i];
					this->m_tail[i] = other->m_tail[i];
					other->m_tail[i] = this_tail;

					if(this->m_tail[i] == other->npos())
					{
						this->m_tail[i] = this;
					}
					else
					{
						this->m_tail[i]->set_next(i, this);
					}
					if(other->m_tail[i] == this->npos())
					{
						other->m_tail[i] = other;
					}
					else
					{
						other->m_tail[i]->set_next(i, other);
					}

					node_base* this_next = this->get_next(i);
					this->set_next(i, other->get_next(i));
					other->set_next(i, this_next);

					if(this->get_next(i) == other->npos())
					{
						this->set_next(i, this);
					}
					if(other->get_next(i) == this->npos())
					{
						other->set_next(i, other);
					}
				}

				((node*) other->m_next[0])->set_prev(other);
				((node*) this->m_next[0])->set_prev(this);

				std::swap(this->m_length, other->m_length);
				std::swap(this->m_height, other->m_height);
			}

		};

	}
}


#endif //_SKIPLIST_NODE_H
