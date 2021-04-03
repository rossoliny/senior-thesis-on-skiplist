//
// Created by Isa on 4/2/21.
//

#include "test_utils.h"
#include <iostream>
#define tag "[swap]"

template<typename I>
void print(I f, I l)
{
	int i = 0;
	while(f != l)
	{
		cout << i++ << "\t: (" << f->first << ", " << f->second << ")\n";
		++f;
	}
	cout << endl;
}

TEST_CASE("map::swap")
{
	cout << "\nSWAP TEST\n" << endl;
	srand(7); // 2 gives malloc error // 7 infinite loop // 8,11 segfault // 99999->19==17
	cout << "creating first\n";
	CREATE_MAPS_INT_STRING(act1, exp1);
	cout << "created first\n";

	cout << "\ncreating second\n";
	CREATE_MAPS_INT_STRING(act2, exp2);
	cout << "created second\n";

	cout << "randops1\n";
	int N = 11;
	DO_RANDOM_OPERATIONS(act1, exp1, N);
	cout << "randops1 end\n";

	cout << "before\n";
	print(act2.begin(), act2.end());
	DO_RANDOM_OPERATIONS(act2, exp2, N); // fix segfault
	cout << "after\n";
	print(act2.begin(), act2.end());


	MAPS_REQUIRE_EQUAL(act1, exp1);
	MAPS_REQUIRE_EQUAL(act2, exp2);

	act1.swap(act2); // error in swap creates infinite loop

	MAPS_REQUIRE_EQUAL(act2, exp1);
	MAPS_REQUIRE_EQUAL(act1, exp2);

	DO_RANDOM_OPERATIONS(act2, exp1, N);
	DO_RANDOM_OPERATIONS(act1, exp2, N);

	MAPS_REQUIRE_EQUAL(act2, exp1);
	MAPS_REQUIRE_EQUAL(act1, exp2);
	exp1.swap(exp2);

	MAPS_REQUIRE_EQUAL(act2, exp2);
	MAPS_REQUIRE_EQUAL(act1, exp1);
}


TEST_CASE("std::swap")
{
	CREATE_MAPS_INT_STRING(act1, exp1);
	CREATE_MAPS_INT_STRING(act2, exp2);

	std::swap(act1, act2);

	MAPS_REQUIRE_EQUAL(act1, exp2);
	MAPS_REQUIRE_EQUAL(act2, exp1);

	std::swap(exp1, exp2);

	MAPS_REQUIRE_EQUAL(act1, exp1);
	MAPS_REQUIRE_EQUAL(act2, exp2);
}