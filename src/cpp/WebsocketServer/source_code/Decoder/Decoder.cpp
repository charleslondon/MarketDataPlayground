#include <iostream>
#include "Decoder.h"

Decoder::Decoder() :
	symbolMap(std::unordered_map<uint32_t, std::shared_ptr<Message::SymbolIndexMapping>>()),
	mqConnection(AmqpClient::Channel::Create())
{
	populateMessageHandlerMap();
}

/*
	Just a crummy re-implementation of a vtable, but it's prettier than having a massive switch statement,
	may come back to this with dynamic double dispatch or if performance loss is too great some kind of
	CRTP non-sense
*/
void Decoder::populateMessageHandlerMap()
{
	msgHandlerMap.emplace(MessageType::TRADE, [=](char packetData[]) {
		this->handleTrade(loadMessage<Message::Trade>(packetData)); });

	msgHandlerMap.emplace(MessageType::SYMBOL_INDEX_MAPPING, [=](char packetData[]) {
		this->handleSymbolIndexMapping(loadMessage<Message::SymbolIndexMapping>(packetData)); });

	msgHandlerMap.emplace(MessageType::SYMBOL_CLEAR, [=](char packetData[]) {
		this->handleSymbolClear(loadMessage<Message::SymbolClear>(packetData)); });
}

void Decoder::handleTrade(const std::shared_ptr<Message::Trade> msg) const
{
	auto search = symbolMap.find(msg->symbolIndex);
	if (search != symbolMap.end())
	{
		auto mapping = search->second;

		auto message = SymbolMessage(
			mapping->symbol,
			msg->price / pow(10, mapping->priceScaleCode),
			msg->sourceTime,
			msg->sourceTimeNs);

		mqConnection->BasicPublish(
			"TEST",	/*The Default Exchange*/
			"key",	/*TODO use the stock ticker as our routing key*/
			AmqpClient::BasicMessage::Create(message.serialize()));
	}
}

void Decoder::handleSymbolIndexMapping(const std::shared_ptr<Message::SymbolIndexMapping> msg)
{
	symbolMap.insert(std::pair(msg->symbolIndex, msg));
}

void Decoder::handleSymbolClear(const std::shared_ptr<Message::SymbolClear> msg)
{
	//symbolMap.erase(msg->symbolIndex);
}

void Decoder::decodePacket(char packetData[], const int packetSize)
{
	XdpPacketHeader xdpPacketHeader;
	std::memcpy(&xdpPacketHeader, packetData, sizeof(XdpPacketHeader));
	packetData += sizeof(XdpPacketHeader);

	XdpMessageHeader xdpMessageHeader;
	for (int ii = 0; ii < xdpPacketHeader.numberMsgs; ++ii)
	{
		std::memcpy(&xdpMessageHeader, packetData, sizeof(XdpMessageHeader));
		packetData += sizeof(XdpMessageHeader);
		handleMessage(xdpMessageHeader.msgType, packetData);
		packetData += (xdpMessageHeader.msgSize - sizeof(XdpMessageHeader));
	}
}

void Decoder::handleMessage(MessageType msgType, char packetData[])
{
	auto search = msgHandlerMap.find(msgType);
	if (search != msgHandlerMap.end())
	{
		search->second(packetData);
	}
	else if (unhandledMessages.insert(msgType).second)
	{
		std::cout << "Unhandled Message Encountered: Message_Type " << static_cast<int>(msgType) << std::endl;
	}
}
