//
// Created by Isa on 3/25/21.
//

#ifndef _MAP_ITERATOR_H
#define _MAP_ITERATOR_H

//
// Created by Isa on 3/16/21.
//

#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H

#include "old_skiplist_node.h"
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
		using pair = std::pair<Key, Tp>;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = pair;
		using pointer = pair*;
		using reference = pair&;

		skiplist_iterator() noexcept
			: m_node()
		{
		}

		explicit skiplist_iterator(node_base* node) noexcept
			: m_node(node)
		{
		}

		self m_const_cast() const noexcept
		{
			return *this;
		}

		// dereference begin() of empty list is UB
		reference operator*() const noexcept
		{
			return *static_cast<node*>(m_node)->m_dataptr();
		}

		pointer operator->() const noexcept
		{
			return static_cast<node*>(m_node)->m_dataptr();
		}

		self& operator++() noexcept
		{
			m_node = m_node->m_next[0];
			return *this;
		}

		self operator++(int) noexcept
		{
			self old = *this;
			m_node = m_node->m_next[0];
			return old;
		}

		self& operator--() noexcept
		{
			m_node = m_node->m_prev;
			return *this;
		}

		self operator--(int) noexcept
		{
			self old = *this;
			m_node = m_node->m_prev;
			return old;
		}

		friend bool operator==(const self& a, const self& b) noexcept
		{
			return a.m_node == b.m_node;
		}

		friend bool operator!=(const self& a, const self& b) noexcept
		{
			return a.m_node != b.m_node;
		}

		node_base* m_node;
	};

	template<typename Key, typename Tp>
	struct skiplist_const_iterator
	{
	private:
		using self = skiplist_const_iterator<Key, Tp>;
		using node = const detail::skiplist_node<Key, Tp>;
		using node_base = const detail::skiplist_node_base;
		using iterator = skiplist_iterator<Key, Tp>;
		using pair = std::pair<Key, Tp>;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = pair;
		using pointer = const pair*;
		using reference = const pair&;

		skiplist_const_iterator() noexcept
			: m_node()
		{
		}

		explicit skiplist_const_iterator(const node_base* node) noexcept
			: m_node(node)
		{
		}

		explicit skiplist_const_iterator(const iterator& other) noexcept
			: m_node(other.m_node)
		{
		}

		iterator m_const_cast() const noexcept
		{
			return iterator(const_cast<node_base*> (m_node));
		}

		reference operator*() const noexcept
		{
			return *static_cast<node*> (m_node)->m_valptr();
		}

		pointer operator->() const noexcept
		{
			return *static_cast<node*> (m_node)->m_valptr();
		}

		self& operator++() noexcept
		{
			m_node = m_node->m_next[0];
			return *this;
		}

		self operator++(int) noexcept
		{
			self old = *this;
			m_node = m_node->m_next[0];
			return old;
		}

		self& operator--() noexcept
		{
			m_node = m_node->m_prev;
			return *this;
		}

		self& operator--(int) noexcept
		{
			self old = *this;
			m_node = m_node->m_prev;
			return old;
		}

		friend bool operator==(const self& a, const self& b) noexcept
		{
			return a.m_node == b.m_node;
		}

		friend bool operator!=(const self& a, const self& b) noexcept
		{
			return a.m_node != b.m_node;
		}

		const node_base* m_node;
	};
}

#endif //LIST_ITERATOR_H



#endif //_MAP_ITERATOR_H
