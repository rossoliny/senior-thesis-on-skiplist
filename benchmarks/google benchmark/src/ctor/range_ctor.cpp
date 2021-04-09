////
//// Created by Isa on 4/9/21.
////

#include <bench_utils.h>


static void range_ctor_unsorted(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> unsorted = generate_unsorted_range(state.range(0));

	while(state.KeepRunning())
	{
		target::map<std::string, std::string> m(unsorted.begin(), unsorted.end());
	}
}

static void range_ctor_sorted(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0));

	while(state.KeepRunning())
	{
		target::map<std::string, std::string> m(sorted.begin(), sorted.end());
	}
}


BENCHMARK(range_ctor_unsorted)->Send_Args();
BENCHMARK(range_ctor_sorted)->Send_Args();
