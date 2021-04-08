#include <iostream>
#include <smap>
#include <map>
#include <string>
#include <chrono>
#include <thread>


int main()
{
	using namespace std::literals::chrono_literals;

	isa::map<std::string, int> m1;
	std::map<std::string, int> m2;

	auto p = std::make_pair("short string", 10);

	auto start = std::chrono::high_resolution_clock::now();
	m1.insert(p);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end - start;

	auto start2 = std::chrono::high_resolution_clock::now();
	m2.insert(p);
	auto end2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed2 = end2 - start2;

	std::cout << "isa::map::insert:\t" << (elapsed.count() * 1000) << " ms" << std::endl;
	std::cout << "std::map::insert:\t" << (elapsed2.count() * 1000) << " ms" << std::endl;

	return 0;
}
