//
// Created by Isa on 3/30/21.
//

#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H
#include <ctime>
#include <cstdlib>
static int seed_rand()
{
	srand(time(0));
	return 1;
}

static int dummy = seed_rand();

//#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2/catch.hpp"
#include <string>

#include <map>
#include "skiplist.h"


// INPUTS
char rand_char()
{
	return rand() % 127 + 32;
}
std::string rand_string()
{
	int len = rand() % 50 + 1;
	std::string s;
	while(len--)
	{
		s += rand_char();
	}
	return s;
}

#define rand_pair() std::make_pair<int, std::string>(rand(), rand_string())
#define rand_pairs {rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair()}
#define rand_pairs_L {rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), \
					 rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair()}

const static std::initializer_list<std::pair<int, std::string>> init_list_1 = rand_pairs;
const static std::initializer_list<std::pair<int, std::string>> init_list_2 = rand_pairs;
const static std::initializer_list<std::pair<int, std::string>> init_list_L = rand_pairs;

const static std::vector<std::pair<std::string, int>> vec_pair_1 = {
	 std::make_pair("skiplist", 1), std::make_pair("unit", 2), std::make_pair("test", 3),  std::make_pair("auca", 4),
	 std::make_pair("std::map", 5), std::make_pair("ab", 6),   std::make_pair("cd", 7),    std::make_pair("ef", 8),
	 std::make_pair("gh", 9),       std::make_pair("ij", 10),  std::make_pair("kl", 11),   std::make_pair("mnop", 12),
	 std::make_pair("qrstuv", 13),  std::make_pair("wxy", 14), std::make_pair("z", 15)
};


//const static std::vector<std::string> vec_pair_2 = {"list2", "unit2", "test2", "learn2", "gcc std::list2", "ab2", "cd2", "ef2", "gh2", "ij2", "kl2", "mnop2", "qrstuv2", "wxy2", "z2"};

// UTILS
#define MAPS_REQUIRE_EQUAL(my_map, std_map) (verify((my_map), (std_map)))

template<typename Key, typename Tp, typename Cmp, typename Alloc>
void verify(isa::map<Key, Tp, Cmp, Alloc>& my_map, std::map<Key, Tp, Cmp, Alloc>& std_map)
{
	REQUIRE(my_map.size() == std_map.size());
	REQUIRE(my_map.get_allocator() == std_map.get_allocator());
//	REQUIRE(my_map.max_size() == std_map.max_size());

	REQUIRE(my_map.empty() == std_map.empty());

	auto act = my_map.begin();
	auto exp = std_map.begin();

	while(act != my_map.end())
	{
		REQUIRE(act->first == exp->first);
		REQUIRE(act->second == exp->second);

		++act;
		++exp;
	}
}

template<typename Key, typename Tp, typename Cmp, typename Alloc>
void verify(isa::map<Key, Tp, Cmp, Alloc>& my_map, isa::map<Key, Tp, Cmp, Alloc>& my_map_2)
{
	REQUIRE(my_map.size() == my_map_2.size());
	REQUIRE(my_map.get_allocator() == my_map_2.get_allocator());
//	REQUIRE(my_map.max_size() == my_map_2.max_size());

	auto act = my_map.begin();
	auto exp = my_map_2.begin();

	while(act != my_map.end())
	{
		REQUIRE(act->first == exp->first);
		REQUIRE(act->second == exp->second);

		++act;
		++exp;
	}
}


#define CHECK_MAPS_NEQ(gcc_list, std_list) (check_neq((gcc_list), (std_list)))
template<typename Key, typename Tp, typename Cmp, typename Alloc>
bool check_neq(isa::map<Key, Tp, Cmp, Alloc>& my_map, std::map<Key, Tp, Cmp, Alloc>& std_map)
{
	if(my_map.size() != std_map.size())
		return true;

	auto act = my_map.begin();
	auto exp = std_map.begin();

	bool full_match = true;
	while(act != my_map.end())
	{
		if(act->first != exp->first || act->second != exp->second)
		{
			full_match = false;
			break;
		}

		++act;
		++exp;
	}

	CHECK(not full_match);
	return not full_match;
}


#define CREATE_MAPS_INT_STRING(name) \
	std::initializer_list<int> ___init_list_input___##name = rand_pairs; \
	my_map<int, string> my_##name = ___init_list_input___##name; \
	std_map<int, string> std_##name = ___init_list_input___##name

#define my_map isa::map
#define std_map std::map

using namespace std;

#endif //_TEST_UTILS_H
