//
// Created by Isa on 4/2/21.
//

//
// Created by Isa on 4/2/21.
//

#include "test_utils.h"

#define tag "[at]"

TEST_CASE("at", tag)
{
	SECTION("find_node arg rvalue")
	{
		my_map<string, string> act;
		std_map<string, string> exp;

		string const k = "skiplist";
		string const v = rand_string();

		// create keys
		act[k];
		exp[k];

		auto r1 = act.at(k);
		auto r2 = exp.at(k);

		REQUIRE(r1 == r2);

		act.at(k) = v;
		exp.at(k) = v;

		REQUIRE(act[k] == (*((act.insert(make_pair(k, v))).first)).second);
		REQUIRE(exp[k] == (*((exp.insert(make_pair(k, v))).first)).second);
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("find_node arg lvalue")
	{
		my_map<string, string> act;
		std_map<string, string> exp;

		string key = "skiplist";
		act[key];
		exp[key];

		auto r1 = act.at(key);
		auto r2 = exp.at(key);

		REQUIRE(r1 == r2);

		act[key] = "value";
		exp[key] = "value";

		REQUIRE(key == "skiplist");
		REQUIRE(r1 == r2);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("not found empty")
	{
		my_map<string, string> act;
		std_map<string, string> exp;

		REQUIRE_THROWS_AS(act.at(rand_string()), std::out_of_range);
		REQUIRE_THROWS_AS(exp.at(rand_string()), std::out_of_range);
	}
	SECTION("not found")
	{
		my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
		std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

		REQUIRE_THROWS_AS(act.at("skiplist"), std::out_of_range);
		REQUIRE_THROWS_AS(exp.at("skiplist"), std::out_of_range);
	}
}