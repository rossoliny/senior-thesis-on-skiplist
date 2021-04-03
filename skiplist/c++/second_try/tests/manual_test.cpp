//
// Created by Isa on 3/23/21.
//

#include "test_utils.h"
#include "skiplist.h"
#include <utility>
#include <map>

using namespace std;


int main()
{
	isa::map<int, string> my_m {make_pair(4, "4"), make_pair(4, "44"), make_pair(1, "1"), make_pair(2, "2"), make_pair(3, "3")};
	std::map<int, string> std_m {make_pair(4, "4"),  make_pair(4, "44"), make_pair(1, "1"), make_pair(2, "2"), make_pair(3, "3")};
//	srand(1);
//	DO_RANDOM_OPERATIONS(my_m, std_m);

	auto p5 = my_m.insert(std::make_pair(5, "isa5"));
	my_m.erase(4);
	auto p4 = my_m.insert(std::make_pair(4, "isa4"));
	my_m.erase(5);
	auto p6 = my_m.insert(std::make_pair(6, "isa6"));
	auto p7 = my_m.insert(std::make_pair(7, "isa7"));
	auto p10 = my_m.insert(std::make_pair(10, "isa10"));
	auto p12 = my_m.insert(std::make_pair(12, "isa12"));
	auto p8 = my_m.insert(std::make_pair(8, "isa8"));
	auto p9 = my_m.insert(std::make_pair(9, "isa9"));
	auto p13 = my_m.insert(std::make_pair(13, "isa13"));
	auto p11 = my_m.insert(std::make_pair(11, "isa11"));

	my_m.erase(2);
	my_m.erase(1);
	my_m.erase(3);
	my_m.erase(2);
	my_m.erase(6);



}