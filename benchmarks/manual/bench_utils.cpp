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
	return std::move(s);
}

std::vector<std::pair<std::string, std::string const>> generate_unsorted_range(int N, int strlen)
{
	std::vector<std::pair<std::string, std::string const>> result;
	result.reserve(N);

	for(int i = 0; i < N; ++i)
	{
		result.emplace_back(std::make_pair(rand_string(strlen, strlen), rand_string(1, 1)));
	}

	return std::move(result);
}

std::vector<std::pair<std::string, std::string>> generate_sorted_range(int N, int strlen)
{
	union R
	{
		R(std::vector<std::pair<std::string, std::string const>>&& data)
			: data_nc_c(std::move(data))
		{
		}

		std::vector<std::pair<std::string, std::string>> data_nc_nc;
		std::vector<std::pair<std::string, std::string const>> data_nc_c;
		std::vector<std::pair<std::string const, std::string>> data_c_nc;

		~R()
		{
			data_nc_c.~vector();
		}
	};

	R r(std::move(generate_unsorted_range(N, strlen)));
	std::vector<std::pair<std::string, std::string>>& result = r.data_nc_nc;

	std::sort(result.begin(), result.end(), ss_pair_cmp());

	return std::move(r.data_nc_nc);
}
