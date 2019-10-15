#pragma once
#include <fstream>
#include <stdint.h>

class DataUtils
{
public:
	static void swap16Endian(uint16_t& value);
	static void swap64Endian(uint64_t& value);
	static std::ios::pos_type determineFileLength(std::ifstream& fin);
	static std::string trimmedString(const char* array);
};
