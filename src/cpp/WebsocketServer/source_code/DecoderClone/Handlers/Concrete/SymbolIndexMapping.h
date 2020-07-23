#pragma once
#include "DecoderClone/Handlers/Abstract/IMessageHandler.h"
#include "Model/Messages.h"

template<>
struct traits<class SymbolIndexMapping>
{
	using Message = Message::SymbolIndexMapping;
};

class SymbolIndexMapping : public CMessageHandler<SymbolIndexMapping>
{
public:
	static constexpr MessageType MsgType = MessageType::SYMBOL_INDEX_MAPPING;

private:
	friend class CMessageHandler<SymbolIndexMapping>;

protected:
	bool handleMessage(Message msg)
	{
		try
		{
			context.symbolMap.insert(std::pair(msg.symbolIndex, msg));
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
};
