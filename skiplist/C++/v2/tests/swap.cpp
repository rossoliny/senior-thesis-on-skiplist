//
// Created by Isa on 4/2/21.
//

#include "test_utils.h"
#include <iostream>
#define tag "[swap]"

TEST_CASE("smap::swap")
{
//	cout << "\nSWAP TEST\n" << endl;
//	srand(99999); // 2 gives malloc error // 7 infinite loop // 8,11 segfault // 99999->19==17
//	cout << "creating first\n";
	CREATE_MAPS_INT_STRING(act1, exp1);
//	cout << "created first\n";

//	cout << "\ncreating second\n";
	CREATE_MAPS_INT_STRING(act2, exp2);
//	cout << "created second\n";

//	cout << "doing random operations on first\n";
	int N = 11;
	DO_RANDOM_OPERATIONS(act1, exp1);
//	cout << "finished\n";

//	cout << "before random operations of second\n";
//	print(act2.begin(), act2.end());
	DO_RANDOM_OPERATIONS(act2, exp2); // fix segfault
//	cout << "after\n";
//	print(act2.begin(), act2.end());


	MAPS_REQUIRE_EQUAL(act1, exp1);
	MAPS_REQUIRE_EQUAL(act2, exp2);


//	cout << "\nBEFORE SWAP\n";
//	cout << "first:\n";
//	print(act1);
//	cout << "\nsecond\n";
//	print(act2);
//	cout << "\ndoing swap\n";
	act1.swap(act2); // error in swap creates infinite loop
//	cout << "done\n";

	MAPS_REQUIRE_EQUAL(act2, exp1);
	MAPS_REQUIRE_EQUAL(act1, exp2);

	DO_RANDOM_OPERATIONS(act2, exp1);
	DO_RANDOM_OPERATIONS(act1, exp2);

	MAPS_REQUIRE_EQUAL(act2, exp1);
	MAPS_REQUIRE_EQUAL(act1, exp2);

	exp1.swap(exp2);

	MAPS_REQUIRE_EQUAL(act2, exp2);
	MAPS_REQUIRE_EQUAL(act1, exp1);

	DO_RANDOM_OPERATIONS(act2, exp2);
	DO_RANDOM_OPERATIONS(act1, exp1);

	MAPS_REQUIRE_EQUAL(act2, exp2);
	MAPS_REQUIRE_EQUAL(act1, exp1);
}


TEST_CASE("std::swap")
{
	CREATE_MAPS_INT_STRING(act1, exp1);
	CREATE_MAPS_INT_STRING(act2, exp2);

	DO_RANDOM_OPERATIONS(act1, exp1);
	DO_RANDOM_OPERATIONS(act2, exp2);

	MAPS_REQUIRE_EQUAL(act1, exp1);
	MAPS_REQUIRE_EQUAL(act2, exp2);

	std::swap(act1, act2);

	MAPS_REQUIRE_EQUAL(act2, exp1);
	MAPS_REQUIRE_EQUAL(act1, exp2);

	DO_RANDOM_OPERATIONS(act2, exp1);
	DO_RANDOM_OPERATIONS(act1, exp2);

	MAPS_REQUIRE_EQUAL(act2, exp1);
	MAPS_REQUIRE_EQUAL(act1, exp2);

	std::swap(exp1, exp2);

	MAPS_REQUIRE_EQUAL(act2, exp2);
	MAPS_REQUIRE_EQUAL(act1, exp1);

	DO_RANDOM_OPERATIONS(act2, exp2);
	DO_RANDOM_OPERATIONS(act1, exp1);

	MAPS_REQUIRE_EQUAL(act2, exp2);
	MAPS_REQUIRE_EQUAL(act1, exp1);
}