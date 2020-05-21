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
		handleMessage(xdpMessageHeader.msgType, packetData);
		packetData += (xdpMessageHeader.msgSize - sizeof(XdpMessageHeader));
	}
}

void Decoder::handleMessage(MessageType msgType, char packetData[])
{
	switch (msgType)
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

	case MessageType::SOURCE_TIME_REFERENCE:
		SourceTimeReferenceMessage strMessage;
		memcpy(&strMessage, packetData, sizeof(SourceTimeReferenceMessage));
		break;

	case MessageType::QUOTE:
		QuoteMessage qMessage;
		memcpy(&qMessage, packetData, sizeof(QuoteMessage));
		break;

	case MessageType::ADD_ORDER:
		AddOrderMessage aoMessage;
		memcpy(&aoMessage, packetData, sizeof(AddOrderMessage));
		break;

	case MessageType::MODIFY_ORDER:
		ModifyOrderMessage moMessage;
		memcpy(&moMessage, packetData, sizeof(ModifyOrderMessage));
		break;

	case MessageType::DELETE_ORDER:
		DeleteOrderMessage doMessage;
		memcpy(&doMessage, packetData, sizeof(DeleteOrderMessage));
		break;

	case MessageType::REPLACE_ORDER:
		ReplaceOrderMessage roMessage;
		memcpy(&roMessage, packetData, sizeof(ReplaceOrderMessage));
		break;

	case MessageType::ORDER_EXECUTION:
		OrderExecutionMessage oeMessage;
		memcpy(&oeMessage, packetData, sizeof(OrderExecutionMessage));
		break;

	case MessageType::IMBALANCE:
		ImbalanceMessage iMessage;
		memcpy(&iMessage, packetData, sizeof(ImbalanceMessage));
		break;
		
	case MessageType::ADD_ORDER_REFRESH:
		AddOrderRefreshMessage aorMessage;
		memcpy(&aorMessage, packetData, sizeof(AddOrderRefreshMessage));
		break;

	case MessageType::CROSS_TRADE:
		CrossTradeMessage ctMessage;
		memcpy(&ctMessage, packetData, sizeof(CrossTradeMessage));
		break;

	case MessageType::NON_DISPLAYED_TRADE:
		NonDisplayedTradeMessage ndtMessage;
		memcpy(&ndtMessage, packetData, sizeof(NonDisplayedTradeMessage));
		break;

	case MessageType::CROSS_CORRECTION:
		CrossCorrectionMessage ccMessage;
		memcpy(&ccMessage, packetData, sizeof(CrossCorrectionMessage));
		break;

	case MessageType::TRADE_CANCEL:
		TradeCancelMessage tcMessage;
		memcpy(&tcMessage, packetData, sizeof(TradeCancelMessage));
		break;

	case MessageType::RETAIL_IMPROVEMENT:
		RetailImprovementMessage riMessage;
		memcpy(&riMessage, packetData, sizeof(RetailImprovementMessage));
		break;

	default:
		throw;
	}
}
