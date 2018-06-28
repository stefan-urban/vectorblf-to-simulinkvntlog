
#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "blfreader.h"
#include "vntlogwriter.h"

namespace bpo = boost::program_options;


#define VERSION_STRING "Vektor BLF to Simulink VNT CAN Logfile Converter 1.0.0.0"
#define COPYRIGHT_STRING "(C) 2018 Stefan Urban <stefan.urban@live.de>"


void print_title_copy()
{
	std::cout << VERSION_STRING << std::endl;
	std::cout << COPYRIGHT_STRING << std::endl;
}

void print_help()
{
	print_title_copy();
	std::cout << std::endl;
	std::cout << "Usage: vblf2slvnt.exe [options] <input-file.blf> <output-file.mat>" << std::endl;
	std::cout << std::endl;
	std::cout << "Examples:" << std::endl;
	std::cout << std::endl;
	std::cout << "  - Use quotation marks if your paths contain spaces:" << std::endl;
	std::cout << "    vblf2slvnt.exe \"C:\\log files\\courseA.blf\" \"C:\\mat files\\courseA.mat\"" << std::endl;
	std::cout << std::endl;
	std::cout << "  - Create MAT only with data from 10s to 20s:" << std::endl;
	std::cout << "    vblf2slvnt.exe -f 10.0 -t 20.0 courseA.blf courseA.mat" << std::endl;
	std::cout << std::endl;
	std::cout << "  - Create 20s long MAT file starting at 30s but skip empty time at beginning:" << std::endl;
	std::cout << "    vblf2slvnt.exe -f 30.0 -d 10.0 -z courseA.blf courseA.mat" << std::endl;
	std::cout << std::endl;
}

bpo::variables_map handle_opts(int argc, char **argv)
{
	bpo::variables_map opts;

	// Define command line arguments
	bpo::options_description opt_generic("Common options");
	opt_generic.add_options()
		("version,v", "print version string")
		("help", "produce help message")
		("input,i", bpo::value<std::string>()->required(), "input file in Vektor BLF format")
		("output,o", bpo::value<std::string>()->required(), "output file in MATLAB/Simulink VNT CAN log format")
		;

	bpo::options_description opt_timeframe("Select a timeframe inside the BLF file using");
	opt_timeframe.add_options()
		("fromtime,f", bpo::value<double>(), "[secs] skip input file up to this time mark")
		("totime,t", bpo::value<double>(), "[secs] stop conversion at this time mark")
		("duration,d", bpo::value<double>(), "[secs] stop conversion after this amount of time")
		;

	bpo::options_description opt_timebase("Choose how much the timebase should be moved");
	opt_timebase.add_options()
		("moveby,m", bpo::value<double>(), "[secs] move the timestamps by this amount of seconds")
		("tozero,z", bpo::value<bool>(), "[0 or 1] shift timestamp vector so that the first CAN frame starts at 0")
		;

	bpo::options_description visible_options("Allowed options");
	visible_options.add(opt_generic).add(opt_timeframe).add(opt_timebase);

	bpo::positional_options_description opt_posi;
	opt_posi.add("input", 1);
	opt_posi.add("output", 1);

	try
	{
		bpo::parsed_options parsed = bpo::command_line_parser(argc, argv)
			.options(visible_options)
			.positional(opt_posi)
			.run();

		bpo::store(parsed, opts);

		// Handle options that inhibit further execution immediately
		if (opts.count("help")) {
			print_help();
			std::cout << opt_generic << std::endl;
			std::cout << opt_timeframe << std::endl;
			std::cout << opt_timebase;
			return nullptr;
		}

		if (opts.count("version")) {
			print_title_copy();
			return nullptr;
		}

		// Evaluate (required) fields
		bpo::notify(opts);

		if (0 == opts["input"].as<std::string>().compare(opts["output"].as<std::string>()))
		{
			throw bpo::error("input and output file can not be the same");
		}
	}
	catch (bpo::error& e)
	{
		std::cout << "Argument Error: " << e.what();
		return nullptr;
	}

	return opts;
}

int main(int argc, char **argv)
{
	auto opts = handle_opts(argc, argv);

	auto can_data = open_blf_file(opts["input"].as<std::string>());

	write_vnt_mat(opts["output"].as<std::string>(), can_data, "can_data");

	return 0;
}
