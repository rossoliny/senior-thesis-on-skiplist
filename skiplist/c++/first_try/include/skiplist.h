#pragma once
#define USE_C_RAND
#include "skiplist_node.h"
#include <random>
#include "gcc_temp_utils.h"
#include "skiplist_iterator.h"

namespace isa
{

	template<typename Key, typename Tp, typename Alloc, typename Comparator>
	class map_base
	{
	protected:
		using node = detail::skiplist_node<Key, Tp>;
		using node_base = detail::skiplist_node_base;

		using node_alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<node>;
		using node_alloc_traits = std::allocator_traits<node_alloc_type>;

		// helper to extract key from node_base
		static const Key& s_get_key(const node_base* n)
		{
			return *(static_cast<const node*>(n))->m_keyptr();
		}

		struct map_impl : public node_alloc_type
		{
			detail::map_node_header m_head;

			map_impl() noexcept(std::is_nothrow_default_constructible<node_alloc_type>::value)
				: node_alloc_type()
			{
			}

			explicit map_impl(const node_alloc_type& alloc) noexcept
				: node_alloc_type(alloc)
			{
			}

			map_impl(map_impl&&) = default;
//
//			map_impl(node_alloc_type&& rval_alloc, map_impl&& rval)
//				: node_alloc_type(std::move(rval_alloc))
//				, head(std::move(rval.head))
//			{
//			}
//
			explicit map_impl(node_alloc_type&& alloc) noexcept(std::is_nothrow_move_constructible<node_alloc_type>::value)
				: node_alloc_type(std::move(alloc))
			{
			}

		};

		struct node_comparator_type : public Comparator
		{
			node_comparator_type(const node_comparator_type&) noexcept(std::is_nothrow_copy_constructible<Comparator>::value) = default;
			node_comparator_type(node_comparator_type&&) noexcept(std::is_nothrow_move_constructible<Comparator>::value) = default;

			node_comparator_type() = default;

			explicit node_comparator_type(const Comparator& cmp)
				: Comparator(cmp)
			{
			};

			bool operator()(const node_base* n, const Key& b)
			{
				return Comparator::operator()(s_get_key(n), b);
			}

			bool operator()(const node_base* n, const Key& b) const
			{
				return Comparator::operator()(s_get_key(n), b);
			}
		};

		map_impl m_impl;
		node_comparator_type m_comparator;

		typename node_alloc_traits::pointer m_get_node()
		{
			node_base* p = node_alloc_traits::allocate(m_impl, 1);
			node_alloc_traits::construct(m_impl, p);
			return static_cast<typename node_alloc_traits::pointer>(p);
		}

		void m_put_node(typename node_alloc_traits::pointer p) noexcept
		{
			node_alloc_traits::destroy(m_impl, p);
			node_alloc_traits::deallocate(m_impl, p, 1);
		}

		// m_comparator wrappers
		inline bool m_less(node_base* node, const Key& key)
		{
			return m_comparator(node, key);
		}

		inline bool m_greater(node_base* node, const Key& key)
		{
			return static_cast<Comparator> (m_comparator) (key, s_get_key(node));
		}

		inline bool m_equal(node_base* node, const Key& key)
		{
			return !m_less(node, key) && !m_greater(node, key);
		}

		// wrap around node_header's functions
		inline size_t m_max_height()
		{
			return m_impl.m_head.m_max_height();
		}

		inline size_t m_curr_height()
		{
			return m_impl.m_head.m_curr_height();
		}

		inline node* m_find_node(const Key& key, node* update[])
		{
			return (node*) m_impl.m_head.m_find_node(key, reinterpret_cast<node_base**>(update), m_comparator);
		}

		inline node_base* m_insert_node(node_base* nod, size_t new_h, node** update)
		{
			return m_impl.m_head.m_insert_node(nod, new_h, reinterpret_cast<node_base**>(update));
		}

		inline node_base* m_remove_node(node_base* nod, node* update[])
		{
			return m_impl.m_head.m_remove_node(nod, reinterpret_cast<node_base**>(update));
		}

		inline node_base* m_append_node(node_base* nod, size_t node_height)
		{
			return m_impl.m_head.m_append_node(nod, node_height);
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
		map_base(map_base&&) = default;


		// constructs list_base with provided node_allocator
		explicit map_base(const node_alloc_type& alloc)
			: m_impl(alloc) // calls allocator_type's ctor and passes args there
		{
		}



//		// if not inline version
//		list_base(list_base&& rval, node_alloc_type&& rval_alloc)
//		: m_impl(std::move(rval_alloc)) // calls allocator_type's ctor and passes args there
//		{
//			if (rval.m_get_node_allocator() == m_get_node_allocator())
//				m_move_nodes(std::move(rval));
//			// else caller must move individual elements.
//		}
//
//		// Used when allocator is_always_equal is true.
//		//
//		list_base(node_alloc_type&& impl, list_base&& other)
//		: m_impl(std::move(impl), std::move(other.m_impl)) // calls allocator_type's ctor and passes args there
//		{
//		}
//
		// Used when allocator is_always_equal is false.
		explicit map_base(node_alloc_type&& alloc, node_comparator_type&& cmp = node_comparator_type())
			: m_impl(std::move(alloc)) // calls allocator_type's ctor and passes args there
			, m_comparator(std::move(cmp))
		{
		}


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

		using node = detail::skiplist_node<Key, Tp>;
		using pair_t = std::pair<const Key, Tp>;

		using node_compare_type = typename base::node_comparator_type;
/*
		using pair_alloc_type = typename std::allocator_traits<Alloc>::template rebind_alloc<pair_t>;
		using pair_alloc_traits = typename std::allocator_traits<pair_alloc_type>;
*/

	protected:
		using base::m_impl;
		using base::m_get_node;
		using base::m_put_node;
		using base::m_get_node_allocator;
		using base::m_find_node;
		using base::m_insert_node;
		using base::m_max_height;

	public:
		using key_type = Key;
		using mapped_type = Tp;
		using value_type = pair_t;
		using key_compare = Compare;
		using allocator_type = Alloc;

		using reference = value_type&;
		using const_referece = value_type const&;

		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

		using iterator = skiplist_iterator<Key, Tp>;
		using const_iterator = skiplist_const_iterator<Key, Tp>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using deifference_type = typename std::iterator_traits<iterator>::difference_type;
		using size_type = std::size_t;


		inline size_t random_level(size_t max_height)
		{
#ifndef USE_C_RAND
/*			float p = 0.5f;
			std::random_device seed;
			std::default_random_engine e(seed());
			std::geometric_distribution<size_type> d(p);

			return d(e);
*/
#else
			size_t res = 0;
			while(res <= max_height && rand() < RAND_MAX / 2)
			{
				res++;
			}

			return res;
#endif
		}




	public:
//		map() = default;

		explicit map(const key_compare& comparator = key_compare(), const allocator_type& allocator = allocator_type())
			: base(node_alloc_type(allocator), node_compare_type(comparator))
		{
		}

		explicit map(const allocator_type& allocator)
			: base(node_alloc_type(allocator))
		{
		}

		template<typename Input_iterator, typename = gcc::require_input_iter<Input_iterator>>
		map(Input_iterator first, Input_iterator last, const key_compare& comparator = key_compare(), const allocator_type& allocator = allocator_type())
			: base(node_alloc_type(allocator), node_compare_type(comparator))
		{
			//TODO: impl init
			m_range_initialize(first, last);
		}

		map(const map& other)
			: base(node_alloc_traits::select_on_container_copy_construction(other.m_get_node_allocator()), node_compare_type(other.m_comparator))
		{
			//TODO: impl copy
		}

		map(const map& other, const allocator_type& alloc)
			: base(node_alloc_type(alloc), node_compare_type(other.m_comparator))
		{
			//TODO: impl copy
		}

		map(map&& x) = default;

		map(map&& x, const allocator_type& alloc)
		{
		}

		// iterator functions

		iterator begin() noexcept
		{
			return iterator(this->m_impl.m_head.m_next[0]);
		}

		const_iterator begin() const noexcept
		{
			return const_iterator(this->m_impl.m_head.m_next[0]);
		}

		iterator end() noexcept
		{
			return iterator(&this->m_impl.m_head);
		}

		const_iterator end() const noexcept
		{
			return const_iterator(&this->m_impl.m_head);
		}

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		const_iterator cbegin() const noexcept
		{
			return const_iterator(this->m_impl.m_head.m_next);
		}

		const_iterator cend() const noexcept
		{
			return const_iterator(&this->m_impl.m_head);
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		bool erase(const key_type& key)
		{
			node* update[base::m_max_height() + 1];
			memset(update, 0, sizeof(update));

			node* pos = base::m_find_node(key, update);

			if(pos != nullptr && base::m_equal(pos, key))
			{
				base::m_remove_node(pos, update);
				m_delete_node(pos);
				return true;
			}
			return false;
		}


		node* insert(const value_type& pair)
		{
			return m_insert(pair);
		}


		bool empty() const noexcept
		{
			return m_impl.m_head.m_next[0] == nullptr;
		}

	protected:

		// Called by range constructor
		// check for input iterator is done by caller
		template<typename Input_iterator>
		void m_range_initialize(Input_iterator start, Input_iterator stop)
		{
			if(start != stop)
			{
				m_append(pair_t(*start));
			}

			++start;
			for(; start != stop; ++start)
			{
				if((*start).first >= (*--end()).first)
				{
					m_append(pair_t(*start));
				}
				else
				{
					m_insert(pair_t(*start));
				}
			}
		}


		node* m_insert(const value_type& pair)
		{
			const key_type& key = pair.first;
			node* update[base::m_max_height() + 1]; //  +1 in case grow up

			node* pos = base::m_find_node(key, update);

			if(pos == nullptr || !base::m_equal(pos, key))
			{
				// create
				node* new_node = m_create_node(pair);

				size_t new_height = random_level(m_impl.m_head.m_curr_height());

				base::m_insert_node(new_node, new_height, update);

				return new_node;
			}
			else
			{
				// update
				return pos;
			}
		}

		template<typename... Args>
		void m_append(Args&&... args)
		{
			node* new_node = m_create_node(std::forward<Args>(args)...);
			size_t node_height = random_level(m_impl.m_head.m_curr_height());
			base::m_append_node(new_node, node_height);
//			this->m_inc_size(1);
		}



		// allocates node and constructs new object(elemnt) in allocated memory
		template<typename... Args>
		node* m_create_node(Args&&... args)
		{
			// calls allocator_traits::allocate(m_impl, 1)
			node* p = base::m_get_node();
			// return m_impl
			node_alloc_type& alloc = base::m_get_node_allocator();
			// allocated_ptr<node_alloc_type> guard{alloc, p};
			node_alloc_traits::construct(alloc, p->m_dataptr(), std::forward<Args>(args)...);
			// guard = nullptr;
			return p;
		}

		void m_delete_node(node* node)
		{
			node_alloc_traits::destroy(base::m_get_node_allocator(), node->m_dataptr());
			base::m_put_node(node);
		}

	};


}








