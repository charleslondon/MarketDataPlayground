#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <functional>
#include <set>
#include "Model/Messages.h"
#include "MessageHandler.h"

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
private: /*Members*/
	/*Symbol index to Symbol IndexMessage*/
	std::set<MessageType> unhandledMessages;
	std::unordered_map<MessageType, std::function<void(char[])>> msgHandlerMap;
	std::unordered_map<uint32_t, std::shared_ptr<Message::SymbolIndexMapping>> symbolMap;
	AmqpClient::Channel::ptr_t mqConnection;


public: /*Functions*/
	Decoder();
	void decodePacket(char packetData[], const int packetSize);
	

private: /*Functions*/
	void populateMessageHandlerMap();
	void handleMessage(MessageType msgType, char packetData[]);

	template<typename M>
	std::shared_ptr<M> loadMessage(char packetData[]) const
	{
		M message;
		memcpy(&message, packetData, sizeof(M));
		return std::make_shared<M>(message);
	}

	void handleTrade(char packetData[]) const;
	void handleSequenceNumberReset(char packetData[]);
	void handleSymbolIndexMapping(char packetData[]);
	void handleSymbolClear(char packetData[]);
	void handleSecurityStatus(char packetData[]);
	void handleSourceTimeReference(char packetData[]);
	void handleQuote(char packetData[]);
	void handleAddOrder(char packetData[]);
	void handleModifyOrder(char packetData[]);
	void handleDeleteOrder(char packetData[]);
	void handleReplaceOrder(char packetData[]);
	void handleTradeCancel(char packetData[]);
	void handleOrderExecution(char packetData[]);
	void handleImbalance(char packetData[]);
	void handleAddOrderRefresh(char packetData[]);
	void handleCrossTrade(char packetData[]);
	void handleNonDisplayedTrade(char packetData[]);
	void handleCrossCorrection(char packetData[]);
	void handleRetailImprovement(char packetData[]);
};
