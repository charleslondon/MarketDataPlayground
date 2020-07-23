#pragma once
#include <unordered_map>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "Model/Messages.h"

struct Context
{
	std::unordered_map<uint32_t, Message::SymbolIndexMapping> symbolMap;
	AmqpClient::Channel::ptr_t mqConnection;

	Context() :
		symbolMap(std::unordered_map<uint32_t, Message::SymbolIndexMapping>()),
		mqConnection(AmqpClient::Channel::Create()) {}

	~Context() {}
};