#include "Decoder.h"
#include <string>
#include <utility>
#include <math.h> 

std::unordered_map<uint32_t, SymbolIndexMappingMessage> Decoder::symbolMap;
std::unordered_map<std::string, std::vector<TimestampedTrades>> Decoder::trackedSecurities;

void Decoder::popuateSymbolIndexMap(const SymbolIndexMappingMessage& message)
{
	symbolMap.insert(std::pair<uint32_t, SymbolIndexMappingMessage>(message.symbolIndex, message));
}


void Decoder::updateOrderBook(const TradeMessage& trade)
{
	SymbolIndexMappingMessage indexMapping = symbolMap.at(trade.symbolIndex);
	
	std::string securityName = std::string(indexMapping.symbol);

	if (trackedSecurities.find(securityName) == trackedSecurities.end())
	{
		trackedSecurities.insert(std::make_pair(securityName, std::vector<TimestampedTrades>()));
	}

	Timestamp timestamp = { trade.sourceTime, trade.sourceTimeNs };
	TimestampedTrades timedTrade = { timestamp, (double(trade.price) / pow(10, indexMapping.priceScaleCode)) };

	trackedSecurities.at(securityName).emplace_back(timedTrade);
}

void Decoder::clearSymbolIndexMap()
{

}

void Decoder::decodePacket(char packetData[], const int packetSize)
{
	XdpPacketHeader xdpPacketHeader;
	memcpy(&xdpPacketHeader, packetData, sizeof(XdpPacketHeader));
	packetData += sizeof(XdpPacketHeader);
	
	XdpMessageHeader xdpMessageHeader;
	for (int ii = 0; ii < xdpPacketHeader.numberMsgs; ++ii)
	{
		memcpy(&xdpMessageHeader, packetData, sizeof(XdpMessageHeader));
		packetData += sizeof(XdpMessageHeader);

		switch (xdpMessageHeader.msgType)
		{
		case MessageType::TRADE:
			TradeMessage tMessage;
			memcpy(&tMessage, packetData, sizeof(TradeMessage));
			updateOrderBook(tMessage);
			break;

		case MessageType::SEQUENCE_NUMBER_RESET:
			SequenceNumberResetMessage snrMessage;
			memcpy(&snrMessage, packetData, sizeof(SequenceNumberResetMessage));
			break;

		case MessageType::SYMBOL_INDEX_MAPPING:
			SymbolIndexMappingMessage simMessage;
			memcpy(&simMessage, packetData, sizeof(SymbolIndexMappingMessage));
			popuateSymbolIndexMap(simMessage);
			break;

		case MessageType::SYMBOL_CLEAR:
			SymbolClearMessage scMessage;
			memcpy(&scMessage, packetData, sizeof(SymbolClearMessage));
			clearSymbolIndexMap();
			break;

		case MessageType::SECURITY_STATUS:
			SecurityStatusMessage ssMessage;
			memcpy(&ssMessage, packetData, sizeof(SecurityStatusMessage));
			break;

		default:
			throw;
		}

		packetData += (xdpMessageHeader.msgSize - sizeof(XdpMessageHeader));
	}
	
	return;
}