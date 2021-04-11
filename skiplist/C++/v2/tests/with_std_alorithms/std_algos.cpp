//
// Created by Isa on 4/7/21.
//


#include "test_utils.h"

#define tag "[std algorithms]"

TEST_CASE("std::find_node", tag)
{
	my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

	auto it1 = std::find(act.begin(), act.end(), vec_pairs[3]);
	auto it2 = std::find(exp.begin(), exp.end(), vec_pairs[3]);

	REQUIRE(*it1 == *it2);
}

bool predicate(pair<string, int> const p)
{
	return p.first == "smap";
}

TEST_CASE("std::find_if", tag)
{
	my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

	auto it1 = std::find_if(act.begin(), act.end(), predicate);
	auto it2 = std::find_if(exp.begin(), exp.end(), predicate);

	REQUIRE(*it1 == *it2);
}

void func(pair<string const, int>&  p)
{
	p.second = 228;
}
TEST_CASE("std::for_each", tag)
{
	my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

	std::for_each(act.begin(), act.end(), func);
	std::for_each(exp.begin(), exp.end(), func);

	MAPS_REQUIRE_EQUAL(act, exp);
}

template<typename Iter>
Iter advance(Iter i, int n)
{
	std::advance(i, n);
	return i;
}
TEST_CASE("std::copy", tag)
{
	my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

	pair<string, int> buff1[10];
	pair<string, int> buff2[10];

	std::copy(act.begin(), ::advance(act.begin(), 10), begin(buff1));
	std::copy(exp.begin(), ::advance(exp.begin(), 10), begin(buff2));

	for(int i = 0; i < 10; ++i)
	{
		REQUIRE(buff1[i] == buff2[i]);
	}
	MAPS_REQUIRE_EQUAL(act, exp);
}

TEST_CASE("std::copy with move iter", tag)
{
	my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

	pair<string, int> buff1[10];
	pair<string, int> buff2[10];

	std::copy(make_move_iterator(act.begin()), make_move_iterator(::advance(act.begin(), 10)), begin(buff1));
	std::copy(make_move_iterator(exp.begin()), make_move_iterator(::advance(exp.begin(), 10)), begin(buff2));

	for(int i = 0; i < 10; ++i)
	{
		REQUIRE(buff1[i] == buff2[i]);
	}
	MAPS_REQUIRE_EQUAL(act, exp);
}


TEST_CASE("std::is_sorted", tag)
{
	my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

	auto b1 = std::is_sorted(act.begin(), act.end(), act.value_comp());
	auto b2 = std::is_sorted(exp.begin(), exp.end(), exp.value_comp());

	REQUIRE(b1);
	REQUIRE(b2);
}


TEST_CASE("std::merge", tag)
{
	my_map<string, int> act1(vec_pairs.begin(), vec_pairs.end());
	my_map<string, int> act2(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> exp1(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> exp2(vec_pairs.begin(), vec_pairs.end());

	pair<string, int> buff1[act1.size() + exp1.size()];
	pair<string, int> buff2[act1.size() + exp1.size()];

	std::merge(act1.begin(), act1.end(), act2.begin(), act2.end(), buff1);
	std::merge(exp1.begin(), exp1.end(), exp2.begin(), exp2.end(), buff2);

	for(int i = 0; i < sizeof(buff1)/sizeof(buff1[0]); ++i)
	{
		REQUIRE(buff1[i] == buff2[i]);
	}
}