#include "Context.h"

std::shared_ptr<Context> Context::instance = nullptr;
std::mutex Context::singletonLock = std::mutex();

Context::Context() :
	symbolMap(std::make_shared<SymbolMap>()),
	mqConnection(AmqpClient::Channel::Create()) 
{
}

std::shared_ptr<Context::SymbolMap> Context::getSymbolMap()
{
	return symbolMap;
}

AmqpClient::Channel::ptr_t Context::getConnection() const
{
	return mqConnection;
}

std::shared_ptr<Context> Context::getInstance()
{
	std::scoped_lock<std::mutex> lock(singletonLock);

	if (instance == nullptr)
	{
		instance = std::shared_ptr<Context>(new Context());
	}

	return instance;
}