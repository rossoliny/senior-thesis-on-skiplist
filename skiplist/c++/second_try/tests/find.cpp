//
// Created by Isa on 4/2/21.
//


#include "test_utils.h"

#define tag "[find]"


TEST_CASE("find", tag)
{
	SECTION("find success")
	{
		my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
		std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

		auto key = vec_pairs.begin()->first;

		auto it1 = act.find(key);
		auto it2 = exp.find(key);

		REQUIRE(*it1 == *it2);
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("find const")
	{
		my_map<string, int> const act(vec_pairs.begin(), vec_pairs.end());
		std_map<string, int> const exp(vec_pairs.begin(), vec_pairs.end());

		auto key = vec_pairs.begin()->first;

		auto it1 = act.find(key);
		auto it2 = exp.find(key);

		REQUIRE(*it1 == *it2);
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("not found in empty")
	{
		my_map<string, int> act;
		std_map<string, int> exp;

		auto it1 = act.find("skiplist");
		auto it2 = exp.find("skiplist");

		REQUIRE(it1 == act.end());
		REQUIRE(it2 == exp.end());
		REQUIRE(it1 == act.begin());
		REQUIRE(it2 == exp.begin());

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("not found")
	{
		my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
		std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

		auto it1 = act.find("skiplist");
		auto it2 = exp.find("skiplist");

		REQUIRE(it1 == act.end());
		REQUIRE(it2 == exp.end());

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}