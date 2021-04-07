//
// Created by Isa on 3/31/21.
//

#include "test_utils.h"
#include <initializer_list>
#include <string>
#include <iostream>

#define tag "[operator=] [assign operator] [operator] [operators]"

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

		SECTION("some random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
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
		SECTION("some random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
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
		SECTION("some random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
			MAPS_REQUIRE_EQUAL(act, exp);
		}
	}
	SECTION("some random assignments")
	{
		CREATE_MAPS_INT_STRING(act, exp);
		DO_RANDOM_OPERATIONS(act, exp);

		CREATE_MAPS_INT_STRING(act2, exp2);
		DO_RANDOM_OPERATIONS(act2, exp2);

		act = act2;
		MAPS_REQUIRE_EQUAL(act, act2);
		MAPS_REQUIRE_EQUAL(act2, exp2);
		MAPS_REQUIRE_EQUAL(act, exp2);

		DO_RANDOM_OPERATIONS(act2, exp2);

		CREATE_MAPS_INT_STRING(act3, exp3);

		act = act3;
		MAPS_REQUIRE_EQUAL(act, act3);
		MAPS_REQUIRE_EQUAL(act, exp3);
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
	SECTION("some random move assignments")
	{
		CREATE_MAPS_INT_STRING(act, exp);
		DO_RANDOM_OPERATIONS(act, exp);

		CREATE_MAPS_INT_STRING(act2, exp2);
		DO_RANDOM_OPERATIONS(act2, exp2);

		act = std::move(act2);
		exp = std::move(exp2);

//		MAPS_REQUIRE_EQUAL(act, act2);
		MAPS_REQUIRE_EQUAL(act, exp);
		MAPS_REQUIRE_EQUAL(act2, exp2);

		DO_RANDOM_OPERATIONS(act, exp);
		DO_RANDOM_OPERATIONS(act2, exp2);

		CREATE_MAPS_INT_STRING(act3, exp3);

		act2 = std::move(act3);
		exp2 = std::move(exp3);
		MAPS_REQUIRE_EQUAL(act2, act2);
		MAPS_REQUIRE_EQUAL(act3, exp3);
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

