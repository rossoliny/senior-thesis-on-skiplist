//
// Created by Isa on 3/30/21.
//

#include "test_utils.h"


TEST_CASE("clear_nodes", "[clear_nodes]")
{
	SECTION("int")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		act.clear();
		exp.clear();

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("string")
	{
		my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
		std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

		act.clear();
		exp.clear();

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}

/*
TEST_CASE("destructor", "[destructor] [dtor]")
{
	SECTION("int")
	{
		CREATE_MAPS_INT_STRING(res);

		my_res.~map();
		std_res.~map();

		MAPS_REQUIRE_EQUAL(my_res, std_res);
	}
	SECTION("string")
	{
		my_map<string, int> act(vec_pairs.begin(), vec_pairs.end());
		std_map<string, int> exp(vec_pairs.begin(), vec_pairs.end());

		act.~map();
		exp.~map();

		MAPS_REQUIRE_EQUAL(act, exp);
	}
}
*/
