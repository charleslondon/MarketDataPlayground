#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Model/Messages.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>

struct SymbolMessage
{
	SymbolMessage() = delete;

	SymbolMessage(
		const std::string& newSymbol,
		const double newPrice,
		const uint32_t newSourceTime,
		const uint32_t newSourceTimeNs) :
		symbol(newSymbol),
		price(std::to_string(newPrice)),
		time(std::to_string(newSourceTime)
			.append(".")
			.append(std::to_string(newSourceTimeNs))) {}

	std::string serialize() const
	{
		return
			"Symbol:" + symbol +
			",Price:" + price +
			",Time:" + time;
	}

	const std::string symbol;
	const std::string price;
	const std::string time;
};

class Decoder
{
private:
	/*Symbol index to Symbol IndexMessage*/
	std::unordered_map<uint32_t, SymbolIndexMappingMessage> symbolMap;
	AmqpClient::Channel::ptr_t mqConnection;

public:
	Decoder();
	void decodePacket(char packetData[], const int packetSize);
	void popuateSymbolIndexMap(const SymbolIndexMappingMessage& message);
	void clearSymbolIndexMap();
	void updateOrderBook(const TradeMessage& trade);
};
