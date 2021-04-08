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

int rand_int(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

char rand_char()
{
	return rand_int('A', 'Z');
}

std::string rand_string(int min_len, int max_len)
{
	int len = rand_int(min_len, max_len);
	std::string s(len, 'l');
	while(len--)
	{
		s[len] = rand_char();
	}
	return s;
}


const int N = 25;
int min_string = 20;
int max_string = 120;

isa::map<std::string, int> m1;
std::map<std::string, int> m2;

std::chrono::duration<float> bench_skiplist()
{
	static int val = 1;

	const auto p = std::make_pair(rand_string(min_string, max_string), val++);

	auto start = std::chrono::high_resolution_clock::now();

		escape(&m1);
		m1.insert(p);
		escape(&m1);
		clobber();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end - start;

	return elapsed;
}

std::chrono::duration<float> bench_rbtree()
{
	static int val = 1;

	const auto p = std::make_pair(rand_string(min_string, max_string), val++);

	auto start = std::chrono::high_resolution_clock::now();

		escape(&m2);
		m2.insert(p);
		escape(&m2);
		clobber();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end - start;

	return elapsed;
}

int main()
{
//	using namespace std::literals::chrono_literals;
	int RUNS = 10000;


	for(int i = 0; i < RUNS; ++i)
	{
		escape(&m1);
		const volatile auto elapsed1 = bench_skiplist();
		escape(&m1);
	}

	for(int i = 0; i < RUNS; ++i)
	{
		escape(&m2);
		const volatile auto elapsed2 = bench_rbtree();
		escape(&m2);
	}


	if(m1.size() != m2.size())
	{
		exit(-1);
	}
//	std::cout << "isa::map::insert:\t" << (elapsed1.count() * 1000) << "\tms" << std::endl;
//	std::cout << "std::map::insert:\t" << (elapsed2.count() * 1000) << "\tms" << std::endl;
//	std::cout << "ratio:\t" << (elapsed1.count() / elapsed2.count()) << std::endl;

//	std::this_thread::sleep_for(1000ms);
}
