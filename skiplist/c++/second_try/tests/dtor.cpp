//
// Created by Isa on 3/30/21.
//

#include "test_utils.h"


TEST_CASE("clear", "[clear]")
{
	SECTION("int")
	{
		CREATE_MAPS_INT_STRING(res);

		my_res.clear();
		std_res.clear();

		MAPS_REQUIRE_EQUAL(my_res, std_res);
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
