#include "resolve_includes.hpp"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	std::vector<unsigned char> data{};

	std::filesystem::path      in{"main.frag"};
	std::filesystem::path      result{"result.frag"};

	load_file(in, data);
	std::string input{data.begin(), data.end()};

	resolve_includes(input, false);

	load_file(result, data);
	std::string res{data.begin(), data.end()};

	if (input.compare(res) == 0 && res.length() == input.length())
		std::cout << "TEST PASSED" << std::endl;
	else
	{
		std::cout << "Here are size " << res.length() << "!=" << input.length();

		for (size_t i = 0; i < std::min(input.length(), res.length()); ++i)
		{
			if (res[i] != input[i])
				std::cout << i << "th mismatch" << res[i] << ":" << std::endl;
			else
				std::cout << res[i] << ":" << input[i] << std::endl;
		}

		std::cout << "Stitched file is:\n" << std::endl;
		std::cout << input << std::endl;
		
		std::cout << "Expected output is:\n" << std::endl;
		std::cout << res << std::endl;
		
		std::cout << "TEST FAILED" << std::endl;
	}

	return 0;
}
