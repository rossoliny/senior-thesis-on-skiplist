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

/*
TEST_CASE("initializer_list assignment operator")
{
	SECTION("short <- long")
	{
		my_map<int, string> act = init_list_1;
		std_map<int, string> exp = init_list_1;

		act = init_list_L;
		exp = init_list_L;

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("long <- short")
	{
		my_map<int, string> act = init_list_L;
		std_map<int, string> exp = init_list_L;

		act = init_list_1;
		exp = init_list_1;

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}
*/
