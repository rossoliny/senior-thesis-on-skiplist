//
// Created by Isa on 4/1/21.
//

#include "test_utils.h"

#define tag "[insert]"


TEST_CASE("insert single element", tag)
{
	SECTION("empty map")
	{
		my_map<int, string> act;
		std_map<int, string> exp;

		pair<int const, string> const val = rand_pair();

		auto p1 = act.insert(val);
		auto p2 = exp.insert(val);

		REQUIRE(p1.first == act.begin());
		REQUIRE(p2.first == exp.begin());
		REQUIRE(p1.second == p2.second);

		REQUIRE(*act.begin() == val);
		REQUIRE(*exp.begin() == val);

		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("non empty map")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		pair<int const, string> const val = rand_pair();

		auto p1 = act.insert(val);
		auto p2 = exp.insert(val);

		REQUIRE(*p1.first == *p2.first);
		REQUIRE(p1.second == p2.second);

		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("insert at begin")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		pair<int const, string> const val = make_pair(-1, "min");

		auto p1 = act.insert(val);
		auto p2 = exp.insert(val);

		REQUIRE(*p1.first == *p2.first);
		REQUIRE(p1.second == p2.second);

		REQUIRE(*act.begin() == val);
		REQUIRE(*exp.begin() == val);

		auto it1 = p1.first;
		auto it2 = p2.first;

		MAPS_REQUIRE_EQUAL(act, exp);

		// TODO: fix malloc error by removing append functionality
		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("insert at end")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		pair<int const, string> const val = make_pair(INT_MAX, "max");

		auto p1 = act.insert(val);
		auto p2 = exp.insert(val);

		REQUIRE(*p1.first == *p2.first);
		REQUIRE(p1.second == p2.second);

		REQUIRE(*--act.end() == val);
		REQUIRE(*--exp.end() == val);

		auto it1 = p1.first;
		auto it2 = p2.first;

		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("insert existing")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		auto it = act.cbegin();
		std::advance(it, rand_int(0, act.size() - 1));

		auto p1 = act.insert(*it);
		auto p2 = exp.insert(*it);

		REQUIRE(*p1.first == *p2.first);
		REQUIRE(p1.second == p2.second);

		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
}


TEST_CASE("template insert single element", tag)
{
	SECTION("empty map")
	{
		SECTION("lvalue template arg")
		{
			my_map<int, string> act;
			std_map<int, string> exp;

			pair<int const, string> val = rand_pair();

			auto p11 = act.insert<pair<int const, string>&>(val); // instantiate P as reference
			auto p22 = exp.insert<pair<int const, string>&>(val);

			REQUIRE(p11.first == act.begin());
			REQUIRE(p22.first == exp.begin());
			REQUIRE(p11.second == p22.second);
			REQUIRE(*act.begin() == val);
			REQUIRE(*exp.begin() == val);

			SECTION("do random operations")
			{
				DO_RANDOM_OPERATIONS(act, exp);
			}
		}
		SECTION("rvalue template arg")
		{
//			cout << "\nSTART DEBUG SECTION\n";
			my_map<int, string> act;
			std_map<int, string> exp;

			pair<int const, string> vall = rand_pair();
			pair<int const, string> val1 = vall;
			pair<int const, string> val2 = vall;

			auto p1 = act.insert<pair<int const, string>>(std::move(val1));
			auto p2 = exp.insert<pair<int const, string>>(std::move(val2));

			REQUIRE(val1 == val2);
			REQUIRE(*p1.first == vall);
			REQUIRE(*p2.first == vall);
			REQUIRE(p1.second == p2.second);

			MAPS_REQUIRE_EQUAL(act, exp);

			SECTION("do random operations")
			{
//				cout << "map before random operations\n";
//				print(act.begin(), act.end());
//				cout << "doing random operations\n";
				DO_RANDOM_OPERATIONS(act, exp);
			}
//			cout << "END DEBUG SECTION\n\n";
		}
	}
	SECTION("non empty map")
	{
		SECTION("lvalue template arg")
		{
			CREATE_MAPS_INT_STRING(act, exp);

			SECTION("do random operations")
			{
				DO_RANDOM_OPERATIONS(act, exp);
			}

			pair<int const, string> val(2000, rand_string());


			auto p11 = act.insert<pair<int const, string>&>(val); // instantiate P as reference
			auto p22 = exp.insert<pair<int const, string>&>(val);

			REQUIRE(*p11.first == val);
			REQUIRE(*p22.first == val);
			REQUIRE(p11.second == p22.second);

			MAPS_REQUIRE_EQUAL(act, exp);

			SECTION("do random operations")
			{
				DO_RANDOM_OPERATIONS(act, exp);
			}
		}
		SECTION("rvalue template arg")
		{
			CREATE_MAPS_INT_STRING(act, exp);

			SECTION("do random operations")
			{
				DO_RANDOM_OPERATIONS(act, exp);
			}

			pair<int const, string> vall(rand_int(2000, 3000), rand_string());
			pair<int const, string> val1 = vall;
			pair<int const, string> val2 = vall;

			auto p1 = act.insert<pair<int const, string>>(std::move(val1));
			auto p2 = exp.insert<pair<int const, string>>(std::move(val2));

			REQUIRE(*p1.first == *p2.first);
			REQUIRE(p1.second == p2.second);
			REQUIRE(val1 == val2);

			MAPS_REQUIRE_EQUAL(act, exp);

			SECTION("do random operations")
			{
				DO_RANDOM_OPERATIONS(act, exp);
			}
		}
	}
}

TEST_CASE("range insert", tag)
{
	SECTION("empty map and non empty")
	{
		auto input = rand_pairs_of_len(rand_int(1, 1000));

		my_map<int, string> act;
		std_map<int, string> exp;

		act.insert(input.begin(), input.end());
		exp.insert(input.begin(), input.end());
		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations 1")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		auto input2 = rand_pairs_of_len(rand_int(1, 1000));

		act.insert(input2.begin(), input2.end());
		exp.insert(input2.begin(), input2.end());
		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations 2")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
	SECTION("insert existing")
	{
		CREATE_MAPS_INT_STRING(act, exp);

		auto start = exp.begin();
		auto stop = exp.end();
		advance(start, rand_int(0, exp.size() - 1));

		vector<pair<int, string>> input(start, stop);
		input.insert(input.end(), rand_pairs);

		SECTION("do random operations 1")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		act.insert(input.begin(), input.end());
		exp.insert(input.begin(), input.end());

		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations 2")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}

}

TEST_CASE("initializer list insert", tag)
{
	SECTION("empty map and non empty")
	{
		auto input = rand_pairs_of_len(rand_int(1, 1000));

		my_map<int, string> act;
		std_map<int, string> exp;

		act.insert(init_list_1);
		exp.insert(init_list_1);
		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations 1")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}

		auto input2 = rand_pairs_of_len(rand_int(1, 1000));

		act.insert(init_list_2);
		exp.insert(init_list_2);
		MAPS_REQUIRE_EQUAL(act, exp);

		SECTION("do random operations 2")
		{
			DO_RANDOM_OPERATIONS(act, exp);
		}
	}
}


TEST_CASE("insert with hint", tag)
{

}