#include "Client.h"

void Client::run()
{
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);

	/*Sit in loop just listening forever*/
	while (true)
	{
		memset(buffer, '\0', MAX_PACKET_SIZE);

		/*RecvFrom is a blocking call, will wait here for the next packet*/
		if (recvfrom(sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr*) & si_other, &slen) != SOCKET_ERROR)
		{
			decoder->decodePacket(buffer, slen);
		}
	}

	closesocket(sock);
	WSACleanup();
}

Client::Client()
{
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE); //Failed to start.
	}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE); //Failed to get socket.
	}

	/* Clear out server struct */
	memset((void*)&server, '\0', sizeof(struct sockaddr_in));

	/*Additional setup for running locally*/
	server.sin_family = AF_INET;
	server.sin_port = htons(29024);
	InetPtonW(AF_INET, (PCWSTR)INADDR_ANY, &server.sin_addr);

	/*Bind the socket to a port.*/
	int bindResult = bind(sock, (struct sockaddr*)&server, sizeof(server));
	if (bindResult == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}