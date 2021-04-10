//
// Created by Isa on 4/10/21.
//

#include <bench_utils.h>




static void swap(benchmark::State& state)
{
	const static std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0));

	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m1(sorted.begin(), sorted.end());
		target::map<std::string, std::string> m2(sorted.begin(), sorted.end());
		state.ResumeTiming();

		m1.swap(m2);
	}
}

BENCHMARK(swap)->Send_Args();