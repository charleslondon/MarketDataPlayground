#pragma once
#include <set>
#include <memory>
#include <unordered_map>
#include "Model/Messages.h"
#include "Model/Enums.h"
#include "DecoderClone/Handlers/MessageHandlerThread.h"

class CRTPDecoder
{
private:
	std::set<MessageType> unhandledMessages;
	std::unordered_map<MessageType, std::unique_ptr<MessageHandlerThread>> handlers;

public:
	CRTPDecoder();
	void decodePacket(char* packetData, const int packetSize);
	void decodeMessage(MessageType msgType, char* packetData);
};