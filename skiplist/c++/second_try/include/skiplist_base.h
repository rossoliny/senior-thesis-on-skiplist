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

		using pair_type = std::pair<Key const, Tp>;

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

		size_t length() const
		{
			return m_head.m_size;
		}

		pair_comparator_type& get_pair_comparator()
		{
			return m_pair_comparator;
		}

		pair_comparator_type const& get_pair_comparator() const
		{
			return m_pair_comparator;
		}

		Comparator& get_key_comparator()
		{
			return m_pair_comparator.m_key_comparator;
		}

		Comparator const& get_key_comparator() const
		{
			return m_pair_comparator.m_key_comparator;
		}

		node_alloc_type& get_node_allocator()
		{
			return m_node_allocator;
		}

		node_alloc_type const& get_node_allocator() const
		{
			return m_node_allocator;
		}

		node_pointer get_node()
		{
			return node_alloc_traits::allocate(m_node_allocator, 1);
		}

		void put_node(node_pointer ptr)
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

		template<typename... Args>
		inline void append_first(Args&& ... args)
		{
			node_pointer node = create_node(std::forward<Args>(args)...);
			m_head.append_first(node, random_level());
			++m_head.m_size;
		}

		template<typename... Args>
		bool append(Args&& ... args)
		{
			pair_type data(std::forward<Args>(args)...);
			Key const& last = _s_node_key(m_head.m_tail[0]);

			if(less(last, data.first))
			{
				node_pointer new_node = create_node(std::move(data));
				size_t node_height = random_level();
				m_head.append_node(new_node, node_height);

				++m_head.m_size;
				return true;
			}

			return false;
		}

		template<typename... Args>
		bool insert(Args&& ... args)
		{
			// find node pos
			node_base* update[1 + MAX_ADDITIONAL_LEVELS];
//			memset(update, 0, sizeof(update));

			pair_type element(std::forward<Args>(args)...);
			node_base* pos = m_head.find_node(element.first, get_key_comparator(), update);

			// insert if not found
			if(pos == m_head.npos() || !equals(pos, element))
			{
				node_pointer new_node = create_node(std::move(element));
				m_head.insert_node(new_node, random_level(), update);

				++m_head.m_size;
				return true;
			}
			return false;
		}

		template<typename... Args>
		bool append_or_insert(Args&& ... args)
		{
			pair_type data(std::forward<Args>(args)...);
			Key const& last = _s_node_key(m_head.m_tail[0]);

			if(less(last, data.first))
			{
				node_pointer new_node = create_node(std::move(data));
				size_t node_height = random_level();
				m_head.append_node(new_node, node_height);

				++m_head.m_size;
				return true;
			}

			if(greater(last, data.first))
			{
				return insert(std::move(data));
			}

			return false;
		}

		bool is_empty() const
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
			while(res <= MAX_ADDITIONAL_LEVELS && rand() < RAND_MAX / 2)
			{
				res++;
			}

			return res;
#endif
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
			m_head.m_size = rval.m_head.m_size;
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

	};

}


#endif //_SKIPLIST_BASE_H
