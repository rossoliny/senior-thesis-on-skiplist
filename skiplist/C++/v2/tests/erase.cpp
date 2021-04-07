//
// Created by Isa on 4/1/21.
//


#include "test_utils.h"

#define tag "[erase]"


TEST_CASE("erase single by position", tag)
{
	SECTION("erase last element (not back)")
	{
		my_map<int, std::string> act;
		std_map<int, std::string> exp;

		pair<int const, string> val = rand_pair();

		auto p1 = act.insert(val);
		auto p2 = exp.insert(val);
		REQUIRE(p1.second == p2.second);

		auto it1 = act.erase(act.begin());
		auto it2 = exp.erase(exp.begin());

		REQUIRE(it1 == act.end());
		REQUIRE(it2 == exp.end());
		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("non last (not back)")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		act.insert(make_pair(1050, "1500"));
		exp.insert(make_pair(1050, "1500"));

		act.insert(make_pair(1500, "1500"));
		exp.insert(make_pair(1500, "1500"));

		act.insert(make_pair(1600, "1600"));
		exp.insert(make_pair(1600, "1600"));

		pair<int, string> val(1100, rand_string());

		auto p1 = act.insert(val);
		auto p2 = exp.insert(val);

		auto it1 = p1.first;
		auto it2 = p2.first;

		REQUIRE(*it1 == *it2);
		REQUIRE(p1.second == p2.second);

		it1 = act.erase(p1.first);
		it2 = exp.erase(p2.first);

#ifdef TEST_CMP_GREATER
		int next = 1050;
#else
		int next = 1500;
#endif

		REQUIRE(it2->first == next);
		REQUIRE(it1->first == next);
		REQUIRE(*it1 == *it2);
		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("non last, pop back")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		act.insert(make_pair(1500, "1500"));
		exp.insert(make_pair(1500, "1500"));

		pair<int, string> val(2000, rand_string());

		auto p1 = act.insert(val);
		auto p2 = exp.insert(val);

		auto it1 = p1.first;
		auto it2 = p2.first;

		REQUIRE(*it1 == *it2);
		REQUIRE(p1.second == p2.second);

		it1 = act.erase(p1.first);
		it2 = exp.erase(p2.first);

#ifdef TEST_CMP_GREATER
		auto next1 = act.insert(make_pair(1500, "1500")).first;
		auto next2 = exp.insert(make_pair(1500, "1500")).first;
#else
		auto next1 = act.end();
		auto next2 = exp.end();
#endif

		REQUIRE(it1 == next1);
		REQUIRE(it2 == next2);

		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("test next")
	{
		my_map<int, std::string> act;
		std_map<int, std::string> exp;

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

#ifdef TEST_CMP_GREATER
		int key = INT_MAX;
#else
		int key = -1;
#endif
		pair<int const, string> val1 = make_pair(key, "first");
		pair<int const, string> val2 = rand_pair();
		pair<int const, string> val3 = rand_pair();

		auto p1 = act.insert(val1); act.insert(val2); act.insert(val3);
		auto p2 = exp.insert(val1); exp.insert(val2); exp.insert(val3);
		REQUIRE(p1.second == p2.second);

		auto it11 = ++p1.first;
		auto it22 = ++p2.first;

		auto it1 = act.erase(act.begin());
		auto it2 = exp.erase(exp.begin());

		REQUIRE(it11 == it1);
		REQUIRE(it22 == it2);

		REQUIRE(*it11 == *it1);
		REQUIRE(*it22 == *it2);

		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
}

TEST_CASE("erase single by key", tag)
{
	SECTION("erase last element")
	{
		my_map<int, std::string> act;
		std_map<int, std::string> exp;

		pair<int const, string> val = rand_pair();
		int key = val.first;

		auto p1 = act.insert(val);
		auto p2 = exp.insert(val);
		REQUIRE(p1.second == p2.second);

		size_t res1 = act.erase(key);
		size_t res2 = exp.erase(key);

		REQUIRE(res1 == res2);
		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

	}
	SECTION("non last")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		pair<int const, string> val = rand_pair();
		int key = val.first;

		auto it1 = act.insert(val);
		auto it2 = exp.insert(val);

		size_t res1 = act.erase(key);
		size_t res2 = exp.erase(key);

		REQUIRE(res1 == res2);
		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("pop back")
	{
		CREATE_MAPS_INT_STRING(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
		my_map<int, string> act_cpy = act;

		pair<int const, string> val = make_pair(9999, "BACK");
		int key = val.first;

		auto it1 = act.insert(val);
		auto it2 = exp.insert(val);

		size_t res1 = act.erase(key);
		size_t res2 = exp.erase(key);

		REQUIRE(res1 == res2);
		MAPS_REQUIRE_EQUAL(act, act_cpy);
		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
}

TEST_CASE("erase range", tag)
{
	SECTION("erase from empty map")
	{
		my_map<int, std::string> act;
		std_map<int, std::string> exp;

		auto it1 = act.erase(act.begin(), act.end());
		auto it2 = exp.erase(exp.begin(), exp.end());

		REQUIRE(it1 == act.begin());
		REQUIRE(it2 == exp.begin());

		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("erase nothing")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		auto it1 = act.erase(act.begin(), act.begin());
		auto it2 = exp.erase(exp.begin(), exp.begin());

		REQUIRE(it1 == act.begin());
		REQUIRE(it2 == exp.begin());
		REQUIRE(*it1 == *it2);

		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("erase postfix")
	{
		CREATE_MAPS_INT_STRING(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		auto begin1 = act.begin();
		auto begin2 = exp.begin();

		int N = rand_int(0, act.size() / 2);
		advance(begin1, N);
		advance(begin2, N);

		auto it1 = act.erase(begin1, act.end());
		auto it2 = exp.erase(begin2, exp.end());

		REQUIRE(it1 == act.end());
		REQUIRE(it2 == exp.end());
		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("erase prefix")
	{
		CREATE_MAPS_INT_STRING(act, exp);
		SECTION("do random operations 1")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		auto end1 = act.end();
		auto end2 = exp.end();

		int N = rand_int(2, act.size() / 2);
		advance(end1, -N);
		advance(end2, -N);

		auto it1 = act.erase(act.begin(), end1);
		auto it2 = exp.erase(exp.begin(), end2);

		REQUIRE(*it1 == *it2);
		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations 2")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("erase middle")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		auto begin1 = act.begin();
		auto begin2 = exp.begin();

		int N = rand_int(1, act.size() / 2);
		advance(begin1, N);
		advance(begin2, N);

		auto end1 = act.end();
		auto end2 = exp.end();

		int N2 = rand_int(1, act.size() / 2);
		advance(end1, -N2);
		advance(end2, -N2);

		auto it1 = act.erase(begin1, end1);
		auto it2 = exp.erase(begin2, end2);

		REQUIRE(*it1 == *it2);
		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("erase everything")
	{
		CREATE_MAPS_INT_STRING(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		auto it1 = act.erase(act.begin(), act.end());
		auto it2 = exp.erase(exp.begin(), exp.end());

		REQUIRE(it1 == act.end());
		REQUIRE(it2 == exp.end());
		REQUIRE(it1 == act.begin());
		REQUIRE(it2 == exp.begin());
		MAPS_REQUIRE_EQUAL(act, exp);
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
}