#pragma once
#include <stdint.h>

#pragma pack(push, 1)
struct PacketHeader
{
	uint16_t length; //Length of the entire packet (including this data member).
	uint16_t numUpdates; //Number of market updates in this packet.
};

struct PayloadHeader
{
	uint16_t length; //Length of the entire market update (Including this data member).
	unsigned char type; // Either a "Q" or a "T" to describe the incoming data chunk
	char symbol[5]; //Left-justified string with spaces filling the unused bytes.
};

struct Trade
{
	uint16_t tradeSize; //Volume traded (given in round lots; 100's)
	uint64_t tradePrice; //Price of traded volume
	//char* dynamicData; //Reserved data; can be ignored.
};

/*
struct Quote
{
	uint16_t priceLevel; //Zero indexed price level to apply this update
	uint16_t priceLevelSize; //New size of price level
	uint64_t priceLevelPrice; //New price of price level
	//char* dynamicData; //Reserved data; can be ignored.
};
*/
#pragma pack(pop)