//
// Created by Isa on 3/16/21.
//
#include "test_utils.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#define tag "[constructors] [constructor] [ctor]"

using namespace std;

TEST_CASE("default ctor", tag)
{
	std::allocator<std::string> alloc;
	my_map<int, std::string> m(alloc);

	my_map<int, std::string> actual;
	std_map<int, std::string> expected;

	MAPS_REQUIRE_EQUAL(actual, expected);
}


TEST_CASE("range ctor", tag)
{
	SECTION("no compare, no alloc")
	{
		my_map<string, int> act(vec_pair_1.begin(), vec_pair_1.end());
		std_map<string, int> exp(vec_pair_1.begin(), vec_pair_1.end());

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("with compare, no alloc")
	{
		std::greater<string> compare;
		my_map<string, int, greater<string>> act(vec_pair_1.begin(), vec_pair_1.end(), compare);
		std_map<string, int, greater<string>> exp(vec_pair_1.begin(), vec_pair_1.end(), compare);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
/*
    SECTION("no compare, with alloc")
	{
		my_map<string, int> act(vec_pair_1.begin(), vec_pair_1.end());
		std_map<string, int> exp(vec_pair_1.begin(), vec_pair_1.end());

		MAPS_REQUIRE_EQUAL(act, exp);
	}
 */
	SECTION("with compare, with alloc")
	{
		std::greater<string> compare;
		std::allocator<pair<string, int>> alloc;
		my_map<string, int, greater<string>> act(vec_pair_1.begin(), vec_pair_1.end(), compare, alloc);
		std_map<string, int, greater<string>> exp(vec_pair_1.begin(), vec_pair_1.end(), compare, alloc);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("with move iter")
	{
		auto v1 = vec_pair_1;
		auto v2 = vec_pair_1;

		auto v1begin = make_move_iterator(v1.begin());
		auto v1end = make_move_iterator(v1.end());
		auto v2begin = make_move_iterator(v2.begin());
		auto v2end = make_move_iterator(v2.end());

		my_map<string, int> act(v1begin, v1end);
		std_map<string, int> exp(v2begin, v2end);

		for(int i = 0; i < v1.size(); ++i)
		{
			REQUIRE(v1[i] == v2[i]);
		}
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("with reverse iter")
	{
		auto v1 = vec_pair_1;
		auto v2 = vec_pair_1;

		my_map<string, int> act(v1.rbegin(), v1.rend());
		std_map<string, int> exp(v2.rbegin(), v2.rend());

		for(int i = 0; i < v1.size(); ++i)
		{
			REQUIRE(v1[i] == v2[i]);
		}
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("with reverse move iter")
	{
		auto v1 = vec_pair_1;
		auto v2 = vec_pair_1;

		auto v1begin = make_move_iterator(v1.rbegin());
		auto v1end = make_move_iterator(v1.rend());
		auto v2begin = make_move_iterator(v2.rbegin());
		auto v2end = make_move_iterator(v2.rend());

		my_map<string, int> act(v1begin, v1end);
		std_map<string, int> exp(v2begin, v2end);

		for(int i = 0; i < v1.size(); ++i)
		{
			REQUIRE(v1[i] == v2[i]);
		}
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("with const move iter")
	{
		auto v1 = vec_pair_1;
		auto v2 = vec_pair_1;

		auto v1begin = make_move_iterator(v1.cbegin());
		auto v1end = make_move_iterator(v1.cend());
		auto v2begin = make_move_iterator(v2.cbegin());
		auto v2end = make_move_iterator(v2.cend());

		my_map<string, int> act(v1begin, v1end);
		std_map<string, int> exp(v2begin, v2end);

		for(int i = 0; i < v1.size(); ++i)
		{
			REQUIRE(v1[i] == v2[i]);
		}
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("with const reverse move iter")
	{
		auto v1 = vec_pair_1;
		auto v2 = vec_pair_1;

		auto v1begin = make_move_iterator(v1.crbegin());
		auto v1end = make_move_iterator(v1.crend());
		auto v2begin = make_move_iterator(v2.crbegin());
		auto v2end = make_move_iterator(v2.crend());

		my_map<string, int> act(v1begin, v1end);
		std_map<string, int> exp(v2begin, v2end);

		for(int i = 0; i < v1.size(); ++i)
		{
			REQUIRE(v1[i] == v2[i]);
		}
		MAPS_REQUIRE_EQUAL(act, exp);
	}
}


TEST_CASE("copy ctor", tag)
{
	SECTION("with default allocator")
	{
		my_map<string, int> act_inp(vec_pair_1.begin(), vec_pair_1.end());
		std_map<string, int> exp_inp(vec_pair_1.begin(), vec_pair_1.end());

		my_map<string, int> act(act_inp);
		std_map<string, int> exp(exp_inp);

		MAPS_REQUIRE_EQUAL(act_inp, act);
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("with given allocator")
	{
		my_map<string, int> act_inp(vec_pair_1.begin(), vec_pair_1.end());
		std_map<string, int> exp_inp(vec_pair_1.begin(), vec_pair_1.end());

		std::allocator<void*> alloc;
		my_map<string, int> act(act_inp, alloc);
		std_map<string, int> exp(exp_inp, alloc);

		MAPS_REQUIRE_EQUAL(act_inp, act);
		MAPS_REQUIRE_EQUAL(act, exp);
	}
}

TEST_CASE("move ctor", tag)
{
	SECTION("with default allocator argument")
	{
		my_map<string, int> act_inp(vec_pair_1.begin(), vec_pair_1.end());
		std_map<string, int> exp_inp(vec_pair_1.begin(), vec_pair_1.end());

		my_map<string, int> act(std::move(act_inp));
		std_map<string, int> exp(std::move(exp_inp));

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("with given allocator argument")
	{
		vector<pair<string, int>> inp = {
			make_pair("asdf", rand()),      make_pair("fdsf", rand()),      make_pair("fdefg", rand()),
			make_pair("fdj", rand()),       make_pair("fjhrii", rand()),    make_pair("fda", rand()),
			make_pair("fhiuro", rand()),    make_pair("fruibvn", rand()),   make_pair("fkjoruvn", rand()),
			make_pair("fjhiur", rand())
		};

		my_map<string, int> act_inp(inp.begin(), inp.end());
		std_map<string, int> exp_inp(inp.begin(), inp.end());

		std::allocator<void> alloc;
		my_map<string, int> act(std::move(act_inp), alloc);
		std_map<string, int> exp(std::move(exp_inp), alloc);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}

TEST_CASE("initializer_list ctor", tag)
{
	my_map<std::string, int> actual = {  make_pair("skiplist", 1),  make_pair("unit", 2),   make_pair("test", 3),
	                                     make_pair("std::map", 3),  make_pair("auca", 4), make_pair("std::map", 5)};

	std_map<std::string, int> expected = {  make_pair("skiplist", 1),   make_pair("unit", 2),   make_pair("test", 3),
											make_pair("std::map", 3),   make_pair("auca", 4),   make_pair("std::map", 5)};

	MAPS_REQUIRE_EQUAL(actual, expected);
}
/*

TEST_CASE("clear", "[clear]")
{
	SECTION("int")
	{
		CREATE_LISTS_OF_INT(res);

		gcc_res.clear();
		std_res.clear();

		MAPS_REQUIRE_EQUAL(gcc_res, std_res);
	}
	SECTION("string")
	{
		my_map<string, int> act(vec_pair_1.begin(), vec_pair_1.end());
		std_map<string, int> exp(vec_pair_1.begin(), vec_pair_1.end());

		act.clear();
		exp.clear();

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}

*/
