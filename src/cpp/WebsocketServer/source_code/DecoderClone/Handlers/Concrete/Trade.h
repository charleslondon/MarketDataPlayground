#pragma once
#include "DecoderClone/Handlers/Abstract/IMessageHandler.h"
#include "DecoderClone/IPC_SymbolMessage.h"
#include "Model/Messages.h"

template<>
struct traits<class Trade>
{
	struct Message
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSequenceNum;
		uint32_t tradeId;
		uint32_t price;
		uint32_t volume;
		char tradeCond1;
		char tradeCond2;
		char tradeCond3;
		char tradeCond4;
	};
};

class Trade : public CMessageHandler<Trade>
{
public:
	static constexpr MessageType MsgType = MessageType::TRADE;

private:
	friend class CMessageHandler<Trade>;

protected:
	bool handleMessage(Message msg)
	{
		try
		{
			auto c = Context::getInstance();
			auto search = c->getSymbolMap()->find(msg.symbolIndex);
			if (search != c->getSymbolMap()->end())
			{
				auto mapping = search->second;

				auto message = IPC_SymbolMessage(
					mapping.symbol,
					msg.price / pow(10, mapping.priceScaleCode),
					msg.sourceTime,
					msg.sourceTimeNs);

				c->getConnection()->BasicPublish(
					"TEST",	/*The Default Exchange*/
					"key",	/*TODO use the stock ticker as our routing key*/
					AmqpClient::BasicMessage::Create(message.serialize()));
			}
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
};