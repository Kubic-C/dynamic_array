
#define DYNM_ARR_VECTOR_COMPATIBLE
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

	std::vector<int> vector = dynm_arr;
	dynm_arr = kubic::convert_to_dynm_arr(vector);
	std::vector<int> test = { 0, 1, 2, 3 };

	dynm_arr.insert(dynm_arr.end(), test.begin(), test.end());
	
	dynm_arr = { 5, 1, 2 };

	std::cout 
		<< "vector: \n"
		<< "vector capacity: " << dynm_arr.capacity() << '\n'
		<< "vector size: " << dynm_arr.size() << '\n';

	for (int& i : dynm_arr)
		std::cout << i << '\n';


	std::cin.get();
	return 0;
}