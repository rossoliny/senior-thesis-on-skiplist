////
//// Created by Isa on 4/9/21.
////
//
//#include <bench_utils.h>
//
//
//static void range_ctor(benchmark::State& state)
//{
//	while(state.KeepRunning())
//	{
//		map<std::string, std::string> m;
//	}
//}
//
//static void range_ctor_with_alloc(benchmark::State& state)
//{
//	std::allocator<std::pair<std::string, std::string>> alloc;
//
//	while(state.KeepRunning())
//	{
//		map<std::string, std::string> m(alloc);
//	}
//}
//
//
//BENCHMARK(range_ctor);
//BENCHMARK(range_ctor_with_alloc);
//
