//
// Created by Isa on 4/2/21.
//

#include "test_utils.h"

#define tag "[equal_range]"

TEST_CASE("equal range search", tag)
{
	my_map<string, int> const act(vec_pairs.begin(), vec_pairs.end());
	std_map<string, int> const exp(vec_pairs.begin(), vec_pairs.end());

	SECTION("key is out of map's range")
	{
		auto p1 = act.equal_range("zz");
		auto p2 = exp.equal_range("zz");

		REQUIRE(p1.first == act.end());
		REQUIRE(p2.first == exp.end());

		REQUIRE(p1.second == act.end());
		REQUIRE(p2.second == exp.end());

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("key is in map's range, but does not exist: return 0 elements")
	{
		auto p1 = act.equal_range("aa");
		auto p2 = exp.equal_range("aa");

		REQUIRE(p2.first != exp.cend());
		REQUIRE(p2.first == p2.second);
		REQUIRE(p2.first != exp.cend());
		REQUIRE(p2.first == p2.second);
		REQUIRE(*p1.first == *p2.first);
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("key exists")
	{
		auto p1 = act.equal_range("ab");
		auto p2 = exp.equal_range("ab");

		REQUIRE(distance(p1.first, p1.second) == 1);
		REQUIRE(distance(p2.first, p2.second) == 1);

		REQUIRE(*p1.first == *p2.first);
		REQUIRE(*p1.second == *p2.second);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("key exists and is max")
	{
		auto p1 = act.equal_range((--act.end())->first);
		auto p2 = exp.equal_range((--exp.end())->first);

		REQUIRE(distance(p1.first, p1.second) == 1);
		REQUIRE(distance(p2.first, p2.second) == 1);

		REQUIRE(*p1.first == *p2.first);

		REQUIRE(p1.second == act.end());
		REQUIRE(p2.second == exp.end());

		MAPS_REQUIRE_EQUAL(act, exp);
	}

}