#include <iostream>
#include <smap>
#include <map>
#include <string>
#include <chrono>
#include <thread>


static void escape(void* p)
{
	asm volatile("" : : "g"(p) : "memory");
}

static void clobber()
{
	asm volatile("" : : : "memory");
}


const auto p = std::make_pair("not short string. allocated on heap", 10);
const int N = 25;

isa::map<std::string, int> m1;
std::map<std::string, int> m2;

std::chrono::duration<float> bench_skiplist()
{
	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < N; ++i)
	{
		escape(&m1);
		m1.insert(p);
		escape(&m1);
		clobber();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end - start;

	return elapsed;
}

std::chrono::duration<float> bench_rbtree()
{
	auto start = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < N; ++i)
	{
		escape(&m2);
		m2.insert(p);
		escape(&m2);
		clobber();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end - start;

	return elapsed;
}

int main()
{
//	using namespace std::literals::chrono_literals;

	auto elapsed1 = bench_skiplist();
	auto elapsed2 = bench_rbtree();

	std::cout << "isa::map::insert:\t" << (elapsed1.count() * 1000) << "\tms" << std::endl;
	std::cout << "std::map::insert:\t" << (elapsed2.count() * 1000) << "\tms" << std::endl;
	std::cout << "ratio:\t" << (elapsed1.count() / elapsed2.count()) << std::endl;

	return 0;
}
