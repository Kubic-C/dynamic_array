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
	std::vector<int> test = { 100, 101 };

	vector.insert(vector.begin(), test.begin(), test.end());
	std::cout 
		<< "vector: \n";

	for (int& i : vector)
		std::cout << i << '\n';

	for (;;)
	{
		dynm_arr.insert(0, &test[0], &test[1]);
		std::cout
			<< "dynamic array: \n";

		for (int& i : dynm_arr)
			std::cout << i << '\n';
	}

	std::cin.get();
	return 0;
}