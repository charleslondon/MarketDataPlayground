#include "Decoder.h"
#include <iostream>

Decoder::Decoder() :
	symbolMap(std::unordered_map<uint32_t, SymbolIndexMappingMessage>()),
	mqConnection(AmqpClient::Channel::Create())
{
	populateMessageMap();
}

void Decoder::populateMessageMap()
{
	messageHandlerMap.emplace(MessageType::TRADE, [=](char packetData[]) { this->handleTrade(packetData); });
	messageHandlerMap.emplace(MessageType::SEQUENCE_NUMBER_RESET, [=](char packetData[]) { this->handleTradeSequenceNumberReset(packetData); });
	messageHandlerMap.emplace(MessageType::SYMBOL_INDEX_MAPPING, [=](char packetData[]) { this->handleSymbolIndexMapping(packetData); });
	messageHandlerMap.emplace(MessageType::SYMBOL_CLEAR, [=](char packetData[]) { this->handleSymbolClear(packetData); });
	messageHandlerMap.emplace(MessageType::SECURITY_STATUS, [=](char packetData[]) { this->handleSecurityStatus(packetData); });
	messageHandlerMap.emplace(MessageType::SOURCE_TIME_REFERENCE, [=](char packetData[]) { this->handleSourceTimeReference(packetData); });
	messageHandlerMap.emplace(MessageType::QUOTE, [=](char packetData[]) { this->handleQuote(packetData); });
	messageHandlerMap.emplace(MessageType::ADD_ORDER, [=](char packetData[]) { this->handleAddOrder(packetData); });
	messageHandlerMap.emplace(MessageType::MODIFY_ORDER, [=](char packetData[]) { this->handleModifyOrder(packetData); });
	messageHandlerMap.emplace(MessageType::DELETE_ORDER, [=](char packetData[]) { this->handleDeleteOrder(packetData); });
	messageHandlerMap.emplace(MessageType::REPLACE_ORDER, [=](char packetData[]) { this->handleReplaceOrder(packetData); });
	messageHandlerMap.emplace(MessageType::TRADE_CANCEL, [=](char packetData[]) { this->handleTradeCancel(packetData); });
	messageHandlerMap.emplace(MessageType::ORDER_EXECUTION, [=](char packetData[]) { this->handleOrderExecution(packetData); });
	messageHandlerMap.emplace(MessageType::IMBALANCE, [=](char packetData[]) { this->handleImbalance(packetData); });
	messageHandlerMap.emplace(MessageType::ADD_ORDER_REFRESH, [=](char packetData[]) { this->handleAddOrderRefresh(packetData); });
	messageHandlerMap.emplace(MessageType::CROSS_TRADE, [=](char packetData[]) { this->handleCrossTrade(packetData); });
	messageHandlerMap.emplace(MessageType::NON_DISPLAYED_TRADE, [=](char packetData[]) { this->handleNonDisplayedTrade(packetData); });
	messageHandlerMap.emplace(MessageType::CROSS_CORRECTION, [=](char packetData[]) { this->handleCrossCorrection(packetData); });
	messageHandlerMap.emplace(MessageType::RETAIL_IMPROVEMENT, [=](char packetData[]) { this->handleRetailImprovement(packetData); });
}

void Decoder::handleTrade(char packetData[])
{
	TradeMessage message;
	memcpy(&message, packetData, sizeof(TradeMessage));
	updateOrderBook(message);
}

void Decoder::handleTradeSequenceNumberReset(char packetData[])
{
	SequenceNumberResetMessage message;
	memcpy(&message, packetData, sizeof(SequenceNumberResetMessage));
}

void Decoder::handleSymbolIndexMapping(char packetData[])
{
	SymbolIndexMappingMessage message;
	memcpy(&message, packetData, sizeof(SymbolIndexMappingMessage));
	popuateSymbolIndexMap(message);
}

void Decoder::handleSymbolClear(char packetData[])
{
	SymbolClearMessage message;
	memcpy(&message, packetData, sizeof(SymbolClearMessage));
	clearSymbolIndexMap();
}

void Decoder::handleSecurityStatus(char packetData[])
{
	SecurityStatusMessage message;
	memcpy(&message, packetData, sizeof(SecurityStatusMessage));
}

void Decoder::handleSourceTimeReference(char packetData[])
{
	SourceTimeReferenceMessage message;
	memcpy(&message, packetData, sizeof(SourceTimeReferenceMessage));
}

void Decoder::handleQuote(char packetData[])
{
	QuoteMessage message;
	memcpy(&message, packetData, sizeof(QuoteMessage));
}

void Decoder::handleAddOrder(char packetData[])
{
	AddOrderMessage message;
	memcpy(&message, packetData, sizeof(AddOrderMessage));
}

void Decoder::handleModifyOrder(char packetData[])
{
	ModifyOrderMessage message;
	memcpy(&message, packetData, sizeof(ModifyOrderMessage));
}

void Decoder::handleDeleteOrder(char packetData[])
{
	DeleteOrderMessage message;
	memcpy(&message, packetData, sizeof(DeleteOrderMessage));
}

void Decoder::handleReplaceOrder(char packetData[])
{
	ReplaceOrderMessage message;
	memcpy(&message, packetData, sizeof(ReplaceOrderMessage));
}

void Decoder::handleTradeCancel(char packetData[])
{
	TradeCancelMessage message;
	memcpy(&message, packetData, sizeof(TradeCancelMessage));
}

void Decoder::handleOrderExecution(char packetData[])
{
	OrderExecutionMessage message;
	memcpy(&message, packetData, sizeof(OrderExecutionMessage));
}

void Decoder::handleImbalance(char packetData[])
{
	ImbalanceMessage message;
	memcpy(&message, packetData, sizeof(ImbalanceMessage));
}

void Decoder::handleAddOrderRefresh(char packetData[])
{
	AddOrderRefreshMessage message;
	memcpy(&message, packetData, sizeof(AddOrderRefreshMessage));
}

void Decoder::handleCrossTrade(char packetData[])
{
	CrossTradeMessage message;
	memcpy(&message, packetData, sizeof(CrossTradeMessage));
}

void Decoder::handleNonDisplayedTrade(char packetData[])
{
	NonDisplayedTradeMessage message;
	memcpy(&message, packetData, sizeof(NonDisplayedTradeMessage));
}

void Decoder::handleCrossCorrection(char packetData[])
{
	CrossCorrectionMessage message;
	memcpy(&message, packetData, sizeof(CrossCorrectionMessage));
}

void Decoder::handleRetailImprovement(char packetData[])
{
	RetailImprovementMessage message;
	memcpy(&message, packetData, sizeof(RetailImprovementMessage));
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
	auto search = messageHandlerMap.find(msgType);
	if(search != messageHandlerMap.end())
	{
		search->second(packetData);
	}
	else
	{ 
		bool wasInserted = unhandledMessages.insert(msgType).second;

		if (wasInserted)
		{
			std::cout << "Unhandled Message Encountered: Message_Type " << static_cast<int>(msgType) << std::endl;
		}
	}
}
