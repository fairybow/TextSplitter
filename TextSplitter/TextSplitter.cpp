#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace Ts
{
	enum class TrimPunctuation
	{
		No,
		Yes
	};

	namespace
	{
		constexpr auto EXT = ".csv";
		constexpr auto TRIMMABLE = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

		std::filesystem::path _csvPath(const std::filesystem::path& path)
		{
			static const std::string extension = EXT;

			return std::filesystem::path(path).replace_extension(extension);
		}

		std::string _trimPunctuation(const std::string& word)
		{
			static const std::string punctuation = TRIMMABLE;
			std::string result{};

			std::copy_if
			(
				word.begin(),
				word.end(),
				std::back_inserter(result),
				[&](int c) { return punctuation.find(c) == std::string::npos; }
			);

			return result;
		}

		void _write
		(
			std::ifstream& inFile,
			std::ofstream& outFile,
			Ts::TrimPunctuation trimPunctuation
		)
		{
			std::string buffer{};

			while (std::getline(inFile, buffer))
			{
				std::istringstream stream(buffer);

				std::string word{};

				while (stream >> word)
				{
					if (trimPunctuation == Ts::TrimPunctuation::Yes)
						word = _trimPunctuation(word);

					outFile << word << '\n';
				}
			}
		}
	}

	void write
	(
		const std::filesystem::path& path,
		TrimPunctuation trimPunctuation = TrimPunctuation::Yes
	)
	{
		std::ifstream in_file(path);
		if (!in_file.is_open()) return;

		std::ofstream out_file(_csvPath(path));

		_write(in_file, out_file, trimPunctuation);

		out_file.close();
		in_file.close();
	}
}

int main(int argc, char* argv[])
{
	for (auto i = 1; i < argc; ++i)
		Ts::write(argv[i]);
}
