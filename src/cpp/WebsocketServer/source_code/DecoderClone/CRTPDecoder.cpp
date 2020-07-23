#include <iostream>
#include "CRTPDecoder.h"
#include "Handlers/Concrete/Trade.h"
#include "Handlers/Concrete/SymbolIndexMapping.h"
#include "Handlers/Concrete/SymbolClear.h"

CRTPDecoder::CRTPDecoder() 
{
	handlers.emplace(Trade::MsgType, std::make_shared<CMessageHandler<Trade>>());
	handlers.emplace(SymbolIndexMapping::MsgType, std::make_shared<CMessageHandler<SymbolIndexMapping>>());
	handlers.emplace(SymbolClear::MsgType, std::make_shared<CMessageHandler<SymbolClear>>());
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

		auto search = handlers.find(xdpMessageHeader.msgType);
		if (search != handlers.end())
		{
			search->second->handleMessage(packetData);
		}
		else if (unhandledMessages.insert(xdpMessageHeader.msgType).second)
		{
			std::cout << "Unhandled Message Encountered: Message_Type " << static_cast<int>(xdpMessageHeader.msgType) << std::endl;
		}

		packetData += (xdpMessageHeader.msgSize - sizeof(XdpMessageHeader));
	}
}