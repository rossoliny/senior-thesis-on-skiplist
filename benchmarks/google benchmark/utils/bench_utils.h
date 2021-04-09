//
// Created by Isa on 4/9/21.
//

#ifndef _BENCH_UTILS_H
#define _BENCH_UTILS_H

#include <benchmark/benchmark.h>
//#include <smap>
#include <map>

namespace auca_skiplist_bench
{
	template<typename K>
	using Bench_Comparator = std::less<K>;

	template<typename T>
	using Bench_Allocator = std::allocator<T>;

#if defined(SKIPLIST_BENCH)
	#include <smap>
	template<typename K, typename V, typename Cmp = Bench_Comparator<K>, typename Alloc = Bench_Allocator<std::pair<K, V>>>
	using map = isa::map<K, V, Cmp, Alloc>;

#elif defined(STD_MAP_BENCH)

	template<typename K, typename V, typename Cmp = Bench_Comparator<K>, typename Alloc = Bench_Allocator<std::pair<K, V>>>
	using map = std::map<K, V, Cmp, Alloc>;

#endif


}


using namespace std;

#endif //_BENCH_UTILS_H
