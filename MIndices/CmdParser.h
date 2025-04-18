#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <optional>
#include <memory>

#include "Structures.h"

namespace po = boost::program_options;

namespace CMDParser
{
	inline bool isValidRange(int32_t val)
	{
		return val < static_cast<int32_t>(MIndices::ParallelThreads::count)
			&& static_cast<int32_t>(MIndices::ParallelThreads::off) <= val;
	}

	class CmdParser
	{
	public:
		CmdParser();
		~CmdParser() = default;
		bool ParseArguements(int32_t argc, char* argv[]);
		template<typename T>
		T ParseInput(std::string&& prompt);
		const std::string& Filename() const;
		const MIndices::ParallelThreads Threads() const noexcept;

	private:
		std::unique_ptr<po::options_description> desc;
		std::string filename;
		std::optional<MIndices::ParallelThreads> threads;
	};

	template<typename T>
	T CmdParser::ParseInput(std::string&& prompt)
	{
		static_assert(std::is_arithmetic<T>::value, "Parse arithmetic inputs only");

		T value;
		bool isInvalid = true;

		while (isInvalid)
		{
			std::cout << prompt;
			std::cin >> value;

			if (std::cin.fail() || value <= 0)
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Error: please enter a positive number." << std::endl;
			}
			else
			{
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				isInvalid = false;
			}
		}
		return value;
	}
}

