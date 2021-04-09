//
// Created by Isa on 4/10/21.
//

#include <bench_utils.h>


static void copy_ctor(benchmark::State& state)
{
	const int n = state.range(0);
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(n);
	const target::map<std::string, std::string> input(sorted.begin(), sorted.end());

	while(state.KeepRunning())
	{
		target::map<std::string, std::string> m(input);
	}
}


BENCHMARK(copy_ctor)->Send_Args();