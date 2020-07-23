#pragma once
#include <set>
#include <memory>
#include <unordered_map>
#include "Model/Messages.h"
#include "Model/Enums.h"
#include "DecoderClone/Handlers/Abstract/IMessageHandler.h"

class CRTPDecoder 
{
private:
	std::set<MessageType> unhandledMessages;
	std::unordered_map<MessageType, std::shared_ptr<IMessageHandler>> handlers;

public:
	CRTPDecoder();
	void decodePacket(char packetData[], const int packetSize);
};