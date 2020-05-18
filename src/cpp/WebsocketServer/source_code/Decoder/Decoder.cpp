#include "Decoder.h"
#include <string>
#include <utility>
#include <math.h>

Decoder::Decoder() :
	symbolMap(std::unordered_map<uint32_t, SymbolIndexMappingMessage>()),
	mqConnection(AmqpClient::Channel::Create())
{
}

void Decoder::popuateSymbolIndexMap(const SymbolIndexMappingMessage& message)
{
	symbolMap.insert(std::pair<uint32_t, SymbolIndexMappingMessage>(message.symbolIndex, message));
}

void Decoder::updateOrderBook(const TradeMessage& trade)
{
	SymbolIndexMappingMessage mapping = symbolMap.at(trade.symbolIndex);

	auto message = SymbolMessage(
		mapping.symbol,
		trade.price / pow(10, mapping.priceScaleCode),
		trade.sourceTime,
		trade.sourceTimeNs);

	mqConnection->BasicPublish(
		"TEST",		/*The Default Exchange*/
		"key",	/*Using the stock ticker as our routing key*/
		AmqpClient::BasicMessage::Create(message.serialize()));
}

void Decoder::clearSymbolIndexMap()
{
	//symbolMap.clear();
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