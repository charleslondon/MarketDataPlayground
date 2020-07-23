#pragma once
#include <cstdint>
#include "DecoderClone/Context.h"

template<typename>
struct traits;

struct IMessageHandler
{
	virtual ~IMessageHandler() {}
	virtual bool handleMessage(char* packetData) = 0;
};

template<typename Handler>
class CMessageHandler : public IMessageHandler
{
public:
	using Message = typename traits<Handler>::Message;	
	
	CMessageHandler() {}

	bool handleMessage(char* packetData)
	{
		Message message;
		std::memcpy(&message, packetData, sizeof(Message));
		return static_cast<Handler*>(this)->handleMessage(message);
	}
};
