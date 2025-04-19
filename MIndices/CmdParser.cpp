#include "CmdParser.h"
#include <iostream>

using namespace CMDParser;

CmdParser::CmdParser()
{
	desc = std::make_unique<po::options_description>("Options");
	desc->add_options()
		("help,h", "Description")
		("file,f", po::value<std::string>(&filename), "Input Filename")
		("threads,t", po::value<int32_t>()->default_value(static_cast<int32_t>(MIndices::ParallelThreads::off), "Enable Parallel Processing"));
}

bool CmdParser::ParseArguements(int32_t argc, char* argv[])
{
	try
	{
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, *desc), vm);
		po::notify(vm);

		if (vm.count("help"))
		{
			// Exit when help was requested
			std::cout << *desc << std::endl;
			return false;
		}
		if (!vm.count("file"))
		{
			std::cerr << "Error: missing required arguement --file" << std::endl;
			std::cout << *desc << std::endl;
			return false;
		}
		if (vm.count("threads"))
		{
			auto val = vm["threads"].as<int32_t>();
			if (!isValidRange(val))
			{
				std::cerr << "Warning: Invalid value for parallel processing." << std::endl;
			}
			else
			{
				threads = std::make_optional<MIndices::ParallelThreads>(static_cast<MIndices::ParallelThreads>(val));
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return false;
	}
	return true;
}

const std::string& CmdParser::Filename() const
{
	return filename;
}

const MIndices::ParallelThreads CmdParser::Threads() const noexcept
{
	return threads.value_or(MIndices::ParallelThreads::off);
}
