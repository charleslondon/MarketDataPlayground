#pragma once
#include <cstdint>
#include "DecoderClone/Context.h"
#include "Model/Messages.h"

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

	static Message createMessage(char packetData[])
	{
		Message message;
		std::memcpy(&message, packetData, sizeof(Message));
		return message;
	}

	bool handleMessage(char* packetData)
	{
		return static_cast<Handler*>(this)->handleMessage(createMessage(packetData));
	}
};
