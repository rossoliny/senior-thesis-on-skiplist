//
// Created by Isa on 3/26/21.
//
#pragma once
#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include "skiplist_base.h"
#include "skiplist_iterator.h"
#include "container_utils.h"
#include <iostream>


namespace isa
{

	template
		<
			typename Key,
			typename Tp,
			typename Compare = std::less<Key>,
			typename Alloc = std::allocator<std::pair<Key const, Tp>>
		>
	class map : public map_base<Key, Tp, Alloc, Compare>
	{
		using base = map_base<Key, Tp, Alloc, Compare>;

		using node_alloc_type = typename base::node_alloc_type;
		using node_alloc_traits = typename base::node_alloc_traits;

		using node = typename base::node;
		using node_pointer = typename base::node_pointer;
		using pair_type = typename base::pair_type;
		using pair_compare_type = typename base::pair_comparator_type;

	public:
		using key_type = Key;
		using mapped_type = Tp;
		using value_type = pair_type;
		using key_compare = Compare;
		using value_compare = pair_compare_type;
		using allocator_type = Alloc;

		using reference = value_type&;
		using const_referece = value_type const&;

		using pointer = typename std::allocator_traits<allocator_type>::pointer; // ptr to pair_type
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

		using iterator = skiplist_iterator<Key, Tp>;
		using const_iterator = skiplist_const_iterator<Key, Tp>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using difference_type = typename std::iterator_traits<iterator>::difference_type;
		using size_type = std::size_t;

		allocator_type get_allocator() const
		{
			return base::get_node_allocator();
		}

		// iterator getters
		iterator begin() noexcept
		{
			// TODO: fix temp solution
//			if(base::m_head.m_next[0] == base::m_head.npos())
//				return end();
			return iterator(base::m_head.m_next[0]);
		}

		const_iterator begin() const noexcept
		{
			// TODO: fix temp solution
//			if(base::m_head.m_next[0] == base::m_head.npos())
//				return end();
			return const_iterator(base::m_head.m_next[0]);
		}

		iterator end() noexcept
		{
			return iterator(&this->base::m_head);
		}

		const_iterator end() const noexcept
		{
			return const_iterator(&this->base::m_head);
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
			// TODO: fix temp solution
//			if(base::m_head.m_next[0] == base::m_head.npos())
//				return end();
			return const_iterator(base::m_head.m_next[0]);
		}

		const_iterator cend() const noexcept
		{
			return const_iterator(&this->base::m_head);
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(begin());
		}


		void print_map()
		{
			auto it = begin();
			int i = 0;
			while(it != end())
			{
				std::cout << i++ << "\t(" << it->first << ", " << it->second << ")\n";
				++it;
			}
			--it;
			*it;
			std::cout << std::endl;
		}

		void print_map_r()
		{
			auto it = end(); --it;

			int i = 0;
			while(it != end())
			{
				std::cout << i++ << "\t(" << it->first << ", " << it->second << ")\n";
				--it;
			}


			--it;
			*it;
			std::cout << std::endl;
		}

		// constructors

		// empty ctors
		map() = default;



		explicit map(key_compare const& comp)
			: base(comp)
		{
		}

		explicit map(allocator_type const& alloc)
			: base(alloc)
		{
		}

		explicit map(key_compare const& comp, allocator_type const& alloc)
			: base(comp, alloc)
		{
		}

		// copy ctors
		map(map const& other)
			: base(other.get_pair_comparator(), node_alloc_traits::select_on_container_copy_construction(other.get_node_allocator()))
		{
			// 100% sorted
			_p_range_construct_sorted(other.begin(), other.end());
		}

		map(map const& other, allocator_type const& alloc)
			: base(other.get_pair_comparator(), alloc)
		{
			// 100% sorted
			_p_range_construct_sorted(other.begin(), other.end());
		}

		// move ctors
		map(map&& rval) = default;

	protected:
		map(map&& rval, allocator_type const& alloc, std::true_type)
			: base(std::move(rval), alloc)
		{
		}

		map(map&& rval, allocator_type const& alloc, std::false_type)
			: base(alloc)
		{
			if(rval.get_node_allocator() != this->get_node_allocator())
			{
				base::steal_nodes(std::move(rval));
			}
			else
			{
				auto mbegin = utils::make_move_iterator_if_noexcept(rval.begin());
				auto mend = utils::make_move_iterator_if_noexcept(rval.end());

				_p_range_construct_sorted(mbegin, mend);
			}
		}

	public:
		map(map&& rval, allocator_type const& alloc)
			: map(std::move(rval), alloc, std::false_type())
		{
		}

		// optimize: default args must be default constructed (not default constructed and then copied)
		template<typename Input_iterator, typename = utils::require_input_iter<Input_iterator>>
		map(Input_iterator first, Input_iterator last, key_compare const& comp = key_compare(), allocator_type const& alloc = allocator_type())
			: base(comp, alloc)
		{
			// not 100% sorted
			_p_range_construct_unsorted(first, last);
		}

		// initializer list
		map(std::initializer_list<value_type> il, key_compare const& comp, allocator_type const& alloc)
			: base(comp, alloc)
		{
			_p_range_construct_unsorted(il.begin(), il.end());
		}

		map(std::initializer_list<value_type> il, key_compare const& comp)
			: base(comp)
		{
			_p_range_construct_unsorted(il.begin(), il.end());
		}

		map(std::initializer_list<value_type> il)
			: base()
		{
			_p_range_construct_unsorted(il.begin(), il.end());
		}

		map& operator=(map const& other)
		{
			if(this != std::addressof(other))
			{
				if(base::node_alloc_traits::propagate_on_container_copy_assignment::value)
				{
					node_alloc_type this_alloc = this->get_node_allocator();
					node_alloc_type other_alloc = other.get_node_allocator();

					if(!base::node_alloc_traits::is_always_equal::value &&
						this_alloc != other_alloc)
					{
						// replacement allocator cannot free existing storage
						this->clear();
					}
					utils::copy_alloc_on_container_assignment(this_alloc, other_alloc);
				}
				base::m_pair_comparator = other.get_pair_comparator();
				_p_sorted_range_assign_dispatch(other.begin(), other.end(), other.size());
			}

			return *this;
		}

		map& operator=(map&& rval)
		{
			constexpr bool steal_nodes = base::node_alloc_traits::propagate_on_container_move_assignment::value || base::node_alloc_traits::is_always_equal::value;
			_p_move_assign_sorted(std::move(rval), std::integral_constant<bool, steal_nodes> ());
			return *this;
		}

		// capacity
		size_type max_size() const
		{
			return node_alloc_traits::max_size(base::get_node_allocator());
		}

		constexpr size_type size() const
		{
			return base::length();
		}

		bool empty() const
		{
			return base::is_empty();
		}

		~map() noexcept = default;

		void clear() noexcept
		{
			base::clear_nodes();
			base::init_head();
		}

	protected:

		template<typename Input_iterator>
		void _p_range_construct_unsorted(Input_iterator first, Input_iterator last)
		{
			if(first != last)
			{
				base::append_first(*first);
				++first;
			}

			for(; first != last; ++first)
			{
				base::append_or_insert(*first);
			}
		}

		template<typename Input_iterator>
		void _p_range_construct_sorted(Input_iterator first, Input_iterator last)
		{
			// in case when N is known we may balance H to be exactly logN
			// and also perfectly distribute nodes across levels
			// but it can lead to performance degradation
			// in further insert/delete operations
			if(first != last)
			{
				base::append_first(*first);
				++first;
			}

			for(; first != last; ++first)
			{
				base::append(*first);
			}
		}

		template<typename Input_iterator>
		void _p_sorted_range_assign_dispatch(Input_iterator first, Input_iterator last, size_type const count)
		{
/*			constexpr bool const shorter = count < this->size();
			_p_sorted_range_assign(first, last, count, std::integral_constant<bool, shorter>());*/

			if(count < size())
			{
				_p_sorted_range_assign_shorter(first, last, count);
			}
			else
			{
				_p_sorted_range_assign_longer(first, last);
			}
		}

		template<typename Input_iterator>
		void _p_sorted_range_assign_longer(Input_iterator first, Input_iterator last)
		{
			_p_sorted_range_assign_equal(first, last);

			while(first != last)
			{
				base::append(*first);
				++first;
			}
		}

		template<typename Input_iterator>
		void _p_sorted_range_assign_shorter(Input_iterator first, Input_iterator last, size_type new_len)
		{
			iterator this_first = begin();
			std::advance(this_first, new_len);

			base::remove_tail(this_first.nodeptr);

			_p_sorted_range_assign_equal(first, last);
		}

		template<typename Input_iterator>
		void _p_sorted_range_assign_equal(Input_iterator& first, Input_iterator& last)
		{
			iterator this_first = this->begin();
			iterator this_last = this->end();

			while(first != last &&
			this_first != this_last)
			{
				base::assign_item(this_first.nodeptr, *first);

				++first;
				++this_first;
			}
		}

		// allocators are equal or curr alloc can free stealed nodes.
		void _p_move_assign_sorted(map&& rval, std::true_type)
		{
			clear();
			base::steal_nodes(std::move(rval));
			utils::move_alloc_on_container_assignment(base::m_node_allocator, rval.m_node_allocator);
		}

		void _p_move_assign_sorted(map&& rval, std::false_type)
		{
			if(base::m_node_allocator == rval.m_node_allocator)
			{
				_p_move_assign_sorted(std::move(rval), std::true_type());
			}
			else
			{
				auto mbegin = utils::make_move_iterator_if_noexcept(rval.begin());
				auto mend = utils::make_move_iterator_if_noexcept(rval.end());

				_p_sorted_range_assign(mbegin, mend, rval.size());
			}

		}
	};


}

#endif // _SKIPLIST_H







