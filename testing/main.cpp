#define DYNM_ARR_VECTOR_COMPATIBLE
#include <dynamic_array.h>
#include <iostream>
#include <chrono>

int main()
{
	std::vector<int> vector = { 0 };
	kubic::dynamic_array<int> dynm_arr = { 0 };

	auto start = std::chrono::high_resolution_clock::now();
	vector.reserve(10);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> dur = end - start;
	std::cout << "time took for vector.reserve()(ms): " << dur.count() * 1000 << '\n';

	start = std::chrono::high_resolution_clock::now();
	dynm_arr.reserve(10);
	end = std::chrono::high_resolution_clock::now();
	dur = end - start;
	std::cout << "time took for dynm_arr.reserve()(ms): " << dur.count() * 1000 << '\n';

	return 0;
}