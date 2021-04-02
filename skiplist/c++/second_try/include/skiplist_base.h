//
// Created by Isa on 3/29/21.
//

#ifndef _SKIPLIST_BASE_H
#define _SKIPLIST_BASE_H
#define USE_C_RAND

#include "skiplist_node.h"
#include <random>

namespace isa
{

	template<typename Key, typename Tp, typename Alloc, typename Comparator>
	class map_base
	{
		enum
		{
			MAX_ADDITIONAL_LEVELS = detail::skiplist_node_base::MAX_ADDITIONAL_LEVELS
		};
	protected:
		using node_base = detail::skiplist_node_base;
		using node_header = detail::skiplist_impl<Key, Tp>;
		using node = detail::skiplist_node<Key, Tp>;

		using node_alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;
		using node_alloc_traits = std::allocator_traits<node_alloc_type>;
		using node_pointer = typename node_alloc_traits::pointer;
		using node_const_pointer = typename node_alloc_traits::const_pointer;

		template<typename K, typename V>
		using pair_t = std::pair<K, V>;
		using insert_return_t = pair_t<node_pointer, bool>;

		using pair_type = pair_t<Key const, Tp>;
		using mutable_key_pair = pair_t<Key, Tp>;

		struct pair_comparator_type
		{
			Comparator m_key_comparator;

			pair_comparator_type() = default;

			explicit pair_comparator_type(Comparator const& key_comparator)
				: m_key_comparator(key_comparator)
			{
			}

			inline bool operator()(pair_type const& a, pair_type const& b)
			{
				return m_key_comparator(a.first, b.first);
			}

		};

		node_alloc_type m_node_allocator;
		pair_comparator_type m_pair_comparator;
		node_header m_head;

		inline static Key const& _s_node_key(node_base const* node)
		{
			return *static_cast<node_const_pointer> (node)->keyptr();
		}

		inline bool less(Key const& a, Key const& b) const
		{
			return m_pair_comparator.m_key_comparator(a, b);
		}

		inline bool greater(Key const& a, Key const& b) const
		{
			return m_pair_comparator.m_key_comparator(b, a);
		}

		inline bool equals(Key const& a, Key const& b)
		{
			return !less(a, b) && !greater(a, b);
		}

		inline bool equals(node_base const* node, pair_type const& pair)
		{
			return equals(_s_node_key(node), pair.first);
		}

		inline bool equals(node_base const* a, node_base const* b)
		{
			return a == b || equals(_s_node_key(a), _s_node_key(b));
		}

		inline constexpr size_t length() const
		{
			return m_head.m_length;
		}

		inline pair_comparator_type& get_pair_comparator()
		{
			return m_pair_comparator;
		}

		inline pair_comparator_type const& get_pair_comparator() const
		{
			return m_pair_comparator;
		}

		inline Comparator& get_key_comparator()
		{
			return m_pair_comparator.m_key_comparator;
		}

		inline Comparator const& get_key_comparator() const
		{
			return m_pair_comparator.m_key_comparator;
		}

		inline node_alloc_type& get_node_allocator()
		{
			return m_node_allocator;
		}

		inline node_alloc_type const& get_node_allocator() const
		{
			return m_node_allocator;
		}

		inline node_pointer get_node()
		{
			return node_alloc_traits::allocate(m_node_allocator, 1);
		}

		inline void put_node(node_pointer ptr)
		{
			node_alloc_traits::deallocate(m_node_allocator, ptr, 1);
		}

		template<typename... Args>
		node_pointer create_node(Args&& ... args)
		{
			node_pointer ptr = get_node();
			node_alloc_traits::construct(m_node_allocator, ptr->dataptr(), std::forward<Args>(args)...);
			return ptr;
		}

		inline void delete_node(node_base* ptr)
		{
			delete_node(static_cast<node_pointer> (ptr));
		}

		void delete_node(node_pointer ptr)
		{
			node_alloc_traits::destroy(m_node_allocator, ptr->dataptr());
			put_node(ptr);
		}

		template<typename... Args>
		node_pointer do_append_node(Args&&... args)
		{
			node_pointer new_node = create_node(std::forward<Args>(args)...);
			size_t node_height = random_level();
			m_head.append_node(new_node, node_height);
			++m_head.m_length;

			return new_node;
		}

		template<typename... Args>
		node_pointer do_insert_node(node_base* update[], Args&&... args)
		{
			node_pointer new_node = create_node(std::forward<Args>(args)...);
			size_t node_height = random_level();
			m_head.insert_node(new_node, node_height, update);
			++m_head.m_length;

			return new_node;
		}


		template<typename... Args>
		inline void append_first(Args&& ... args)
		{
			node_pointer node = create_node(std::forward<Args>(args)...);
			m_head.append_first(node, random_level());
			++m_head.m_length;
		}

		template<typename... Args>
		bool append(Args&& ... args)
		{
			pair_type data(std::forward<Args>(args)...);
			Key const& last = _s_node_key(m_head.m_tail[0]);

			if(less(last, data.first))
			{
				do_append_node(std::move(data));
				return true;
			}

			return false;
		}

		template<typename... Args>
		insert_return_t insert(Args&& ... args)
		{
			node_base* update[1 + MAX_ADDITIONAL_LEVELS];

			pair_type element(std::forward<Args>(args)...);
			node_base* pos = m_head.find_node(element.first, get_key_comparator(), update);

			if(pos == m_head.npos() || !equals(pos, element))
			{
				node_pointer new_node = do_insert_node(update, std::move(element));
				return insert_return_t(new_node, true);
			}

			return insert_return_t(static_cast<node_pointer> (pos), false);
		}

		template<typename... Args>
		insert_return_t append_or_insert(Args&& ... args)
		{
			pair_type data(std::forward<Args>(args)...);
			Key const& last = _s_node_key(m_head.m_tail[0]);

			if(less(last, data.first))
			{
				node_pointer new_node = do_append_node(std::move(data));

				return insert_return_t(new_node, true);
			}
			if(greater(last, data.first))
			{
				return insert(std::move(data));
			}

			return insert_return_t(static_cast<node_pointer> (m_head.m_tail[0]), false);
		}

		template<typename... Args>
		inline void assign_pair(node_base* node, Args&&... args) const
		{
			static_cast<node_pointer> (node)->mutable_dataptr()->operator=(std::forward<Args>(args)...);
		}


		size_t remove_key(Key const& key)
		{
			char node_buff[sizeof(node)];
			node_pointer node = reinterpret_cast<node_pointer> (node_buff);
			node->mutable_dataptr()->first = key;

			return remove_node(node) != node;
		}

		node_base* remove_node(node_base const* node)
		{
			node_base* update[1 + MAX_ADDITIONAL_LEVELS];
			node_base* pos = m_head.find_node(_s_node_key(node), get_key_comparator(), update);

			if(equals(node, pos))
			{
				node_base* next = pos->m_next[0];
				m_head.remove_node(pos, update);
				delete_node(pos);
				--m_head.m_length;

				return next;
			}
			return const_cast<node_base*> (node);
		}

		node_base* remove_tail(node_base const* begin)
		{
			node_base* update[1 + MAX_ADDITIONAL_LEVELS];
			node_base* first = m_head.find_node(_s_node_key(begin), get_key_comparator(), update);
			node_base* last = std::addressof(m_head);

			if(equals(begin, first))
			{
				m_head.remove_range(first, last, update, m_head.m_tail);

				while(first != last)
				{
					node_base* next = first->m_next[0];
					delete_node(first);
					first = next;
					--m_head.m_length;
				}
				return first;
			}
			return const_cast<node_base*> (begin);
		}

		// begin and end must never be equal to head, it's caller's responsibility to check it
		node_base* remove_range(node_base const* begin, node_base const* end)
		{
			node_base* update1[1 + MAX_ADDITIONAL_LEVELS];
			node_base* update2[1 + MAX_ADDITIONAL_LEVELS];

			node_base* first = m_head.find_node(_s_node_key(begin), get_key_comparator(), update1);
			node_base* last = m_head.find_node(_s_node_key(end), get_key_comparator(), update2);

			if((first == begin && last == end) || (equals(begin, first) && equals(end, last)))
			{
				m_head.remove_range(first, last, update1, update2);
				while(first != last)
				{
					auto next = first->m_next[0];
					delete_node(static_cast<node*> (first));
					first = next;
					--m_head.m_length;
				}
				return first;
			}
			return const_cast<node_base*> (begin);
		}

		template<typename K>
		Tp& find_or_insert(K&& key)
		{
			node_base* update[1 + MAX_ADDITIONAL_LEVELS];
			node_base* pos = m_head.find_node(key, get_key_comparator(), update);

			if(pos == m_head.npos() || !equals(_s_node_key(pos), key))
			{
				pair_type new_element(std::forward<K> (key), Tp());
				return do_insert_node(update, std::move(new_element))->dataptr()->second;
			}

			return static_cast<node_pointer> (pos)->dataptr()->second;
		}

		inline bool is_empty() const
		{
			return m_head.m_next[0] == m_head.npos();
		}

		size_t random_level()
		{
#ifndef USE_C_RAND
			float p = 0.5f;
			std::random_device seed;
			std::default_random_engine e(seed());
			std::geometric_distribution<size_type> d(p);

			return d(e);
#else
			size_t res = 0;
			while(res < MAX_ADDITIONAL_LEVELS && rand() < RAND_MAX / 2)
			{
				res++;
			}

			return res;
#endif
		}

		void clear_nodes() noexcept
		{
			node_pointer curr = static_cast<node_pointer> (m_head.m_next[0]);
			while(curr != m_head.npos())
			{
				node_pointer nod = curr;
				curr = static_cast<node_pointer> (curr->m_next[0]);

				node_alloc_traits::destroy(m_node_allocator, nod->dataptr());
				put_node(nod);
			}
		}

		void init_head() noexcept
		{
			m_head.init();
		}

	public:
		map_base() = default;

		explicit map_base(Comparator const& key_comp)
			: m_pair_comparator(key_comp)
		  	, m_head()
		{
		}

		explicit map_base(Alloc const& alloc)
			: m_node_allocator(alloc)
		  	, m_head()
		{
		}

		map_base(Comparator const& key_comp, Alloc const& alloc)
			: m_node_allocator(alloc)
			, m_pair_comparator(key_comp)
			, m_head()
		{
		}

		map_base(pair_comparator_type const& pair_comp, node_alloc_type&& alloc)
			: m_node_allocator(std::move(alloc))
		  	, m_pair_comparator(pair_comp)
		  	, m_head()
		{
		}

		map_base(pair_comparator_type const& pair_comp, Alloc const& alloc)
			: m_node_allocator(alloc)
		  	, m_pair_comparator(pair_comp)
		  	, m_head()
		{
		}

		map_base(map_base&& rval) = default;


	protected:
		void steal_nodes(map_base&& rval)
		{
			m_head.m_length = rval.m_head.m_length;
			m_head.m_height = rval.m_head.m_height;
			m_head.steal_nodes(std::move(rval.m_head));
		}

	public:

		map_base(map_base&& rval, Alloc const& alloc)
			: m_node_allocator(alloc)
		  	, m_pair_comparator(std::move(rval.m_pair_comparator))
		  	, m_head(std::move(rval.m_head))
		{
		}


		~map_base() noexcept
		{
			clear_nodes();
		}
	};

}


#endif //_SKIPLIST_BASE_H
