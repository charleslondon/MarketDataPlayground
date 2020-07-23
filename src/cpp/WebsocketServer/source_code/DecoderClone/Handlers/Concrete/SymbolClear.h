#pragma once
#include "DecoderClone/Handlers/Abstract/IMessageHandler.h"
#include "Model/Messages.h"

template<>
struct traits<class SymbolClear>
{
	struct Message
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t nextSourceSeqNum;
	};
};

class SymbolClear : public CMessageHandler<SymbolClear>
{
public:
	static constexpr MessageType MsgType = MessageType::SYMBOL_CLEAR;

private:
	friend class CMessageHandler<SymbolClear>;

protected:
	bool handleMessage(Message msg)
	{
		auto c = Context::getInstance();

		try
		{
			c->getSymbolMap()->erase(msg.symbolIndex);
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
};
