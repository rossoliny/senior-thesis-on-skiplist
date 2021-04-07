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
			: nodeptr()
		{
		}

		explicit skiplist_iterator(node_base* node) noexcept
			: nodeptr(node)
		{
		}

		self m_const_cast() const noexcept
		{
			return *this;
		}

		// dereference begin() of empty list is UB
		reference operator*() const noexcept
		{
			return *static_cast<node*>(nodeptr)->dataptr();
		}

		pointer operator->() const noexcept
		{
			return static_cast<node*>(nodeptr)->dataptr();
		}

		self& operator++() noexcept
		{
			nodeptr = nodeptr->m_next[0];
			return *this;
		}

		self operator++(int) noexcept
		{
			self old = *this;
			nodeptr = nodeptr->m_next[0];
			return old;
		}

		self& operator--() noexcept
		{
			nodeptr = static_cast<node*> (nodeptr)->get_prev();
			return *this;
		}

		self operator--(int) noexcept
		{
			self old = *this;
			nodeptr = static_cast<node*> (nodeptr)->get_prev();
			return old;
		}

		friend bool operator==(const self& a, const self& b) noexcept
		{
			return a.nodeptr == b.nodeptr;
		}

		friend bool operator!=(const self& a, const self& b) noexcept
		{
			return a.nodeptr != b.nodeptr;
		}
	public:

		node_base* nodeptr;
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
			: nodeptr()
		{
		}

		explicit skiplist_const_iterator(const node_base* node) noexcept
			: nodeptr(node)
		{
		}

		skiplist_const_iterator(iterator const& other) noexcept
			: nodeptr(other.nodeptr)
		{
		}

		iterator m_const_cast() const noexcept
		{
			return iterator(const_cast<node_base*> (nodeptr));
		}

		reference operator*() const noexcept
		{
			return *static_cast<node*> (nodeptr)->dataptr();
		}

		pointer operator->() const noexcept
		{
			return static_cast<node*> (nodeptr)->dataptr();
		}

		self& operator++() noexcept
		{
			nodeptr = nodeptr->m_next[0];
			return *this;
		}

		self operator++(int) noexcept
		{
			self old = *this;
			nodeptr = nodeptr->m_next[0];
			return old;
		}

		self& operator--() noexcept
		{
			nodeptr = static_cast<const node*> (nodeptr)->get_prev();
			return *this;
		}

		self& operator--(int) noexcept
		{
			self old = *this;
			nodeptr = static_cast<const node*> (nodeptr)->get_prev();
			return old;
		}

		friend bool operator==(const self& a, const self& b) noexcept
		{
			return a.nodeptr == b.nodeptr;
		}

		friend bool operator!=(const self& a, const self& b) noexcept
		{
			return a.nodeptr != b.nodeptr;
		}

	public:
		const node_base* nodeptr;
	};
}

#endif //_SKIPLIST_ITERATOR_H
