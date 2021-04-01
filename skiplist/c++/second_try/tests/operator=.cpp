//
// Created by Isa on 3/31/21.
//

#include "test_utils.h"
#include <initializer_list>
#include <string>
#include <iostream>

#define tag "[operator=] [assign operator] [operator] [operators]"


void print_map(my_map<int, string> const& m)
{

}

TEST_CASE("copy assignment operator")
{
	SECTION("equal size")
	{
		int len = 1 + (rand() % 5000);

		// inputs
		auto input = rand_pairs_of_len(len);
		my_map<int, string> act_inp(input.begin(), input.end());
		std_map<int, string> exp_inp(input.begin(), input.end());
		MAPS_REQUIRE_EQUAL(act_inp, exp_inp);

		// initial state of maps
		auto init_state = rand_pairs_of_len(len);
		my_map<int, string> act(init_state.begin(), init_state.end());
		std_map<int, string> exp(init_state.begin(), init_state.end());

		// assignment
		act = act_inp;
		exp = exp_inp;

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("shorter <-- longer")
	{
		initializer_list<pair<int const, string>> input = rand_pairs_L;
		my_map<int, string> act_inp = input;
		std_map<int, string> exp_inp = input;
		MAPS_REQUIRE_EQUAL(act_inp, exp_inp);

		my_map<int, string> act = rand_pairs;
		std_map<int, string> exp = rand_pairs;

		CHECK_MAPS_NEQ(act, exp);

		// shorter = longer
		act = act_inp;
		exp = exp_inp;

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("longer <-- shorter")
	{
		initializer_list<pair<int const, string>> input = rand_pairs;
		my_map<int, string> act_inp = input;
		std_map<int, string> exp_inp = input;

		MAPS_REQUIRE_EQUAL(act_inp, exp_inp);

		my_map<int, string> act = rand_pairs_L;
		std_map<int, string> exp = rand_pairs_L;

		CHECK_MAPS_NEQ(act, exp);

		act = act_inp;
		exp = exp_inp;

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}

TEST_CASE("move assignment operator")
{
	SECTION("equal size")
	{
		int len = 1 + (rand() % 5000);

		// inputs
		auto input = rand_pairs_of_len(len);
		my_map<int, string> act_inp(input.begin(), input.end());
		std_map<int, string> exp_inp(input.begin(), input.end());
		MAPS_REQUIRE_EQUAL(act_inp, exp_inp);

		// initial state of maps
		auto init_state = rand_pairs_of_len(len);
		my_map<int, string> act(init_state.begin(), init_state.end());
		std_map<int, string> exp(init_state.begin(), init_state.end());

		// assignment
		act = std::move(act_inp);
		exp = std::move(exp_inp);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("short <-- long")
	{
		initializer_list<pair<int const, string>> input = rand_pairs_L;
		my_map<int, string> act_inp = input;
		std_map<int, string> exp_inp = input;
		MAPS_REQUIRE_EQUAL(act_inp, exp_inp);

		my_map<int, string> act = rand_pairs;
		std_map<int, string> exp = rand_pairs;

		CHECK_MAPS_NEQ(act, exp);

		act = std::move(act_inp);
		exp = std::move(exp_inp);

		MAPS_REQUIRE_EQUAL(act, exp);
		MAPS_REQUIRE_EQUAL(act_inp, exp_inp);
	}
	SECTION("long <-- short")
	{
		initializer_list<pair<int const, string>> input = rand_pairs;
		my_map<int, string> act_inp = input;
		std_map<int, string> exp_inp = input;

		my_map<int, string> act = rand_pairs_L;
		std_map<int, string> exp = rand_pairs_L;

		act = std::move(act_inp);
		exp = std::move(exp_inp);

		MAPS_REQUIRE_EQUAL(act, exp);
		MAPS_REQUIRE_EQUAL(act_inp, exp_inp);
	}
}


TEST_CASE("initializer_list assignment operator")
{
	SECTION("equal size")
	{
		auto init1 = rand_pairs_of_len(initializer_list_pairs.size());
		auto init2 = rand_pairs_of_len(init1.size());

		my_map<int, string> act(init1.begin(), init1.end());
		std_map<int, string> exp(init2.begin(), init2.end());
		CHECK_MAPS_NEQ(act, exp);

		act = initializer_list_pairs;
		exp = initializer_list_pairs;

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("short <-- long")
	{
		my_map<int, string> act = initializer_list_pairs;
		std_map<int, string> exp = initializer_list_pairs;

		act = initializer_list_pairs_L;
		exp = initializer_list_pairs_L;

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("long <-- short")
	{
		my_map<int, string> act = initializer_list_pairs_L;
		std_map<int, string> exp = initializer_list_pairs_L;

		act = initializer_list_pairs;
		exp = initializer_list_pairs;

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}

