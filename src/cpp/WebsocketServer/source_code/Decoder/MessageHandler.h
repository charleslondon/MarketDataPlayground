#pragma once

class IMessageHandler
{
public:
	virtual void handleMessage(char packetData[]) = 0;
};

class TradeHandler : public IMessageHandler
{
public:
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

protected:
	virtual void handleMessage(char packetData[]) override
	{

	}
};

class SequenceNumberResetHandler : public IMessageHandler
{
public:
	struct Message
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint8_t productId;
		uint8_t channelId;
	};

protected:
	virtual void handleMessage(char packetData[]) override
	{

	}
};

class SymbolIndexMappingHandler : public IMessageHandler
{
public:
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

protected:
	virtual void handleMessage(char packetData[]) override
	{

	}
};

class SymbolClearHandler : public IMessageHandler
{
public:
	struct Message
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t nextSourceSeqNum;
	};

protected:
	virtual void handleMessage(char packetData[]) override
	{

	}
};

class SecurityStatusHandler : public IMessageHandler
{
public:
	struct Message
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSeqNum;
		char securityStatus;
		char haltCondition;
		uint32_t reserved;
		uint32_t price1;
		uint32_t price2;
		ExchangeId ssrTriggeringExchangeId;
		uint32_t ssrTriggeringVolume;
		uint32_t time;
		char ssrState;
		SecurityMarketStatus marketState;
		char sessionState;
	};

protected:
	virtual void handleMessage(char packetData[]) override
	{

	}
};