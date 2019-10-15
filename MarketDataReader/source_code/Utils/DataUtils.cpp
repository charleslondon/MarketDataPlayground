#include "DataUtils.h"

void DataUtils::swap16Endian(uint16_t& value) {
	value = (value >> 8) | (value << 8);
}

void DataUtils::swap64Endian(uint64_t& value) {
	value = ((value & 0x00000000FFFFFFFFull) << 32) | ((value & 0xFFFFFFFF00000000ull) >> 32);
	value = ((value & 0x0000FFFF0000FFFFull) << 16) | ((value & 0xFFFF0000FFFF0000ull) >> 16);
	value = ((value & 0x00FF00FF00FF00FFull) << 8) | ((value & 0xFF00FF00FF00FF00ull) >> 8);
}

std::ios::pos_type DataUtils::determineFileLength(std::ifstream& fin)
{
	fin.seekg(0, std::ios::end);
	std::ios::pos_type length = fin.tellg();
	fin.seekg(0, std::ios::beg);

	return length;
}

std::string DataUtils::trimmedString(const char* input)
{
	int stopper = 0;
	while (stopper < 5)
	{
		if (input[stopper] == ' ')
		{
			break;
		}
		++stopper;
	}

	return std::string(input, stopper);
}

