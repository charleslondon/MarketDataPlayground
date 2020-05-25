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

namespace Message
{
	struct SequenceNumberReset
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint8_t productId;
		uint8_t channelId;
	};

	struct SymbolIndexMapping
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

	struct SymbolClear
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t nextSourceSeqNum;
	};

	struct SecurityStatus
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

	struct Trade
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

	struct SourceTimeReference
	{
		uint32_t id;
		uint32_t symbolSequenceNum;
		uint32_t sourceTime;
	};

	struct Quote
	{
		uint32_t SourcetimeNs;
		uint32_t SymbolIndex;
		uint32_t SymbolSequenceNum;
		uint32_t askPrice;
		uint32_t askVolume;
		uint32_t bidPrice;
		uint32_t bidVolume;
		QuoteType quoteCondition;
		RetailPriceIndicator rpiIndicator;
	};

	struct AddOrder
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSequenceNum;
		uint64_t orderId;
		uint32_t price;
		uint32_t volume;
		TradeSide side;
		char firmId[5];
		uint8_t numParitySplits;
	};

	struct ModifyOrder
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSequenceNum;
		uint64_t orderId;
		uint32_t price;
		uint32_t volume;
		BookPosition positionChange;
		uint8_t prevPriceParitySplits;
		uint8_t newPriceParitySplits;
	};

	struct DeleteOrder
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSequenceNum;
		uint64_t orderId;
		uint8_t numParitySplits;
	};

	struct ReplaceOrder
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSequenceNum;
		uint64_t orderId;
		uint64_t newOrderId;
		uint32_t price;
		uint32_t volume;
		uint8_t prevPriceParitySplits;
		uint8_t newPriceParitySplits;
	};

	struct OrderExecution
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSequenceNum;
		uint64_t orderId;
		uint32_t tradeId;
		uint32_t price;
		uint32_t volume;
		Printable printableFlag;
		uint8_t numParitySplits;
		uint32_t dbExecId;
	};

	struct AddOrderRefresh
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSeqNum;
		uint64_t orderId;
		uint32_t price;
		uint32_t volume;
		TradeSide side;
		char firmId[5];
		uint8_t numParitySplits;
	};

	struct Imbalance
	{
		uint32_t sourceTime;
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSequenceNum;
		uint32_t referencePrice;
		uint32_t pairedQty;
		uint32_t totalImbalanceQty;
		uint32_t marketImbalanceQty;
		uint16_t auctionTime;
		AuctionType auctionType;
		TradeSide imbalanceSide;
		uint32_t continuousBookClearingPrice;
		uint32_t closingOnlyClearingPrice;
		uint32_t ssrFilingPrice;
		uint32_t indicativeMatchPrice;
		uint32_t upperCollar;
		uint32_t lowerCollar;
		AuctionStatus auctionStatus;
		FreezeStatus freezeStatus;
		uint8_t numExtensions;
	};

	struct NonDisplayedTrade
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSeqNum;
		uint32_t tradeId;
		uint32_t price;
		uint32_t volume;
		Printable printableFlag;
		uint32_t dbExecId;
	};

	struct CrossTrade
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSeqNum;
		uint32_t crossId;
		uint32_t price;
		uint32_t volume;
		CrossType crossType;
	};

	struct CrossCorrection
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSeqNum;
		uint32_t crossId;
		uint32_t volume;
	};

	struct TradeCancel
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSeqNum;
		uint32_t tradeId;
	};

	struct RetailImprovement
	{
		uint32_t sourceTimeNs;
		uint32_t symbolIndex;
		uint32_t symbolSeqNum;
		RetailPriceIndicator rpiIndicator;
	};
}
#pragma pack(pop)
