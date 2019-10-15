#pragma once
#include <unordered_map>
#include "Messages.h"

struct Timestamp
{
	uint32_t secondsSinceEpoch;
	uint32_t currentNanoSeconds;
};

struct TimestampedTrades
{
	Timestamp time;
	double price;
};

class Decoder
{
private:
	/*Symbol index to Symbol IndexMessage*/
	static std::unordered_map<uint32_t, SymbolIndexMappingMessage> symbolMap;
	
	/*The stock mapped to a list of timestamp/prices */
	static std::unordered_map<std::string, std::vector<TimestampedTrades>> trackedSecurities;

public:
	static void decodePacket(char packetData[], const int packetSize);
	static void popuateSymbolIndexMap(const SymbolIndexMappingMessage& message);
	static void clearSymbolIndexMap();
	static void updateOrderBook(const TradeMessage& trade);
};
