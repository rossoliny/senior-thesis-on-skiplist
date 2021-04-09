//
// Created by Isa on 4/9/21.
//

#include <bench_utils.h>

static void empty_ctor(benchmark::State& state)
{
	while(state.KeepRunning())
	{
		target::map<std::string, std::string> m;
	}
}

static void empty_ctor_with_alloc(benchmark::State& state)
{
	std::allocator<std::pair<std::string, std::string>> alloc;
	while(state.KeepRunning())
	{
		target::map<std::string, std::string> m(alloc);
	}
}


BENCHMARK(empty_ctor);
BENCHMARK(empty_ctor_with_alloc);
