#pragma once

#include "map_node.h"

namespace isa
{

	template<typename Key, typename Tp, typename Alloc, typename Less>
	class map_base
	{
	protected:
		using node = details::map_node<Key, Tp>;
		using node_base = details::map_node_base;

		using node_alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;
		using node_alloc_traits = std::allocator_traits<node_alloc_type>;


		struct map_impl : public node_alloc_type
		{
			details::map_node_header head;

//			map_impl() noexcept(std::is_nothrow_default_constructible<node_alloc_type>::value)
//				: node_alloc_type()
//			{
//			}
//
//			map_impl(const node_alloc_type& other_base) noexcept
//				: node_alloc_type(other_base)
//			{
//			}
//
//			map_impl(map_impl&&) = default;
//
//			map_impl(node_alloc_type&& rval_alloc, map_impl&& rval)
//				: node_alloc_type(std::move(rval_alloc))
//				, head(std::move(rval.head))
//			{
//			}
//
//			map_impl(node_alloc_type&& rval_base) noexcept
//				: node_alloc_type(std::move(rval_base))
//			{
//			}
		};

		map_impl m_impl;

		typename node_alloc_traits::pointer m_alloc_node()
		{
			return node_alloc_traits::allocate(m_impl, 1);
		}

		void m_dealloc_node(typename node_alloc_traits::pointer p) noexcept
		{
			node_alloc_traits::deallocate(m_impl, p, 1);
		}

		// helper to work with node_base
		static Key& s_get_key(details::map_node_base* n)
		{
			return *static_cast<node*>(n)->m_keyptr();
		}

		// comparator wrapper
		static inline bool s_less(node_base* node, const Key& key)
		{
			return Less() (s_get_key(node), key);
		}

		static inline bool s_greater(node_base* node, const Key& key)
		{
			return Less() (key, s_get_key(node));
		}

		static inline bool s_equal(node_base* node, const Key& key)
		{
			return !s_less(node, key) && !s_greater(node, key);
		}

		// wrap around node_header's functions
		inline size_t m_levels()
		{
			return m_impl.head.m_levels();
		}

		inline size_t m_height()
		{
			return m_impl.head.m_height();
		}

		inline node* m_find_node(const Key& key, node* update[])
		{
			return (node*) m_impl.head.m_find_node(key, reinterpret_cast<node_base**>(update), s_less);
		}

		inline void m_complete_update(size_t new_height, node* update[])
		{
			m_impl.head.m_complete_update(new_height, reinterpret_cast<node_base**>(update));
		}

		inline node_base* m_link_node(node_base* nod, size_t new_h, node* update[])
		{
			return m_impl.head.m_link_node(nod, new_h, reinterpret_cast<node_base**>(update));
		}

		inline node_base* m_unlink_node(node_base* nod, node* update[])
		{
			return m_impl.head.m_unlink_node(nod, reinterpret_cast<node_base**>(update));
		}

	public:
		using allocator_type = Alloc;

		// returns reference to instance of used allocator
		node_alloc_type& m_get_node_allocator() noexcept
		{
			return m_impl;
		}

		const node_alloc_type& m_get_node_allocator() const noexcept
		{
			return m_impl;
		}

		map_base() = default;


		~map_base() noexcept
		{
			//m_clear();
		}

	};




	template<
			typename Key,
			typename Tp,
			typename Compare = std::less<Key>,
			typename Alloc = std::allocator<std::pair<const Key, Tp>>
		>
	class map : public map_base<Key, Tp, Alloc, Compare>
	{
		using base = map_base<Key, Tp, Alloc, Compare>;

		using node_alloc_type = typename base::node_alloc_type;
		using node_alloc_traits = typename base::node_alloc_traits;

		using node = details::map_node<Key, Tp>;
		using pair = std::pair<const Key, Tp>;

		using pair_alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<pair>;
		using pair_alloc_traits = typename std::allocator_traits<pair_alloc_type>;

	protected:
		using base::m_impl;
		using base::m_alloc_node;
		using base::m_dealloc_node;
		using base::m_get_node_allocator;
		using base::m_find_node;
		using base::m_complete_update;
		using base::m_link_node;
		using base::m_levels;

	public:
		using key_type = Key;
		using mapped_type = Tp;
		using value_type = pair;
		using key_compare = Compare;
		using allocator_type = Alloc;

		using reference = value_type&;
		using const_referece = value_type const&;

		using pointer = typename pair_alloc_traits::pointer;
		using const_pointer = typename pair_alloc_traits::const_pointer;

		using deifference_type = ptrdiff_t;
		using size_type = size_t;


		inline size_t random_lvl(size_t max_height)
		{
			size_t res = 0;
			while(res <= max_height && rand() < RAND_MAX / 2)
			{
				res++;
			}

			return res;
		}




	public:
		map() = default;


		bool erase(const key_type& key)
		{
			node* update[base::m_levels() + 1];
			memset(update, 0, sizeof(update));

			node* pos = base::m_find_node(key, update);

			if(pos != nullptr && base::s_equal(pos, key))
			{
				base::m_unlink_node(pos, update);
				m_delete_node(pos);
				return true;
			}
			return false;
		}


		node* insert(const value_type& pair)
		{
			const key_type& key = pair.first;
			node* update[base::m_levels() + 1]; //  +1 in case grow up

			node* pos = base::m_find_node(key, update);

			if(pos == nullptr || !base::s_equal(pos, key))
			{
				// create
				node* new_node = m_create_node(pair);

				size_t new_height = random_lvl(m_impl.head.m_height());
				base::m_complete_update(new_height, update);

				base::m_link_node(new_node, new_height, update);

				return new_node;
			}
			else
			{
				// update
				return pos;
			}
		}


		// allocates node and constructs new object(elemnt) in allocated memory
		template<typename... Args>
		node* m_create_node(Args&&... args)
		{
			// calls allocator_traits::allocate(m_impl, 1)
			node* p = base::m_alloc_node();
			// return m_impl
			node_alloc_type& alloc = base::m_get_node_allocator();
			// allocated_ptr<node_alloc_type> guard{alloc, p};
			node_alloc_traits::construct(alloc, p, std::forward<Args>(args)...);
			// guard = nullptr;
			return p;
		}

		void m_delete_node(node* node)
		{
			node_alloc_traits::destroy(base::m_get_node_allocator(), node->m_dataptr());
			base::m_dealloc_node(node);
		}

	};


}








