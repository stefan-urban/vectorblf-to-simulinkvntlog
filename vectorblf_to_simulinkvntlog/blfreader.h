#pragma once


#include <deque>
#include <string>
#include <vector>


typedef struct {
	std::deque<unsigned char> Extended;
	std::deque<unsigned char> Length;
	std::deque<unsigned char> Remote;
	std::deque<unsigned char> Error;
	std::deque<unsigned int> ID;
	std::deque<double> Timestamp;
	std::deque<unsigned char> Data;
} can_data_t;

class blfexception : std::exception {};

can_data_t open_blf_file(std::string filename);
