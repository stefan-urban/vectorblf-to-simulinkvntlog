#pragma once

#include <string>
#include <vector>


typedef struct {
	std::vector<unsigned char> Extended;
	std::vector<unsigned char> Length;
	std::vector<unsigned char> Remote;
	std::vector<unsigned char> Error;
	std::vector<unsigned int> ID;
	std::vector<double> Timestamp;
	std::vector<unsigned char> Data;
} can_data_t;

class blfexception : std::exception {};

can_data_t open_blf_file(std::string filename);