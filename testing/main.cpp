#include <dynamic_array.h>
#include <iostream>
#include <vector>
#include <string>

int main()
{
	kubic::dynamic_array<int> dynm_arr;
	dynm_arr.push_back(0);
	dynm_arr.push_back(1);
	dynm_arr.push_back(2);
	dynm_arr.push_back(3);

	std::vector<int> vector = { 0, 1, 2, 3 };

	vector.resize(6);
	std::cout 
		<< "vector: \n"
		<< "vector capacity: " << vector.capacity() << '\n'
		<< "vector size: " << vector.size() << '\n';

	dynm_arr.resize(6);
	std::cout
		<< "dynm_arr: \n"
		<< "dynm_arr capacity: " << dynm_arr.capacity() << '\n'
		<< "dynm_arr size: " << dynm_arr.size() << '\n';


	std::cin.get();
	return 0;
}