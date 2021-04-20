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

std::vector<std::pair<std::string, std::string>> generate_unsorted_range(int N)
{
	std::vector<std::pair<std::string, std::string>> result(N);

	int strlen = 100;
	for(int i = 0; i < N; ++i)
	{
		result[i] = std::make_pair(rand_string(strlen, strlen), rand_string(strlen, strlen));
	}

	return result;
}

std::vector<std::pair<std::string, std::string>> generate_sorted_range(int N)
{
	std::vector<std::pair<std::string, std::string>> result = generate_unsorted_range(N);
	std::sort(result.begin(), result.end(), ss_pair_cmp());
	return result;
}
