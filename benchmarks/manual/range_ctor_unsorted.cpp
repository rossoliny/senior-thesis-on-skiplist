//
// Created by Isa on 4/11/21.
//

#include <bench_utils.h>
#include <iostream>

template<typename Map, typename Input>
long long static range_ctor_bench(int RUNS, Input& input)
{
	int len = 0;
	auto start = std::chrono::high_resolution_clock::now();

	while(RUNS--)
	{
		escape(&input);
		Map m(input.begin(), input.end());
		len = m.size();
		escape(&m);
		clobber();
	}

	auto end = std::chrono::high_resolution_clock::now();

	auto res = end - start;

	std::cout << "\tsize = " << len << std::endl;
	return std::chrono::duration_cast<std::chrono::milliseconds>(res).count();
}

auto input = generate_sorted_range(_100_THOUSANDS, _10_THOUSANDS);

int main()
{
	escape(&input);
	int RUNS = 1;


	auto my_map_ms = range_ctor_bench<isa::map<std::string, std::string>>(RUNS, input);
	auto std_map_ms = range_ctor_bench<std::map<std::string, std::string>>(RUNS, input);

	escape(&std_map_ms);
	escape(&my_map_ms);
	clobber();
	std::cout << "range ctor bench, unsorted input\n";
	std::cout << "isa::map:\t" << (my_map_ms) << "\tmilliseconds" << std::endl;
	std::cout << "std::map:\t" << (std_map_ms) << "\tmilliseconds" << std::endl;
	escape(&std_map_ms);
	escape(&my_map_ms);
	clobber();
}

