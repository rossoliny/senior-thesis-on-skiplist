//
// Created by Isa on 4/2/21.
//

#include "test_utils.h"

#define tag "[emplace]"

pair<string const, string> rand_pair_ss()
{
	return make_pair(rand_string(), rand_string());
}

TEST_CASE("emplace single element", tag)
{
	SECTION("from pair lvalue")
	{
		my_map<int, string> act;
		std_map<int, string> exp;

		pair<int const, string> const val = rand_pair();

		auto p1 = act.emplace(val);
		auto p2 = exp.emplace(val);

		REQUIRE(p1.first == act.begin());
		REQUIRE(p2.first == exp.begin());
		REQUIRE(p1.second == p2.second);

		REQUIRE(*act.begin() == val);
		REQUIRE(*exp.begin() == val);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("from pair rvalue")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		pair<int const, string> val1 = rand_pair();
		pair<int const, string> val2 = val1;

		auto p1 = act.emplace(std::move(val1));
		auto p2 = exp.emplace(std::move(val2));

		REQUIRE(*p1.first == *p2.first);
		REQUIRE(p1.second == p2.second);
		REQUIRE(val1 == val2);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("from pair ctor rvalue args")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		pair<int, string> val1 = rand_pair();
		pair<int, string> val2 = val1;

		auto p1 = act.emplace(std::move(val1.first), std::move(val1.second));
		auto p2 = exp.emplace(std::move(val2.first), std::move(val2.second));

		REQUIRE(*p1.first == *p2.first);
		REQUIRE(p1.second == p2.second);
		REQUIRE(val1 == val2);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("from pair's elements ctor args")
	{
		initializer_list<pair<string const, string>> il = {rand_pair_ss(), rand_pair_ss(), rand_pair_ss(), rand_pair_ss(), rand_pair_ss(), rand_pair_ss(), rand_pair_ss()};
		my_map<string, string> act = il;
		std_map<string, string> exp = il;

		pair<string, string> val1 = rand_pair_ss();
		pair<string, string> val2 = val1;

		auto p1 = act.emplace(std::move(val1.first), val1.second.c_str());
		auto p2 = exp.emplace(std::move(val2.first), val2.second.c_str());

		REQUIRE(*p1.first == *p2.first);
		REQUIRE(p1.second == p2.second);
		REQUIRE(val1 == val2);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}

TEST_CASE("emplace with hint", tag)
{

}