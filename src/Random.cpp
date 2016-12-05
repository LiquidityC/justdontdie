#include <random>

int getRandomBetween(int a, int b)
{
	std::random_device rd;
	std::minstd_rand gen(rd());
	std::uniform_int_distribution<int> dis(a, b);
	return dis(gen);
}
