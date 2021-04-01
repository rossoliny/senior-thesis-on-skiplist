//
// Created by Isa on 3/23/21.
//

#include "skiplist.h"
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main()
{
	std::vector<pair<int, string>> v = {make_pair(4, "44"),  make_pair(4, "4"), make_pair(1, "1"), make_pair(2, "2"), make_pair(3, "3")};
	srand(228765);
	isa::map<int, std::string> m;
	isa::map<int, std::string> mrange(v.rbegin(), v.rend());
	isa::map<int, std::string> mcpy(mrange);
	isa::map<int, std::string> mcpyalloc(mcpy, std::allocator<std::pair<int, string>>());
	isa::map<int, std::string> mmove(std::move(mcpy));
	isa::map<int, std::string> mmovealloc(std::move(mmove), std::allocator<std::pair<int, string>>());

	std::map<int, string> sm(v.begin(), v.end());
	cout << sm.begin()->first << endl;
	m.insert(std::make_pair(1, "1"));

	auto p1 = m.insert(std::make_pair(1, "isa"));
	auto p3 = m.insert(std::make_pair(3, "isa3"));
	auto p2 = m.insert(std::make_pair(2, "isa2"));
	auto p5 = m.insert(std::make_pair(5, "isa5"));
	auto p4 = m.insert(std::make_pair(4, "isa4"));
	auto p6 = m.insert(std::make_pair(6, "isa6"));
	auto p7 = m.insert(std::make_pair(7, "isa7"));
	auto p10 = m.insert(std::make_pair(10, "isa10"));
	auto p12 = m.insert(std::make_pair(12, "isa12"));
	auto p8 = m.insert(std::make_pair(8, "isa8"));
	auto p9 = m.insert(std::make_pair(9, "isa9"));
	auto p13 = m.insert(std::make_pair(13, "isa13"));
	auto p11 = m.insert(std::make_pair(11, "isa11"));

	m.erase(5);
	m.erase(2);
	m.erase(1);
	m.erase(3);
	m.erase(2);
	m.erase(6);

	std::pair<const int, char> pair_type = std::make_pair(228, 'X');
	std::map<int, char>::pointer p = &pair_type;
	isa::map<int, char>::pointer pp = &pair_type;

}
