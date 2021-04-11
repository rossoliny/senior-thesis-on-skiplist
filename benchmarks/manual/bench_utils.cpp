//
// Created by Isa on 4/10/21.
//

#include <bench_utils.h>

 int rand_int(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

 char rand_char()
{
	return rand_int('A', 'Z');
}

 std::string rand_string(int min_len, int max_len)
{
	int len = rand_int(min_len, max_len);
	std::string s(len, 'l');
	while(len--)
	{
		s[len] = rand_char();
	}
	return s;
}

std::vector<std::pair<std::string, std::string const>> generate_unsorted_range(int N, int strlen)
{
	std::vector<std::pair<std::string, std::string const>> result;
	result.reserve(N);

	for(int i = 0; i < N; ++i)
	{
		result.push_back(std::make_pair(rand_string(strlen, strlen), rand_string(strlen, strlen)));
	}

	return result;
}

std::vector<std::pair<std::string, std::string const>> generate_sorted_range(int N, int strlen)
{
	std::vector<std::pair<std::string, std::string const>> result = generate_unsorted_range(N, strlen);
//	std::sort(result.begin(), result.end(), ss_pair_cmp());
	return result;
}
