#include <random>

int getRandomBetween(int a, int b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(a, b);
	return dis(gen);
}
