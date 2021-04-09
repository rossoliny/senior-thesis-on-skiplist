//
// Created by Isa on 4/10/21.
//

#include <bench_utils.h>



static void clear(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0));

	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m(sorted.begin(), sorted.end());
		state.ResumeTiming();

		m.clear();
	}
}

BENCHMARK(clear)->Send_Args();