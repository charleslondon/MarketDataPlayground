#include <string>
#include <fstream>
#include "Model/DataStructs.h"
#include "Utils/DataUtils.h"
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

static const char TRADE_PAYLOAD = 'T';
static const unsigned int PAYLOAD_HEADER_SIZE = sizeof(PayloadHeader);
static const unsigned int TRADE_SIZE = sizeof(Trade);
/*static const unsigned int QUOTE_SIZE = sizeof(Quote);*/

void decodePayload(std::ifstream& fin, FILE* fptr)
{
	PayloadHeader payloadHeader;
	fin.read((char*)&payloadHeader, sizeof(payloadHeader));

	std::streamoff skip = ntohs(payloadHeader.length) - PAYLOAD_HEADER_SIZE;

	if (payloadHeader.type == TRADE_PAYLOAD)
	{
		Trade trade;
		fin.read((char*)&trade, sizeof(trade));

		fprintf(fptr, "%d %s @ %.2f \n",
			(ntohs(trade.tradeSize) * 100),
			&DataUtils::trimmedString(payloadHeader.symbol)[0],
			double(ntohll(trade.tradePrice)) / 10000);

		skip -= TRADE_SIZE;
	}

	fin.ignore(skip);
}

void decodeFile(const std::string& inputFileName, const std::string& outputFileName)
{
	FILE* fptr;
	fopen_s(&fptr, outputFileName.c_str(), "w");
	std::ifstream fin(inputFileName.c_str(), std::ios::binary);

	PacketHeader packetHeader;
	while (!fin.eof())
	{
		fin.read((char*)&packetHeader, sizeof(packetHeader));

		for (int ii = 0; ii < ntohs(packetHeader.numUpdates); ++ii)
		{
			decodePayload(fin, fptr);
		}
	}

	fin.close();
	fclose(fptr);
}

int main()
{
	decodeFile(
		"C:/Users/charl/Documents/PROJECTS/GTS Code Assignment/input.dat",
		"C:/Users/charl/Documents/PROJECTS/GTS Code Assignment/output.txt");

	return 0;
}
