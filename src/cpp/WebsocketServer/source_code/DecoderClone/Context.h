#pragma once
#include <unordered_map>
#include <mutex>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "Model/Messages.h"

class Context
{
public:
	using SymbolMap = std::unordered_map<uint32_t, Message::SymbolIndexMapping>;

private:
	std::shared_ptr<SymbolMap> symbolMap;
	AmqpClient::Channel::ptr_t mqConnection;
	static std::shared_ptr<Context> instance;
	static std::mutex singletonLock;

private:
	Context();

public:
	std::shared_ptr<SymbolMap> getSymbolMap();
	AmqpClient::Channel::ptr_t getConnection() const;
	static std::shared_ptr<Context> getInstance();
};