#pragma once
#include <stdint.h>

enum class DeliveryFlag : uint8_t
{
	HEARTBEAT = 1,
	XDP_FAILOVER = 10,
	ORIGINAL_MESSAGE = 11,
	SEQUENCE_NUMBER_RESET_MESSAGE = 12,
	ONLY_ONE_PACKET_IN_RETRANSMISSION_SEQUENCE = 13,
	PART_OF_A_RETRANSMISSION_SEQUENCE = 15,
	ONLY_ONE_PACKET_IN_REFRESH_SEQUENCE = 17,
	START_OF_A_REFRESH_SEQUENCE = 18,
	PART_OF_A_REFRESH_SEQUENCE = 19,
	END_OF_A_REFERESH_SEQUENCE = 20,
	MESSAGE_UNAVAILABLE = 21
};

enum class MessageType : uint16_t
{
	SEQUENCE_NUMBER_RESET = 1,
	SOURCE_TIME_REFERENCE = 2,
	SYMBOL_INDEX_MAPPING = 3,
	RETRANSMISSION_REQUEST = 10,
	REQUEST_RESPONSE = 11,
	HEARTBEAT_RESPONSE = 12,
	SYMBOL_INDEX_MAPPING_REQUEST = 13,
	REFRESH_REQUEST = 15,
	MESSAGE_UNAVAILABLE = 31,
	SYMBOL_CLEAR = 32,
	SECURITY_STATUS = 34,
	REFRESH_HEADER = 35,
	TRF_TRADE = 215,
	TRF_TRADE_CANCEL = 216,
	TRF_TRADE_CORRECTION = 217,
	TRF_PRIOR_DAY_TRADE = 218,
	TRF_PRIOR_DAY_TRADE_CANCEL = 219,
	TRADE = 220,
	TRADE_CANCEL = 221,
	TRADE_CORRECTION = 222,
	STOCK_SUMMARY = 223
};

enum class MarketId : uint16_t
{
	NYSE_EQUITIES = 1,
	NYSE_ARCA_EQUITIES = 3,
	NYSE_ARCA_OPTIONS = 4,
	NYSE_BONDS = 5,
	GLOBAL_OTC = 6,
	NYSE_AMERICAN_OPTIONS = 8,
	NYSE_AMERICAN_EQUITIES = 9,
	NYSE_NATIONAL_EQUITIES = 10
};

enum class SecurityMarketStatus : unsigned char
{
	PRE_OPENING = 'P',
	EARLY_SESSION = 'E',
	CORE_SESSION = 'O',
	LATE_SESSION = 'L',
	CLOSED = 'X'
};

enum class ExchangeId : unsigned char
{
	UNSET_DEFAULT = ' ',
	NYSE_AMERICAN = 'A',
	NASDAQ_OMX_BX = 'B',
	NYSE_NATIONAL = 'C',
	FINRA = 'D',
	ISE = 'I',
	EDGA = 'J',
	EDGX = 'K',
	NYSE_CHICAGO = 'M',
	NYSE = 'N',
	NYSE_ARCA = 'P',
	NASDAQ = 'Q',
	CTS = 'S',
	NASDAQ_OMX = 'T',
	IEX = 'V',
	CBSX = 'W',
	NASDAQ_OMX_PSX = 'X',
	BATS_Y = 'Y',
	BATS = 'Z'
};
