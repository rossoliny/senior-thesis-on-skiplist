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
	_THOUSAND = 1000,
	_10_THOUSANDS = 10000,
	_100_THOUSANDS = 100000,
	_MILLION = 1000000,
	_10_MILLIONS = 10000000
};

#define Send_Args() DenseRange(1, _MILLION + 1, _10_THOUSANDS)
//#define Send_Args() RangeMultiplier(2)->Range(8, 8<<11)->Iterations(150)

#endif //_BENCH_UTILS_H
