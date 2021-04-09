//
// Created by Isa on 4/6/21.
//

#include "test_utils.h"

int seed_rand()
{
	unsigned seed = -1;
	srand(time(NULL));
	return 1;
}

const volatile static int dummy = seed_rand();

int rand_int(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

char rand_char()
{
	return rand_int(65, 90);
}
std::string rand_string()
{
	int len = rand() % 50 + 1;
	std::string s;
	while(len--)
	{
		s += rand_char();
	}
	return s;
}

int print(my_map<int, std::string>& m)
{
	print(m.begin(), m.end());
	return 0;
}

const std::initializer_list<std::pair<int const, std::string>> init_list_1 = rand_pairs;
const std::initializer_list<std::pair<int const, std::string>> init_list_2 = rand_pairs;
const std::initializer_list<std::pair<int const, std::string>> init_list_L = rand_pairs;

const  std::vector<std::pair<std::string const, int>> vec_pairs = {
	std::make_pair("smap", 1),     std::make_pair("unit", 2), std::make_pair("test", 3),  std::make_pair("auca", 4),
	std::make_pair("std::smap", 5), std::make_pair("ab", 6),   std::make_pair("cd", 7),    std::make_pair("ef", 8),
	std::make_pair("gh", 9),       std::make_pair("ij", 10),  std::make_pair("kl", 11),   std::make_pair("mnop", 12),
	std::make_pair("qrstuv", 13),  std::make_pair("wxy", 14), std::make_pair("z", 15)
};

const std::initializer_list<std::pair<int const, std::string>> initializer_list_pairs = {
	std::make_pair(1, "smap"), std::make_pair(2, "unit"), std::make_pair(3, "test"),  std::make_pair(4, "auca"),
	std::make_pair(5, "std::smap"), std::make_pair(6, "ab"),   std::make_pair(7, "cd"),    std::make_pair(8, "ef"),
	std::make_pair(9, "gh"),       std::make_pair(10, "ij"),  std::make_pair(11, "kl"),   std::make_pair(12, "mnop"),
	std::make_pair(13, "qrstuv"),  std::make_pair(14, "wxy"), std::make_pair(15, "z")
};

const std::initializer_list<std::pair<int const, std::string>> initializer_list_pairs_L = {
	std::make_pair(1, "smap"), std::make_pair(2, "unit"),   std::make_pair(3, "test"),  std::make_pair(4, "auca"),
	std::make_pair(5, "std::smap"), std::make_pair(6, "ab"),     std::make_pair(7, "cd"),    std::make_pair(8, "ef"),
	std::make_pair(9, "gh"),       std::make_pair(10, "ij"),    std::make_pair(11, "kl"),   std::make_pair(12, "mnop"),
	std::make_pair(13, "qrstuv"),  std::make_pair(14, "wxy"),   std::make_pair(15, "z"),    std::make_pair(16, "16"),
	std::make_pair(17, "qwer"),    std::make_pair(18, "tyui"),  std::make_pair(19, "op[]"), std::make_pair(20, "20"),
	std::make_pair(21, "asdf"),    std::make_pair(22, "ghjk"),  std::make_pair(23, "zxcv"), std::make_pair(24, "24"),
};


std::set<std::pair<int const, std::string>> rand_pairs_of_len(int len)
{
	std::set<std::pair<int const, std::string>> res;
	while(res.size() != len)
	{
		res.insert(rand_pair());
	}
	return res;
}

template<typename I, typename I2>
void random_advance(I& start1, I end1, I2& start2, I2 end2)
{
	while(rand() > INT_MAX / 2 && start1 != end1 && start2 != end2)
	{
		++start1;
		++start2;
	}
}

void do_random_insertions_and_deletions(my_map<int, std::string>& act, std_map<int, std::string>& exp)
{
	int N = rand_int(10, 20);
	std::stack<std::pair<int const, std::string>> history;
//	history.reserve(N);

	int i = 0;
	while(i++ < N)
	{
		int cmd = rand();

		int lim = INT_MAX / 2;
		if(cmd < lim || history.empty())
		{
			lim = INT_MAX / 4;
			if(cmd < lim)
			{
				// insert
				auto p = rand_pair();
				int k = p.first;

				act.insert(p);
				exp.insert(p);
			}
			else
			{
				// range insert
				std::initializer_list<std::pair<int const, std::string>> inp = rand_pairs;
				std::initializer_list<std::pair<int const, std::string>> inp2 = inp;

				act.insert(inp.begin(), inp.end());
				exp.insert(inp2.begin(), inp2.end());

				for(const auto& p : inp)
				{
					history.push(p);
				}
			}

		}
		else if(!history.empty())
		{
			int lim = 3 * (INT_MAX / 4);
			if(cmd >= lim)
			{
				// erase
				auto pos = history.top();
				history.pop();

				act.erase(pos.first);
				exp.erase(pos.first);
			}
			else if(!exp.empty())
			{
				// erase range
				auto begin1 = act.begin();
				auto begin2 = exp.begin();
				random_advance(begin1, act.end(), begin2, exp.end());

				auto end1 = begin1;
				auto end2 = begin2;
				random_advance(end1, act.end(), end2, exp.end());

				if(begin1 != end1 && begin2 != end2)
				{
					act.erase(begin1, end1);
					exp.erase(begin2, end2);
				}
			}
		}
	}
	MAPS_REQUIRE_EQUAL(act, exp);
}

