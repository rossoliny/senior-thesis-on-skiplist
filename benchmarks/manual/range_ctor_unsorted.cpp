//
// Created by Isa on 4/11/21.
//

#include <bench_utils.h>
#include <iostream>

template<typename Map, typename Input>
long long static range_ctor_bench(int const RUNS, Input& input)
{
	int len = 0;
	auto start = std::chrono::high_resolution_clock::now();

	int run = 0;
	while(run++ < RUNS)
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
	return std::chrono::duration_cast<std::chrono::milliseconds>(res).count() / RUNS;
}

const auto /*vector*/ input = generate_sorted_range(_100_THOUSANDS, _10_THOUSANDS);

int main()
{
	escape(&input);
	int RUNS = 50;

	auto my_map_ms = range_ctor_bench<std::map<std::string const, std::string const>>(RUNS, input);
	auto std_map_ms = range_ctor_bench<std::map<std::string const, std::string const>>(RUNS, input);

	my_map_ms = range_ctor_bench<isa::map<std::string const, std::string const>>(RUNS, input);
	std_map_ms = range_ctor_bench<isa::map<std::string const, std::string const>>(RUNS, input);

	escape(&std_map_ms);
	escape(&my_map_ms);
	clobber();
	std::cout << "range ctor bench, unsorted input\n";
	std::cout << "isa::map:\t" << (my_map_ms) << "\tms " << "\truns:\t" << RUNS << std::endl;
	std::cout << "std::map:\t" << (std_map_ms) << "\tms " << "\truns:\t" << RUNS << std::endl;
	escape(&input);
	escape(&std_map_ms);
	escape(&my_map_ms);
	clobber();

	for(int i = 0; i < input.size(); ++i)
	{
		escape(input[i].first.c_str());
		escape(input[i].second.c_str());
		clobber();
	}
}

