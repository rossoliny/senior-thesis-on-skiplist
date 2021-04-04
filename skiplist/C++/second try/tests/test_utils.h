//
// Created by Isa on 3/30/21.
//

#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <ctime>
#include <cstdlib>
#include <climits>

static int seed_rand()
{
//	srand(1);
	return 1;
}

static volatile int dummy = seed_rand();

//#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2/catch.hpp"
#include <string>
#include <initializer_list>

#include <map>
#include <smap>
#include <set>


// INPUTS

static int rand_int(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

static char rand_char()
{
	return rand_int(65, 90);
}
static std::string rand_string()
{
	int len = rand() % 50 + 1;
	std::string s;
	while(len--)
	{
		s += rand_char();
	}
	return s;
}




#define rand_pair() std::make_pair<int const, std::string>(rand_int(0, 1000), rand_string())
#define rand_pairs {rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair()}
#define rand_pairs_L {rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), \
					 rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair(), rand_pair()}

const static std::initializer_list<std::pair<int const, std::string>> init_list_1 = rand_pairs;
const static std::initializer_list<std::pair<int const, std::string>> init_list_2 = rand_pairs;
const static std::initializer_list<std::pair<int const, std::string>> init_list_L = rand_pairs;

const static std::vector<std::pair<std::string const, int>> vec_pairs = {
	 std::make_pair("smap", 1),     std::make_pair("unit", 2), std::make_pair("test", 3),  std::make_pair("auca", 4),
	 std::make_pair("std::map", 5), std::make_pair("ab", 6),   std::make_pair("cd", 7),    std::make_pair("ef", 8),
	 std::make_pair("gh", 9),       std::make_pair("ij", 10),  std::make_pair("kl", 11),   std::make_pair("mnop", 12),
	 std::make_pair("qrstuv", 13),  std::make_pair("wxy", 14), std::make_pair("z", 15)
};

const std::initializer_list<std::pair<int const, std::string>> initializer_list_pairs = {
	std::make_pair(1, "smap"), std::make_pair(2, "unit"), std::make_pair(3, "test"),  std::make_pair(4, "auca"),
	std::make_pair(5, "std::map"), std::make_pair(6, "ab"),   std::make_pair(7, "cd"),    std::make_pair(8, "ef"),
	std::make_pair(9, "gh"),       std::make_pair(10, "ij"),  std::make_pair(11, "kl"),   std::make_pair(12, "mnop"),
	std::make_pair(13, "qrstuv"),  std::make_pair(14, "wxy"), std::make_pair(15, "z")
};

const std::initializer_list<std::pair<int const, std::string>> initializer_list_pairs_L = {
	std::make_pair(1, "smap"), std::make_pair(2, "unit"),   std::make_pair(3, "test"),  std::make_pair(4, "auca"),
	std::make_pair(5, "std::map"), std::make_pair(6, "ab"),     std::make_pair(7, "cd"),    std::make_pair(8, "ef"),
	std::make_pair(9, "gh"),       std::make_pair(10, "ij"),    std::make_pair(11, "kl"),   std::make_pair(12, "mnop"),
	std::make_pair(13, "qrstuv"),  std::make_pair(14, "wxy"),   std::make_pair(15, "z"),    std::make_pair(16, "16"),
	std::make_pair(17, "qwer"),    std::make_pair(18, "tyui"),  std::make_pair(19, "op[]"), std::make_pair(20, "20"),
	std::make_pair(21, "asdf"),    std::make_pair(22, "ghjk"),  std::make_pair(23, "zxcv"), std::make_pair(24, "24"),
};

//const static std::vector<std::string> vec_pair_2 = {"list2", "unit2", "test2", "learn2", "isa std::list2", "ab2", "cd2", "ef2", "gh2", "ij2", "kl2", "mnop2", "qrstuv2", "wxy2", "z2"};

static std::set<std::pair<int const, std::string>> rand_pairs_of_len(int len)
{
	std::set<std::pair<int const, std::string>> res;
	while(res.size() != len)
	{
		res.insert(rand_pair());
	}
	return res;
}

// UTILS
#define MAPS_REQUIRE_EQUAL(my_map, std_map) (verify((my_map), (std_map)))

template<typename Key, typename Tp, typename Cmp, typename Alloc>
void verify(isa::map<Key, Tp, Cmp, Alloc> const& my_map, std::map<Key, Tp, Cmp, Alloc> const& std_map)
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

	auto ract = my_map.rbegin();
	auto rexp = std_map.rbegin();

	while(ract != my_map.rend())
	{
		REQUIRE(ract->first == rexp->first);
		REQUIRE(ract->second == rexp->second);

		++ract;
		++rexp;
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

	auto ract = my_map.rbegin();
	auto rexp = my_map_2.rbegin();

	while(ract != my_map.rend())
	{
		REQUIRE(ract->first == rexp->first);
		REQUIRE(ract->second == rexp->second);

		++ract;
		++rexp;
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

#define DO_RANDOM_OPERATIONS(my_map, std_map, num) (do_random_insertions_and_deletions((my_map), (std_map), (num)))

template<typename Key, typename Tp, typename Cmp, typename Alloc>
static void do_random_insertions_and_deletions(isa::map<Key, Tp, Cmp, Alloc> & my_map, std::map<Key, Tp, Cmp, Alloc> & std_map, int num)
{
	int N = rand_int(10, 20);
//	N = num;
	std::vector<std::pair<Key const, Tp>> history;
	history.reserve(N);

	while(N--)
	{
		bool insert = rand() < INT_MAX / 2;
		if(insert || history.size() == 0)
		{
			auto p = rand_pair();
			int k = p.first;

			history.push_back(p);

//			std::cout << "insert " << k << std::endl;
			my_map.insert(p);
			std_map.insert(p);
		}
		else
		{
			auto pos = history.begin();
			std::advance(pos, rand_int(0, history.size()-1));
			int k = pos->first;

//			std::cout << "erase " << k << std::endl;
			my_map.erase(pos->first);
			std_map.erase(pos->first);
		}
	}
}

#define CREATE_MAPS_INT_STRING(name1, name2) \
	std::initializer_list<std::pair<int, string>> ___init_list_input___##name1##name2 = rand_pairs; \
	my_map<int, string> name1(___init_list_input___##name1##name2.begin(), ___init_list_input___##name1##name2.end()); \
	std_map<int, string> name2(___init_list_input___##name1##name2.begin(), ___init_list_input___##name1##name2.end()); \
	MAPS_REQUIRE_EQUAL(name1, name2)




#define my_map isa::map
#define std_map std::map

using namespace std;

#endif //_TEST_UTILS_H