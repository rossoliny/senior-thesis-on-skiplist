//
// Created by Isa on 4/10/21.
//

#include <bench_utils.h>



static void find(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0));
	target::map<std::string, std::string> m(sorted.begin(), sorted.end());

	while(state.KeepRunning())
	{
		state.PauseTiming();
		auto key = sorted[rand_int(0, sorted.size() - 1)].first;
		state.ResumeTiming();

		m.find(key);
	}
}

BENCHMARK(find)->Send_Args();