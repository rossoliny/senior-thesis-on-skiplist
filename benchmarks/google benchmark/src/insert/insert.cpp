//
// Created by Isa on 4/14/21.
//

#include <bench_utils.h>


static void insert(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> input = generate_sorted_range(state.range(0));

	int strlen = 100;
	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m(input.begin(), input.end());
		auto val = std::make_pair(rand_string(strlen, strlen), rand_string(strlen, strlen));
		state.ResumeTiming();

		m.insert(std::move(val));
	}
}


BENCHMARK(insert)->Send_Args();
