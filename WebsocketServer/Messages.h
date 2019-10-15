#pragma once
#include "Enums.h"

#pragma pack(push, 1)
struct XdpPacketHeader
{
	uint16_t pktSize;
	DeliveryFlag deliveryFlag;
	uint8_t numberMsgs;
	uint32_t seqNum;
	uint32_t sendTime;
	uint32_t sendTimeNS;
};

struct XdpMessageHeader
{
	uint16_t msgSize;
	MessageType msgType;
};

struct OrderTradeId
{
	uint8_t systemId;
	MarketId marketId;
	uint32_t orderTradeId;
};

struct SequenceNumberResetMessage
{
	uint32_t sourceTime;
	uint32_t sourceTimeNs;
	uint8_t productId;
	uint8_t channelId;
};

struct SymbolIndexMappingMessage
{
	uint32_t symbolIndex;
	char symbol[11];
	uint8_t reserved1;
	MarketId marketId;
	uint8_t systemId;
	unsigned char exchangeCode;
	uint8_t priceScaleCode;
	unsigned char securityType;
	uint16_t lotSize;
	uint32_t prevClosePrice;
	uint32_t prevCloseVolume;
	unsigned char roundLot;
	uint16_t mpv;
	uint16_t unitOfTrade;
	uint16_t reserved2;
};

struct SymbolClearMessage
{
	uint32_t sourceTime;
	uint32_t sourceTimeNs;
	uint32_t symbolIndex;
	uint32_t nextSourceSeqNum;
};

struct SecurityStatusMessage
{
	uint32_t sourceTime;
	uint32_t sourceTimeNs;
	uint32_t symbolIndex;
	uint32_t symbolSeqNum;
	unsigned char securityStatus;
	unsigned char haltCondition;
	uint32_t reserved;
	uint32_t price1;
	uint32_t price2;
	ExchangeId ssrTriggeringExchangeId;
	uint32_t ssrTriggeringVolume;
	uint32_t time;
	unsigned char ssrState;
	SecurityMarketStatus marketState;
	unsigned char sessionState;
};

struct TradeMessage
{
	uint32_t sourceTime;
	uint32_t sourceTimeNs;
	uint32_t symbolIndex;
	uint32_t symbolSequenceNum;
	uint32_t tradeId;
	uint32_t price;
	uint32_t volume;
	unsigned char tradeCond1;
	unsigned char tradeCond2;
	unsigned char tradeCond3;
	unsigned char tradeCond4;
};
#pragma pack(pop)
