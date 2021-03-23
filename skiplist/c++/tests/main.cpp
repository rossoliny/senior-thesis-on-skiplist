//
// Created by Isa on 3/23/21.
//

#include "skiplist.h"
#include <string>
#include <map>

using namespace std;

int main()
{
	srand(228765);
	isa::map<int, std::string> m;

//	m.insert(std::make_pair(1, '1'));

	auto p1 = m.insert(std::make_pair(1, "isa"));
	auto p3 = m.insert(std::make_pair(3, "isa3"));
	auto p2 = m.insert(std::make_pair(2, "isa2"));
	auto p5 = m.insert(std::make_pair(5, "isa5"));
	auto p4 = m.insert(std::make_pair(4, "isa4"));
	auto p6 = m.insert(std::make_pair(6, "isa6"));
	auto p7 = m.insert(std::make_pair(7, "isa7"));
//	auto p10 = m.insert(std::make_pair(10, "isa10"));
//	auto p12 = m.insert(std::make_pair(12, "isa12"));
//	auto p8 = m.insert(std::make_pair(8, "isa8"));
//	auto p9 = m.insert(std::make_pair(9, "isa9"));
//	auto p13 = m.insert(std::make_pair(13, "isa13"));
//	auto p11 = m.insert(std::make_pair(11, "isa11"));

	m.erase(5);
	m.erase(2);
	m.erase(1);
	m.erase(3);
	m.erase(2);
	m.erase(6);

	std::pair<const int, char> pair = std::make_pair(228, 'X');
	std::map<int, char>::pointer p = &pair;
	isa::map<int, char>::pointer pp = &pair;

}