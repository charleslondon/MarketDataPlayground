#pragma once
#include <stdio.h>
#include<winsock2.h>
#include <ws2tcpip.h>
#include <memory>
#include "Decoder/Decoder.h"

class Client
{
private: /*Members*/
	struct sockaddr_in server;
	WSAData data;
	SOCKET sock;
	static constexpr int MAX_PACKET_SIZE = 1400;
	char buffer[MAX_PACKET_SIZE];
	std::shared_ptr<Decoder> decoder = std::make_shared<Decoder>();

public: /*Functions*/
	Client();
	void run();
};