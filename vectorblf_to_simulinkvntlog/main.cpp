
#include <iostream>
#include <limits>
#include <string>
#include <boost/program_options.hpp>

#include "blfreader.h"
#include "vntlogwriter.h"

namespace bpo = boost::program_options;


#define VERSION_STRING "Vektor BLF to Simulink VNT CAN Logfile Converter"
#define BUILD_STRING "Version  1.0.0.2 - Build " __DATE__ " " __TIME__
#define COPYRIGHT_STRING "(C) 2018 Stefan Urban <stefan.urban@live.de>"


void print_title_copy()
{
	std::cout << VERSION_STRING << std::endl;
	std::cout << COPYRIGHT_STRING << std::endl;
	std::cout << BUILD_STRING << std::endl;
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

bool handle_opts(int argc, char **argv, bpo::variables_map *opts)
{
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
		("fromtime,f", bpo::value<double>()->default_value(0.0), "[secs] skip input file up to this time mark")
		("endtime,e", bpo::value<double>()->default_value(std::numeric_limits<double>::max()), "[secs] stop conversion at this time mark")
		("duration,d", bpo::value<double>(), "[secs] stop conversion after this amount of time (has priority over endtime)")
		;

	bpo::options_description opt_timebase("Choose how much the timebase should be moved (applied after the time section cut above)");
	opt_timebase.add_options()
		("moveby,m", bpo::value<double>()->default_value(0.0), "[secs] move the timestamps by this amount of seconds")
		("movetozero,z", bpo::value<bool>(), "[0 or 1] shift timestamp vector so that the first CAN frame starts at 0")
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

		bpo::store(parsed, *opts);

		// Handle options that inhibit further execution immediately
		if (opts->count("help")) {
			print_help();
			std::cout << opt_generic << std::endl;
			std::cout << opt_timeframe << std::endl;
			std::cout << opt_timebase;
			return false;
		}

		if (opts->count("version")) {
			print_title_copy();
			return false;
		}

		// Evaluate (required) fields
		bpo::notify(*opts);

		if (0 == (*opts)["input"].as<std::string>().compare((*opts)["output"].as<std::string>()))
		{
			std::cout << "input and output file can not be the same" << std::endl;
			return false;
		}
	}
	catch (bpo::error& e)
	{
		std::cout << "argument error: " << e.what();
		return false;
	}

	return true;
}

int main(int argc, char **argv)
{
	try {
		bpo::variables_map opts;

		if (false == handle_opts(argc, argv, &opts))
		{
			return 0;
		}

		// Obtain complete data from BLF file
		can_data_t can_data;
		std::string blf_filename = opts["input"].as<std::string>();

		try
		{
			can_data = open_blf_file(blf_filename);
		}
		catch (blfexception e)
		{
			return -1;
		}

		std::cout << "back in main" << std::endl;

		// Define measurement start and delete unused can frames
		double meas_start = opts["fromtime"].as<double>();
		double meas_end = opts["endtime"].as<double>();

		if (opts.count("duration"))
		{
			meas_end = meas_start + opts["duration"].as<double>();
		}

		while (can_data.Timestamp.size() && can_data.Timestamp.front() < meas_start)
		{
			can_data.Extended.pop_front();
			can_data.Length.pop_front();
			can_data.Remote.pop_front();
			can_data.Error.pop_front();
			can_data.ID.pop_front();
			can_data.Timestamp.pop_front();
			for (size_t i = 0; i < 8; i++) can_data.Data.pop_front();
		}

		while (can_data.Timestamp.size() && can_data.Timestamp.back() > meas_end)
		{
			can_data.Extended.pop_back();
			can_data.Length.pop_back();
			can_data.Remote.pop_back();
			can_data.Error.pop_back();
			can_data.ID.pop_back();
			can_data.Timestamp.pop_back();
			for (size_t i = 0; i < 8; i++) can_data.Data.pop_back();
		}

		// Move timebase
		double movebytime = opts["moveby"].as<double>();

		if (opts.count("movetozero") && true == opts["movetozero"].as<bool>())
		{
			movebytime = -1.0 * can_data.Timestamp.front();
		}

		std::for_each(can_data.Timestamp.begin(), can_data.Timestamp.end(), [movebytime](double& d) { d += movebytime;});

		// Write result into MAT file
		write_vnt_mat(opts["output"].as<std::string>(), can_data, "can_data");

	}
	catch (std::exception e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
	}

	return 0;
}
