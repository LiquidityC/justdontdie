#include <random>

int getRandomBetween(int a, int b)
{
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> dis(a, b);
	return dis(gen);
}
