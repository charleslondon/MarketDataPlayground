#pragma once
#include <string>

struct IPC_SymbolMessage
{
	IPC_SymbolMessage() = delete;

	IPC_SymbolMessage(
		const std::string& newSymbol,
		const double newPrice,
		const uint32_t newSourceTime,
		const uint32_t newSourceTimeNs) :
		symbol(newSymbol),
		price(std::to_string(newPrice)),
		time(std::to_string(newSourceTime)
			.append(".")
			.append(std::to_string(newSourceTimeNs))) {}

	std::string serialize() const
	{
		return
			"Symbol:" + symbol +
			",Price:" + price +
			",Time:" + time;
	}

	const std::string symbol;
	const std::string price;
	const std::string time;
};
