#include <iostream>
#include "CRTPDecoder.h"
#include "Handlers/Concrete/Trade.h"
#include "Handlers/Concrete/SymbolIndexMapping.h"
#include "Handlers/Concrete/SymbolClear.h"


template<typename M>
std::shared_ptr<M> loadMessage(char* packetData)
{
	M message;
	std::memcpy(&message, packetData, sizeof(M));
	return std::make_shared<M>(message);
}

CRTPDecoder::CRTPDecoder()
{
	handlers.emplace(Trade::MsgType,
		std::make_unique<MessageHandlerThread>(std::make_shared<CMessageHandler<Trade>>()));
	handlers.emplace(SymbolIndexMapping::MsgType,
		std::make_unique<MessageHandlerThread>(std::make_shared<CMessageHandler<SymbolIndexMapping>>()));
	handlers.emplace(SymbolClear::MsgType,
		std::make_unique<MessageHandlerThread>(std::make_shared<CMessageHandler<SymbolClear>>()));
}

void CRTPDecoder::decodePacket(char packetData[], const int packetSize)
{
	XdpPacketHeader xdpPacketHeader;
	std::memcpy(&xdpPacketHeader, packetData, sizeof(XdpPacketHeader));
	packetData += sizeof(XdpPacketHeader);

	XdpMessageHeader xdpMessageHeader;
	for (int ii = 0; ii < xdpPacketHeader.numberMsgs; ++ii)
	{
		std::memcpy(&xdpMessageHeader, packetData, sizeof(XdpMessageHeader));
		packetData += sizeof(XdpMessageHeader);
		decodeMessage(xdpMessageHeader.msgType, packetData);
		packetData += (xdpMessageHeader.msgSize - sizeof(XdpMessageHeader));
	}
}

void CRTPDecoder::decodeMessage(MessageType msgType, char packetData[])
{
	auto search = handlers.find(msgType);
	if (search != handlers.end())
	{
		search->second->ProcessMessage(packetData);
	}
	else if (unhandledMessages.insert(msgType).second)
	{
		std::cout << "Unhandled Message Encountered: Message_Type " << static_cast<int>(msgType) << std::endl;
	}
}