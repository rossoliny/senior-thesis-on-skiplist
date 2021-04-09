//
// Created by Isa on 4/10/21.
//

#include <bench_utils.h>




static void erase(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0));

	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m(sorted.begin(), sorted.end());
		auto pos = advance(m.begin(), --m.end(), rand_int(0, m.size() - 1));
		std::string const& key = pos->first;
		state.ResumeTiming();

		m.erase(key);
	}
}

BENCHMARK(erase)->Send_Args();