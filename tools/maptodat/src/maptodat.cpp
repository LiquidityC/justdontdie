#include <iostream>
#include <fstream>
#include <string>

void print_usage(std::string cmd)
{
	std::cerr << "Usage: " << cmd << " [infile] [outfile]" << std::endl;
}

int main(int argc, char* argv[])
{
	std::string command(argv[0]);
	if (argc < 2) {
		std::cerr << "You need to supply two filenames" << std::endl;
		print_usage(command);
		return 1;
	}

	std::string infile(argv[1]);
	std::string outfile(argv[2]);

	std::ifstream is(infile, std::ios::in);
	std::ofstream os(outfile, std::ios::out);

	char c;
	while (is.get(c)) {
		switch (c) {
			case '\n':
			case ' ':
				break;
			default:
				os << c;
		}
	}

	is.close();
	os.close();

	return 0;
}
