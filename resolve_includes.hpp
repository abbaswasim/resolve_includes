#include <cassert>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

void load_file(std::filesystem::path a_absolute_path, std::vector<unsigned char> &a_data)
{
	std::ios_base::openmode mode = std::ios::ate | std::ios::in;

	std::ifstream as_file(a_absolute_path, mode);
	if (!as_file.is_open())
	{
		printf("Can't open file, it probably doesn't exist %s", a_absolute_path.c_str());
		return;
	}

	// No point to synchronise here because some other process might be writing into the file
	std::streampos bytes_count = as_file.tellg();
	as_file.seekg(0, std::ios::beg);

	if (bytes_count <= 0)
	{
		printf("Error! reading file size, it seems to be empty %s", a_absolute_path.c_str());
		return;
	}

	// Cast is ok because if byte_count is bigger than size_t range, we have a bigger problem
	a_data.resize(static_cast<size_t>(bytes_count));                           // std::streampos has "operator long log" so this works fine, here I am just making it unsigned
	as_file.read(reinterpret_cast<char *>(a_data.data()), bytes_count);        // Weird that int8_t is 'signed char' and can't be converted to 'char'

	as_file.close();
}

auto resolve_includes(std::string &a_data, uint32_t &a_level, bool a_with_markers)
{
	if (a_level == 10)
	{
		assert(0 && "too many levels, there seems to be a recursion");
	}

	a_level++;
	bool result{false};

	std::string out{a_data};        // Lets make a copy that we will work on, required otherwise includes_begin gets invalidated

	static const std::regex include_regex("#include *(<|\")[\\S]*\\.[\\S]*(>|\")[\\s]*\n", std::regex::ECMAScript | std::regex::icase);
	static const std::regex filename_regex("(<|\")[\\S]*(>|\")", std::regex::ECMAScript | std::regex::icase);

	auto includes_begin = std::sregex_iterator(a_data.begin(), a_data.end(), include_regex);
	auto includes_end   = std::sregex_iterator();

	for (auto i = includes_begin; i != includes_end; ++i)
	{
		result = true;

		std::smatch filename_matches;
		std::smatch match     = *i;
		std::string match_str = match.str();
		if (std::regex_search(match_str, filename_matches, filename_regex))
		{
			// The first filename_match is the whole string; next is the first parenthesized expression.
			assert(filename_matches.size() == 3 && "Include file name is not in expected format");
			std::string file_name{filename_matches[0].str()};
			file_name = file_name.substr(1, file_name.length() - 2);

			std::vector<unsigned char> include_data{};
			load_file(file_name, include_data);

			std::string include_data_string{include_data.begin(), include_data.end()};

			if (a_with_markers)
				include_data_string = "// start " + file_name + "\n" + include_data_string + "// end " + file_name + "\n\n";
			else
				include_data_string += "\n";

			out = std::regex_replace(out, include_regex, include_data_string, std::regex_constants::match_flag_type::format_first_only);        // this means the found item surrounded with [] "[$&]"
		}
		else
		{
			assert(0 && "Invalid include in the input file");        // Shouldn't reach here
		}
	}

	a_data = out;

	return result;
}

void resolve_includes(std::string &a_input, bool a_with_markers)
{
	uint32_t level  = 0;
	bool     result = false;
	do
	{
		result = resolve_includes(a_input, level, a_with_markers);
	} while (result && level < 10);
}
