//
// Created by Isa on 4/10/21.
//

#include <bench_utils.h>

static void copy_assignment_to_empty(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0));
	target::map<std::string, std::string> input(sorted.begin(), sorted.end());

	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m;
		state.ResumeTiming();

		m = input;
	}
}

static void copy_assignment_equal_size(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0));
	target::map<std::string, std::string> input(sorted.begin(), sorted.end());

	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m(input);
		state.ResumeTiming();

		m = input;
	}
}

static void copy_assignment_to_longer(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0) * 2);
	const std::vector<std::pair<std::string, std::string>> sorted_L = generate_sorted_range(state.range(0));

	target::map<std::string, std::string> input(sorted.begin(), sorted.end());

	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m(sorted_L.begin(), sorted_L.end());
		state.ResumeTiming();

		m = input;
	}
}

static void copy_assignment_to_shorter(benchmark::State& state)
{
	const std::vector<std::pair<std::string, std::string>> sorted = generate_sorted_range(state.range(0) / 2);
	const std::vector<std::pair<std::string, std::string>> sorted_L = generate_sorted_range(state.range(0));

	target::map<std::string, std::string> input(sorted_L.begin(), sorted_L.end());

	while(state.KeepRunning())
	{
		state.PauseTiming();
		target::map<std::string, std::string> m(sorted.begin(), sorted.end());
		state.ResumeTiming();

		m = input;
	}
}


BENCHMARK(copy_assignment_to_empty)->Send_Args();
BENCHMARK(copy_assignment_to_shorter)->Send_Args();
BENCHMARK(copy_assignment_equal_size)->Send_Args();
BENCHMARK(copy_assignment_to_longer)->Send_Args();