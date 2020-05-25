#include "Decoder.h"
#include <iostream>

Decoder::Decoder() :
	symbolMap(std::unordered_map<uint32_t, std::shared_ptr<Message::SymbolIndexMapping>>()),
	mqConnection(AmqpClient::Channel::Create())
{
	populateMessageHandlerMap();
}

void Decoder::populateMessageHandlerMap()
{
	msgHandlerMap.emplace(MessageType::TRADE,					[=](char packetData[]) { this->handleTrade(packetData); });
	msgHandlerMap.emplace(MessageType::SEQUENCE_NUMBER_RESET,	[=](char packetData[]) { this->handleSequenceNumberReset(packetData); });
	msgHandlerMap.emplace(MessageType::SYMBOL_INDEX_MAPPING,	[=](char packetData[]) { this->handleSymbolIndexMapping(packetData); });
	msgHandlerMap.emplace(MessageType::SYMBOL_CLEAR,			[=](char packetData[]) { this->handleSymbolClear(packetData); });
	msgHandlerMap.emplace(MessageType::SECURITY_STATUS,			[=](char packetData[]) { this->handleSecurityStatus(packetData); });
	msgHandlerMap.emplace(MessageType::SOURCE_TIME_REFERENCE,	[=](char packetData[]) { this->handleSourceTimeReference(packetData); });
	msgHandlerMap.emplace(MessageType::QUOTE,					[=](char packetData[]) { this->handleQuote(packetData); });
	msgHandlerMap.emplace(MessageType::ADD_ORDER,				[=](char packetData[]) { this->handleAddOrder(packetData); });
	msgHandlerMap.emplace(MessageType::MODIFY_ORDER,			[=](char packetData[]) { this->handleModifyOrder(packetData); });
	msgHandlerMap.emplace(MessageType::DELETE_ORDER,			[=](char packetData[]) { this->handleDeleteOrder(packetData); });
	msgHandlerMap.emplace(MessageType::REPLACE_ORDER,			[=](char packetData[]) { this->handleReplaceOrder(packetData); });
	msgHandlerMap.emplace(MessageType::TRADE_CANCEL,			[=](char packetData[]) { this->handleTradeCancel(packetData); });
	msgHandlerMap.emplace(MessageType::ORDER_EXECUTION,			[=](char packetData[]) { this->handleOrderExecution(packetData); });
	msgHandlerMap.emplace(MessageType::IMBALANCE,				[=](char packetData[]) { this->handleImbalance(packetData); });
	msgHandlerMap.emplace(MessageType::ADD_ORDER_REFRESH,		[=](char packetData[]) { this->handleAddOrderRefresh(packetData); });
	msgHandlerMap.emplace(MessageType::CROSS_TRADE,				[=](char packetData[]) { this->handleCrossTrade(packetData); });
	msgHandlerMap.emplace(MessageType::NON_DISPLAYED_TRADE,		[=](char packetData[]) { this->handleNonDisplayedTrade(packetData); });
	msgHandlerMap.emplace(MessageType::CROSS_CORRECTION,		[=](char packetData[]) { this->handleCrossCorrection(packetData); });
	msgHandlerMap.emplace(MessageType::RETAIL_IMPROVEMENT,		[=](char packetData[]) { this->handleRetailImprovement(packetData); });
}

void Decoder::handleTrade(char packetData[]) const
{
	auto trade = loadMessage<Message::Trade>(packetData);
	auto mapping = symbolMap.at(trade->symbolIndex);

	auto message = SymbolMessage(
		mapping->symbol,
		trade->price / pow(10, mapping->priceScaleCode),
		trade->sourceTime,
		trade->sourceTimeNs);

	mqConnection->BasicPublish(
		"TEST",	/*The Default Exchange*/
		"key",	/*TODO use the stock ticker as our routing key*/
		AmqpClient::BasicMessage::Create(message.serialize()));
}

void Decoder::handleSequenceNumberReset(char packetData[])
{
	auto message = loadMessage<Message::SequenceNumberReset>(packetData);
}

void Decoder::handleSymbolIndexMapping(char packetData[])
{
	auto message = loadMessage<Message::SymbolIndexMapping>(packetData);
	symbolMap.insert(std::pair(message->symbolIndex, message));
}

void Decoder::handleSymbolClear(char packetData[])
{
	auto message = loadMessage<Message::SymbolClear>(packetData);
	this->symbolMap.erase(message->symbolIndex);
}

void Decoder::handleSecurityStatus(char packetData[])
{
	auto message = loadMessage<Message::SecurityStatus>(packetData);
}

void Decoder::handleSourceTimeReference(char packetData[])
{
	auto message = loadMessage<Message::SourceTimeReference>(packetData);
}

void Decoder::handleQuote(char packetData[])
{
	auto message = loadMessage<Message::Quote>(packetData);
}

void Decoder::handleAddOrder(char packetData[])
{
	auto message = loadMessage<Message::AddOrder>(packetData);
}

void Decoder::handleModifyOrder(char packetData[])
{
	auto message = loadMessage<Message::ModifyOrder>(packetData);
}

void Decoder::handleDeleteOrder(char packetData[])
{
	auto message = loadMessage<Message::DeleteOrder>(packetData);
}

void Decoder::handleReplaceOrder(char packetData[])
{
	auto message = loadMessage<Message::ReplaceOrder>(packetData);
}

void Decoder::handleTradeCancel(char packetData[])
{
	auto message = loadMessage<Message::TradeCancel>(packetData);
}

void Decoder::handleOrderExecution(char packetData[])
{
	auto message = loadMessage<Message::OrderExecution>(packetData);
}

void Decoder::handleImbalance(char packetData[])
{
	auto message = loadMessage<Message::Imbalance>(packetData);
}

void Decoder::handleAddOrderRefresh(char packetData[])
{
	auto message = loadMessage<Message::AddOrderRefresh>(packetData);
}

void Decoder::handleCrossTrade(char packetData[])
{
	auto message = loadMessage<Message::CrossTrade>(packetData);
}

void Decoder::handleNonDisplayedTrade(char packetData[])
{
	auto message = loadMessage<Message::NonDisplayedTrade>(packetData);
}

void Decoder::handleCrossCorrection(char packetData[])
{
	auto message = loadMessage<Message::CrossCorrection>(packetData);
}

void Decoder::handleRetailImprovement(char packetData[])
{
	auto message = loadMessage<Message::RetailImprovement>(packetData);
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
	auto search = msgHandlerMap.find(msgType);
	if(search != msgHandlerMap.end())
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
