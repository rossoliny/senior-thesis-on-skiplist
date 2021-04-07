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

#ifdef TEST_CMP_GREATER
		auto expected_iterator1 = act.begin();
		auto expected_iterator2 = exp.begin();
#else
		auto expected_iterator1 = act.end();
		auto expected_iterator2 = exp.end();
#endif

		REQUIRE(p1.first == expected_iterator1);
		REQUIRE(p2.first == expected_iterator2);

		REQUIRE(p1.second == expected_iterator1);
		REQUIRE(p2.second == expected_iterator2);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("key is in map's range, but does not exist: return 0 elements")
	{
		auto p1 = act.equal_range("aa");
		auto p2 = exp.equal_range("aa");
#ifdef TEST_CMP_GREATER
		auto t = exp.end();
		REQUIRE(p2.first == exp.cend());
		REQUIRE(p2.first == exp.cend());
#else
		REQUIRE(p2.first == exp.begin());
		REQUIRE(p2.first == exp.begin());
		REQUIRE(*p1.first == *p2.first);
#endif
		REQUIRE(p2.first == p2.second);
		REQUIRE(p2.first == p2.second);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("key exists")
	{
		auto p1 = act.equal_range("ab");
		auto p2 = exp.equal_range("ab");

		REQUIRE(distance(p1.first, p1.second) == 1);
		REQUIRE(distance(p2.first, p2.second) == 1);

		REQUIRE(*p1.first == *p2.first);
#ifdef TEST_CMP_GREATER
		REQUIRE(p1.second == act.end());
		REQUIRE(p2.second == exp.end());
#else
		REQUIRE(*p1.second == *p2.second);
#endif

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