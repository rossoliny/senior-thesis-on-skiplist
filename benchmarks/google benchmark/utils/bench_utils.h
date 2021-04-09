//
// Created by Isa on 4/9/21.
//

#ifndef _BENCH_UTILS_H
#define _BENCH_UTILS_H

#include <benchmark/benchmark.h>

#define TMP_NAMESPACE_FIX

#include <map>
#include <smap>

namespace target
{
	template<typename K>
	using Bench_Comparator = std::less<K>;

	template<typename T>
	using Bench_Allocator = std::allocator<T>;

#if defined(SKIPLIST_BENCH)
	#ifndef TMP_NAMESPACE_FIX
	#include <smap> // why creates namespace errors
	#endif
	template<typename K, typename V, typename Cmp = Bench_Comparator<K>, typename Alloc = Bench_Allocator<std::pair<K const, V>>>
	using map = isa::smap<K, V, Cmp, Alloc>;

#elif defined(STD_MAP_BENCH)
	template<typename K, typename V, typename Cmp = Bench_Comparator<K>, typename Alloc = Bench_Allocator<std::pair<K const, V>>>
	using map = std::map<K, V, Cmp, Alloc>;

#endif


}

template<typename K, typename V, typename K_Cmp>
struct pair_cmp
{
	K_Cmp key_comp;

	bool operator()(std::pair<K const, V> const& a, std::pair<K const, V> const& b)
	{
		return key_comp(a.first, b.first);
	}
};

template<typename I>
I advance(I iter, I max, int n)
{
	while (n-- && iter != max)
	{
		++iter;
	}
	return iter;
}


using ss_pair_cmp = pair_cmp<std::string, std::string, target::Bench_Comparator<std::string>>;


int rand_int(int min, int max);

char rand_char();

std::string rand_string(int min_len, int max_len);

std::vector<std::pair<std::string, std::string>> generate_unsorted_range(int N);

std::vector<std::pair<std::string, std::string>> generate_sorted_range(int N);

enum {
	N1 = 1000,
	N2 = 10000,
	N3 = 100000,
	N4 = 1000000
};

#define Send_Args() DenseRange(0, N1, 64)

#endif //_BENCH_UTILS_H
