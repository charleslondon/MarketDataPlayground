#pragma once
#include "DecoderClone/Handlers/Abstract/IMessageHandler.h"
#include "Model/Messages.h"

template<>
struct traits<class SymbolIndexMapping>
{
	struct Message
	{
		uint32_t symbolIndex;
		char symbol[11];
		uint8_t reserved1;
		MarketId marketId;
		uint8_t systemId;
		char exchangeCode;
		uint8_t priceScaleCode;
		char securityType;
		uint16_t lotSize;
		uint32_t prevClosePrice;
		uint32_t prevCloseVolume;
		char roundLot;
		uint16_t mpv;
		uint16_t unitOfTrade;
		uint16_t reserved2;
	};
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
			auto c = Context::getInstance();
			//c->getSymbolMap()->insert(std::pair(msg.symbolIndex, msg));
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
};
