//
// Created by Isa on 3/26/21.
//

#ifndef _SKIPLIST_ITERATOR_H
#define _SKIPLIST_ITERATOR_H


#include "skiplist_node.h"
#include <iterator>

namespace isa
{
	template<typename Key, typename Tp>
	struct skiplist_iterator
	{
	private:
		using self = skiplist_iterator<Key, Tp>;
		using node = detail::skiplist_node<Key, Tp>;
		using node_base = detail::skiplist_node_base;
		using pair_type = std::pair<Key const, Tp>;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = pair_type;
		using pointer = pair_type*;
		using reference = pair_type&;

		skiplist_iterator() noexcept
			: curr()
		{
		}

		explicit skiplist_iterator(node_base* node) noexcept
			: curr(node)
		{
		}

		self m_const_cast() const noexcept
		{
			return *this;
		}

		// dereference begin() of empty list is UB
		reference operator*() const noexcept
		{
			return *static_cast<node*>(curr)->dataptr();
		}

		pointer operator->() const noexcept
		{
			return static_cast<node*>(curr)->dataptr();
		}

		self& operator++() noexcept
		{
			curr = curr->m_next[0];
			return *this;
		}

		self operator++(int) noexcept
		{
			self old = *this;
			curr = curr->m_next[0];
			return old;
		}

		self& operator--() noexcept
		{
			curr = static_cast<node*> (curr)->get_prev();
			return *this;
		}

		self operator--(int) noexcept
		{
			self old = *this;
			curr = static_cast<node*> (curr)->get_prev();
			return old;
		}

		friend bool operator==(const self& a, const self& b) noexcept
		{
			return a.curr == b.curr;
		}

		friend bool operator!=(const self& a, const self& b) noexcept
		{
			return a.curr != b.curr;
		}

		node_base* curr;
	};

	template<typename Key, typename Tp>
	struct skiplist_const_iterator
	{
	private:
		using self = skiplist_const_iterator<Key, Tp>;
		using node = const detail::skiplist_node<Key, Tp>;
		using node_base = const detail::skiplist_node_base;
		using iterator = skiplist_iterator<Key, Tp>;
		using pair_type = std::pair<Key const, Tp>;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = pair_type;
		using pointer = pair_type const*;
		using reference = pair_type const&;

		skiplist_const_iterator() noexcept
			: curr()
		{
		}

		explicit skiplist_const_iterator(const node_base* node) noexcept
			: curr(node)
		{
		}

		explicit skiplist_const_iterator(const iterator& other) noexcept
			: curr(other.curr)
		{
		}

		iterator m_const_cast() const noexcept
		{
			return iterator(const_cast<node_base*> (curr));
		}

		reference operator*() const noexcept
		{
			return *static_cast<node*> (curr)->dataptr();
		}

		pointer operator->() const noexcept
		{
			return *static_cast<node*> (curr)->dataptr();
		}

		self& operator++() noexcept
		{
			curr = curr->m_next[0];
			return *this;
		}

		self operator++(int) noexcept
		{
			self old = *this;
			curr = curr->m_next[0];
			return old;
		}

		self& operator--() noexcept
		{
			curr = static_cast<const node*> (curr)->get_prev();
			return *this;
		}

		self& operator--(int) noexcept
		{
			self old = *this;
			curr = static_cast<const node*> (curr)->get_prev();
			return old;
		}

		friend bool operator==(const self& a, const self& b) noexcept
		{
			return a.curr == b.curr;
		}

		friend bool operator!=(const self& a, const self& b) noexcept
		{
			return a.curr != b.curr;
		}

		const node_base* curr;
	};
}

#endif //_SKIPLIST_ITERATOR_H
