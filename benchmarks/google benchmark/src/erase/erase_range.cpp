//
// Created by Isa on 4/10/21.
//

#include <bench_utils.h>



static void range_erase(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0));

	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m(sorted.begin(), sorted.end());
		auto first = advance(m.begin(), m.end(), rand_int(0, m.size() - 1));
		auto last = advance(first, m.end(), rand_int(1, std::distance(first, m.end())));
		state.ResumeTiming();

		m.erase(first, last);
	}
}

BENCHMARK(range_erase)->Send_Args();